import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class ParallelIntegral {

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        // przedzial do calkowania
        double xp = 0;          // poczatek
        double xk = Math.PI;    // koniec

        // wczytanie parametrow
        System.out.println("Podaj szerokosc trapezu (dx): ");
        double dx = sc.nextDouble();    // dokladnosc (szerokosc trapezu)
        System.out.println("Podaj liczbe watkow (threads): ");
        int numThreads = sc.nextInt();
        System.out.println("Podaj liczbe zadan (threads): ");
        int numTask = sc.nextInt();

        double intervalWidth = (xk -xp) / numTask;  // przedzial dla jednego taska

        // utworzenie puli watkow
        ExecutorService executor = Executors.newFixedThreadPool(numThreads);    // odpowiada za tworzenie i obsluge puli watkow
        List<Future<Double>> futures = new ArrayList<>();   // Future umożliwia efektywne zbieranie wyników z wielu wątków działających równolegle

        // tworzenie i dodawanie zadan do puli
        for (int i = 0; i < numTask; i++) {
            double subXp = xp + i * intervalWidth;
            double subXk = subXp + intervalWidth;
            CalkaCallable task = new CalkaCallable(subXp, subXk, dx);
            futures.add(executor.submit(task));     // dodawanie do puli rozwiazan
        }

        // odbieranie wynikow zadan
        double totalIntegral = 0;
        for (Future<Double> future : futures) {
            try{
                totalIntegral += future.get();      // oczekiwanie na wynik
            } catch (InterruptedException | ExecutionException e) {
                System.err.println("Wystąpił błąd podczas obliczania: " + e.getMessage());
            }
        }

        // zamkniecie puli watkow
        executor.shutdown();

        // wyswietlanie wyniku
        System.out.printf("Wynik całkowania funkcji sin(x) w przedziale [%.2f, %.2f] wynosi: %.6f%n", xp, xk, totalIntegral);

        sc.close();
    }
}
