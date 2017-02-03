typedef struct tab {
	int nlins;
	int ncols;
	int nseg_lins[100];
	int nseg_cols[100];
	char tab[102][102];
} tabuleiro;

typedef struct dados {
	int val;
	int alt;
} data;