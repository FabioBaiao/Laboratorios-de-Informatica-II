int VComand (tabuleiro *t, data *r, undo *p, undotab *ut, int n);

int Vagua (tabuleiro *t);

void E1Comand (tabuleiro *t, data *r, undo *p, int n);

void E1diagonais (tabuleiro *t, data *r, undo *p, int n, int l, int c);

void E1centro (tabuleiro *t, data *r, undo *p, int n, int l, int c);

void E1esquerda (tabuleiro *t, data *r, undo *p, int n, int l, int c);

void E1direita (tabuleiro *t, data *r, undo *p, int n, int l, int c);

void E1cima (tabuleiro *t, data *r, undo *p, int n, int l, int c);

void E1baixo (tabuleiro *t, data *r, undo *p, int n, int l, int c);

void E1todas (tabuleiro *t, data *r, undo *p, int n, int l, int c);

int barco (char c);

void E2Comand (tabuleiro *t, data *r, undo *p, int n);

void E3Comand (tabuleiro *t, data *r, undo *p, int n);

void E3linha (tabuleiro *t, data *r, undo *p, int n, int l);

void E3coluna (tabuleiro *t, data *r, undo *p, int n, int c);

void E3ocupados (tabuleiro *t, data *r, undo *p, int n, int l, int c);

void DComand (tabuleiro *t, undo *p, undotab *ut);

void undoToTab (tabuleiro *t, undotab *ut);

void eComand (tabuleiro *t,char *local);

int lComand (tabuleiro *t,char *local);