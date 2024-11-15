import  java.util.Random;


class Obraz {
    private int sizeN;
    private int sizeM;
    private char[][] tab;	// tablica znaków
    private char[] tabSymb;
	private int histSize;
    private int[] histogram;	// histogram sekwencyjny
	private int[] histParallel;	// histogram równoległy

	// konstruktor - wypełnianie tablicy znaków
    public Obraz(int n, int m, int histSize) {
		if (histSize > 94) {
			System.out.println("Error: histSize > 94");
			histSize = 94;
		}
		this.histSize = histSize;
		this.sizeN = n;
		this.sizeM = m;
		tab = new char[n][m];
		tabSymb = new char[histSize];

		final Random random = new Random();

		// Wypełnianie tablicy symboli (ASCII 33-127)
		for(int k = 0; k < histSize; k++) {
			tabSymb[k] = (char)(k + 33); // substitute symbols
		}

		// Wypełnianie tablicy losowymi symbolami
		for(int i = 0; i < n; i++) {
			for(int j = 0; j < m; j++) {
				tab[i][j] = tabSymb[random.nextInt(histSize)];  // ascii 33-127
				//tab[i][j] = (char)(random.nextInt(histSize)+33);  // ascii 33-127
				System.out.print(tab[i][j] + " ");
			}
			System.out.print("\n");
		}
		System.out.print("\n\n");

		// Inicjalizacja histogramów
		histogram = new int[histSize];
		histParallel = new int[histSize];
		clearHistogram();
	}

	// Czyszczenie histogramu
    public void clearHistogram(){
		for(int i = 0; i < histSize; i++) {
			histogram[i] = 0;
			histParallel[i] = 0;
		}
    }
	public void clearHistogramParallel(){
		for(int i = 0; i < histSize; i++) {
			histParallel[i] = 0;
		}
	}

	// Obliczanie histogramu sekwencyjnie
    public void calculateHistogram(){
		for(int i = 0; i < sizeN; i++) {
			for(int j = 0; j < sizeM; j++) {
				histogram[(int) tab[i][j] - 33]++;
	    	}
	 	}
    }

	// Obliczanie histogramu równolegle dla jednego znaku
	public void calculateHistogramParallel(){

	}
	// synchronized-> tylko jeden watek moze uzyskac dostep do tej metody w danym momencie
	public synchronized void calculateHistogramPart(int start, int end){
		char[] chars = new char[end - start + 1];

		for (int i = start; i <= end; i++) {
			chars[i - start] = (char)i;
		}

		// Print the array of characters
		System.out.println(chars);
		for(int i = 0; i < sizeN; i++) {
			for (int j = 0; j < sizeM; j++) {
				if (isCharInArray(tab[i][j], chars)) {
					for (int k = 0; k < 94; k++) {
						if (tab[i][j] == tabSymb[k]) histParallel[k]++;
					}
				}
			}
		}

		int len = end - start;
		for (int i = 0; i < len; i++) {
			int res = histogram[start + i + 33];
			System.out.print(chars[i] + " ");
			for (int j = 0; j < res; j++)
				System.out.print("=");
			System.out.print("\n");
		}
	}

	// drukowanie histogramu
	public void print_histogram(){
		for(int i = 0; i< histSize; i++) {
			System.out.print(tabSymb[i]+" "+histogram[i]+"\n");
			//System.out.print((char)(i+33)+" "+histogram[i]+"\n");
		}
	}

	// Znajdowanie indexu elementu
	private int getIndexOfElement(char sign) {
		for (int i = 0; i < histSize; i++)
			if (tabSymb[i] == sign)
				return i;

		return -1;
	}

	// zliczanie konkretnego znaku
	public int countSign(char sign){
		int counter = 0;
		for(int i = 0; i < sizeN; i++) {
			for(int j = 0; j < sizeM; j++) {
				if(tab[i][j] == sign) {
					counter++;
				}
			}
		}
		return counter;
	}

	public int[] getHistogramParallel() {
		return histParallel;
	}
	public char getSymbol(int index) {
		return tabSymb[index];
	}

	// Porównywanie dwóch histogramów
	public void compare() {
		System.out.println("Symbol	Sekwencyjne   Równolegle");
		for (int i = 0; i < histSize; i++) {
			System.out.print(tabSymb[i] + " " + histogram[i] + " " + histParallel[i] + "\n");
		}
	}

	// wyszukiwanie znaku w tablicy
	private boolean isCharInArray(char ch, char[] chars) {
		for(int i = 0; i < chars.length; i++) {
			if(ch == chars[i]) {
				return true;
			}
		}
		return false;
	}

	int getHistSize(){
		return histSize;
	}
}
