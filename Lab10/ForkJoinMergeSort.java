import java.util.concurrent.ExecutionException;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.RecursiveTask;
import java.util.stream.LongStream;

public class ForkJoinMergeSort {
	// create forkjoin pool
	private ForkJoinPool pool;
	// write the forkjointask with RecursiveTask
	private static class MergeSort extends RecursiveTask<long[]> {
		private long[] numbers;	
		private int from;
		private int to;
		public MergeSort(long[] numbers, int from, int to) {
			this.numbers = numbers;
			this.from = from;
			this.to = to;		
		}

		@Override
		protected long[] compute() {
			if(to != from) {
				int split = (from + to) / 2;
				int length = to - from + 1;
				long[] numbersBuff = new long[to - from + 1];
				for(int i = 0; i < length; i++) {
					numbersBuff[i] = numbers[from + i];
				}
				MergeSort f1 = new MergeSort(numbersBuff, 0, split - from);
				MergeSort f2 = new MergeSort(numbersBuff, split+1 - from, to - from);
				f1.fork();
				long[] f2Arr = f2.compute();
				long[] f1Arr = f1.join();
				int j1 = 0, j2 = split+1 - from;
				for(int i = from; i <= to; i++) {
					if(j2 >= length) {
						numbers[i] = f1Arr[j1++];
						continue;
					}
					if(j1 > split - from){
						numbers[i] = f2Arr[j2++];
						continue;
					}

					if(f1Arr[j1] <= f2Arr[j2]) {
						numbers[i] = f1Arr[j1++];
					}
					else {
						numbers[i] = f2Arr[j2++];
					}
				}
			}
			return numbers;
		}

	}
	// initialize
	public ForkJoinMergeSort() {
		pool = new ForkJoinPool();
	}
	// invoke the task to pool
	public long[] sorting(long[] numbers) {
		pool.invoke(new MergeSort(numbers, 0, numbers.length-1));
		return numbers;
	}
}
