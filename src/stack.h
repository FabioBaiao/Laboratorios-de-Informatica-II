typedef struct stack {
	int lin;
	int col;
	char caracter;
	int num;
	struct stack *prox;
} *undo;

typedef struct tabstack {
	int nlins;
	int ncols;
	int nseg_lins[100];
	int nseg_cols[100];
	char tab[102][102];
	struct tabstack *prox;
} *undotab;

undo insere (undo *p, int n, int l, int c, char ch);

undotab tabToUndo (tabuleiro *t, undotab *ut);