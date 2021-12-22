import java.lang.Thread;
import java.util.concurrent.locks.*;

public class ReentrantLocks {
	public static void main(String[] args) {
		Warehouse j = new Warehouse();
		Thread t1 = new Thread(j);
		Thread t2 = new Thread(j);
		t1.start();
		t2.start();
		try {
			t1.join();
			t2.join();
		} catch(InterruptedException e) {
		}
	}
}


class Warehouse implements Runnable { // it's a monitor
	
	private Lock lock = new ReentrantLock();
	private Condition threadCond = lock.newCondition();
	private int flag = 1;
	private int product = 0;

	public void run() {
		if(flag == 1) {
			flag = -flag;
			for(int i = 0; i < 25000000; i++) {
				setProduct();
			}
		}
		else {
			flag = -flag;
			for(int i = 0; i < 25000000; i++) {
				getProduct();
			}
		}
	}

	public void setProduct() {
		lock.lock();	
		try {
			while(product == 10) { // warehouse is full, so wait
				try {
					System.out.printf("FULL %n");
					threadCond.await();
				} catch(InterruptedException e) { e.printStackTrace(); }
			}
			product += 1; // set product
			System.out.printf("producer:%d %n", product);
			threadCond.signal();
		}
		finally {
			lock.unlock();
		}
	}
	
	public void getProduct() {
		lock.lock();	
		try {
			while(product == 0) { // warehouse is empty, so wait
				try {
					System.out.printf("	EMPTY %n");
					threadCond.await();
				} catch(InterruptedException e) { e.printStackTrace(); }
			}
			product -= 1; // get product
			System.out.printf("	Consumer:%d %n", product);
			threadCond.signal();
		}
		finally {
			lock.unlock();
		}
	}
}
