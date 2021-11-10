public class lab2 {
	public static int count = 0;	
	public static void main(String argv[]) {
		Thread[] t_count = new Thread[4];
		for(int i = 0; i < 4; i++) {
			t_count[i] = new Thread(new countClass(i));
		}
		for(int i = 0; i < 4; i++) {
			t_count[i].start();	
		}
		try {
			for(int i = 0; i < 4; i++) {
				t_count[i].join();	
			}	
		}
		catch(InterruptedException ie) {
			System.err.println(ie);		
		}
		System.out.printf("The count value is: %d\n", lab2.count);
	}
}

class countClass implements Runnable {
	private int number;	
	
	public countClass(int number) {
		this.number = number;
	}
	public void run() {
		for(int i = 0; i < 5; i++) {
			lab2.count++;
			System.out.printf("Thread Name is: Thread-%d, num is %d\n", number, lab2.count);
		}
	}	
}
