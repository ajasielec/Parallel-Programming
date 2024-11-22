import java.util.Random;
import java.util.concurrent.ForkJoinPool;
import java.util.stream.IntStream;

public class DivideTaskTest {
    // funkcja testujaca dzialanie
    public static void main(String[] args) {
        int[] array = IntStream.generate(() -> new Random().nextInt(100)).limit(100).toArray();

        for (int num : array) {
            System.out.print(num + " ");
        }

        // tworzenie forkjoinpool i zadania glownego
        ForkJoinPool pool = new ForkJoinPool();
        DivideTask mainTask = new DivideTask(array);

        // uruchomienie algorytmu sortowania
        int[] sortedArray = pool.invoke(mainTask);

        // wyswietlenie wynikow
        System.out.println("Posortowana tablica:");
        for (int num : sortedArray) {
            System.out.print(num + " ");
        }
    }
}
