
public class Watek extends Thread {
    private static final Object STATIC_MONITOR = new Object();
    private final Obraz obraz;
    private final char character;
    private final int index;

    public Watek(Obraz obraz, char character, int index) {
        this.obraz = obraz;
        this.character = character;
        this.index = index;
    }

    public void run() {
        try {
            Thread.sleep(30);

            synchronized (STATIC_MONITOR) {
                int res = obraz.countSign(character);

                if (res != 0) {
                    System.out.print("Thread " + index + ": " + character + " [" + res + "] ");
                    for (int i = 0; i < res; i++)
                        System.out.print("=");
                    System.out.print("\n");

                    int cal = character - 33;
                    obraz.getHistogramParallel()[cal] = res;
                }
            }
        } catch (InterruptedException e) {
            System.out.println("Thread " + index + " został przerwany");
        }
    }
}
