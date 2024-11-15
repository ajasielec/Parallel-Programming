public class WatekRunnable implements Runnable {
    private static final Object STATIC_MONITOR = new Object();
    private Obraz obraz;
    private int id;
    private int threadCount;

    public WatekRunnable(Obraz obraz, int id, int threadCount) {
        this.obraz = obraz;
        this.id = id;
        this.threadCount = threadCount;
    }

    @Override
    public void run() {
        synchronized (STATIC_MONITOR) {
            int elemNaWatek = (int) Math.ceil(obraz.getHistSize() / threadCount);
            int start = id * elemNaWatek;
            int end = (id + 1) * elemNaWatek;
            if (end > obraz.getHistSize()) end = obraz.getHistSize();
            int step = 1;

            for (int i = start + 33; i < end + 33; i += step) {

                char character = (char) i;
                int res = obraz.countSign(character);

                if (res != 0) {
                    // Wyświetl wynik
                    System.out.print("Runnable: " + id + ": " + character + " [" + res + "] ");
                    for (int j = 0; j < res; j++)
                        System.out.print("=");

                    System.out.print("\n");

                    int cal = character - 33;
                    obraz.getHistogramParallel()[cal] = res;
                }
            }
        }
    }
}

