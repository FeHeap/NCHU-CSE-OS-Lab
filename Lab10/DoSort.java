import java.util.concurrent.ExecutionException;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.RecursiveTask;
import java.util.stream.LongStream;
import java.util.Random;

public class DoSort {
        public static void main(String[] args) {
                // create a 100 length array
                long[] numbers = LongStream.rangeClosed(1, 100).toArray();
                ForkJoinMergeSort sort = new ForkJoinMergeSort();


		// shaffle numbers
		int range = 100;
		for(int buf; range > 0;){
			buf = (int)(Math.random() * range);
			long temp = numbers[buf];
			numbers[buf] = numbers[--range];
			numbers[range] = temp;
		}
		
		// sort numbers
		numbers = sort.sorting(numbers);
		
		// print numbers sorted
		for(int i = 0; i < 100; i++) {
			System.out.println(numbers[i]);
		}
        }
}

