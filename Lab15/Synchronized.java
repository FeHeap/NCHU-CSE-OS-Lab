import java.lang.Thread;

public class Synchronized {
	public static int c = 0;
	public static void main(String[] args) {
		job j = new job();
		Thread t1 = new Thread(j);
		Thread t2 = new Thread(j);
		Thread t3 = new Thread(j);
		Thread t4 = new Thread(j);
		t1.start();
		t2.start();
		t3.start();
		t4.start();
		try {
			t1.join();
			t2.join();
			t3.join();
			t4.join();
		} catch(InterruptedException e) {
		}
		System.out.println("Count = " + c);
	}
}


class job implements Runnable {
	private int flag = 1;
	public void run() {
		if(flag == 1) {
			flag = -flag;
			for(int i = 0; i < 25000000; i++) {
				add();
			}
		}
		else {
			flag = -flag;
			for(int i = 0; i < 25000000; i++) {
				sub();
			}
		}
	}

	private synchronized void add() {
		Synchronized.c++;
	}

	private synchronized void sub() {
		Synchronized.c--;
	}
}

