import java.util.Arrays;
import java.util.concurrent.RecursiveTask;
import java.util.concurrent.ForkJoinPool;

    // klasa dla zadania dzielacego i sortujacego
    class DivideTask extends RecursiveTask<int []> {

        private final int[] arrayToDivide;

        public DivideTask(int[] arrayToDivide) {
            this.arrayToDivide = arrayToDivide;
        }

        @Override
        protected int[] compute() {
            // warunek stopu
            if (arrayToDivide.length <= 1) return arrayToDivide;

            // podzial tablicy na dwie czesci
            int mid = arrayToDivide.length / 2;
            int[] leftArr = Arrays.copyOfRange(arrayToDivide, 0, mid);
            int[] rightArr = Arrays.copyOfRange(arrayToDivide, mid, arrayToDivide.length);

            // tworzenie zadan dla obu czesci
            DivideTask task1 = new DivideTask(leftArr);
            DivideTask task2 = new DivideTask(rightArr);

            // uruchomienie obu zadan rownolegle
            task1.fork();
            task2.fork();

            // oczekiwanie na wyniki
            int[] sortedLeft = task1.join();
            int[] sortedRight = task2.join();

            // scalanie posortowanych czesci
            int[] result = new int[arrayToDivide.length];

            scal_tab(sortedLeft, sortedRight, result);

            return result;
        }

        // funkcja scalajaca tablice
        private void scal_tab(
                int[] tab1,
                int[] tab2,
                int[] scal_tab) {

            int i = 0, j = 0, k = 0;

            while ((i < tab1.length) && (j < tab2.length)) {

                if (tab1[i] < tab2[j]) {
                    scal_tab[k] = tab1[i++];
                } else {
                    scal_tab[k] = tab2[j++];
                }

                k++;
            }

            if (i == tab1.length) {

                for (int a = j; a < tab2.length; a++) {
                    scal_tab[k++] = tab2[a];
                }

            } else {

                for (int a = i; a < tab1.length; a++) {
                    scal_tab[k++] = tab1[a];
                }

            }
        }

    }

