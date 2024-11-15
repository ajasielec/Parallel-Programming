import java.util.Scanner;

class Histogram_test {
	public static void main(String[] args) {

		Scanner scanner = new Scanner(System.in);

		System.out.println("Set image size: n (#rows), m(#kolumns)");
		int n = scanner.nextInt();
		int m = scanner.nextInt();
		int histogramSize = 94;

		System.out.println("Set number of threads");
		int num_threads = scanner.nextInt();
		histogramSize = num_threads;

		Obraz obraz_1 = new Obraz(n, m, histogramSize);

		// obliczanie sekwencyjne
		obraz_1.calculateHistogram();
		// obraz_1.print_histogram();


		// wariant 1 (Thread) - jeden watek zlicza 1 znak
		System.out.println("Wariant 1");
		Watek[] threads1 = new Watek[num_threads];

		// Tworzenie watkow
		for (int i = 0; i < num_threads; i++) {
			char character = obraz_1.getSymbol(i);
			threads1[i] = new Watek(obraz_1, character, i);
			threads1[i].start();

			try {
				Thread.sleep(30);
			}catch (InterruptedException e) {
				System.out.println(e);
			}
		}

		// Czekanie na zakończenie wszystkich wątków
		for (int i = 0; i < num_threads; i++) {
			try{
				threads1[i].join();
			} catch(InterruptedException e){
				System.out.println(e);
			}
		}
		// Porównanie wyników
		System.out.println("Porównanie ");
		obraz_1.compare();
		obraz_1.clearHistogram();


		// wariant 2 (Runnable) - podział blokowy
		System.out.println("\nWariant 2");

		Thread[] threads2 = new Thread[num_threads];

		histogramSize = 94;

		Obraz obraz_2 = new Obraz(n, m, histogramSize);

		// obliczanie sekwencyjne
		obraz_2.calculateHistogram();
		// obraz_2.print_histogram();

		// obliczenia rownolegle
		for (int i = 0; i < num_threads; i++) {
			threads2[i] = new Thread(new WatekRunnable(obraz_2, i, num_threads));
			threads2[i].start();
		}

		for (int i = 0; i < num_threads; i++) {
			try {
				threads2[i].join();
//				if (threads2[i].isAlive()) {
//					threads2[i].interrupted();
//					threads2[i].join();
//				}
			} catch (InterruptedException e) {
				System.out.println(e);
			}
		}

		obraz_2.compare();
		obraz_2.clearHistogramParallel();
    }
}

