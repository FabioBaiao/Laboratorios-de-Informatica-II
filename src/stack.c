#include "estrutura.h"
#include "stack.h"
#include <stdlib.h>

/**

Insere um conjunto de dados numa stack, à cabeça, para posteriormente poder ser executado o comando D.

@param p apontador para a stack principal

@param n índice do comando a ser inserido na stack

@param l índice da linha a ser alterada

@param c índice da coluna a ser alterada

@param ch caractere a ser inserido na posição especificada

@returns apontador para a estrutura com a stack utilizada no comando D, após a inserção à cabeça das informações da última posição alterada

*/
undo insere (undo *p, int n, int l, int c, char ch){
	undo nova = (undo) malloc (sizeof (struct stack));
	nova -> lin = l; 
	nova -> col = c; 
	nova -> caracter = ch;
	nova -> num = n;
	nova -> prox = (*p);
	return nova;
}

/**

Copia o tabuleiro atual para a cabeça de uma stack, pois será criado um novo tabuleiro e para posteriormente poder ser executado o
comando D.

@param t apontador para a estrutura onde é guardado o tabuleiro

@param ut apontador para a stack de tabuleiros

@returns apontador para a estrutura com a stack utilizada no comando D para os casos em que o último comando foi o comando c, após a inserção à cabeça das informações do novo tabuleiro

*/
undotab tabToUndo (tabuleiro *t, undotab *ut){
	int l, c;
	undotab nova = (undotab) malloc (sizeof (struct tabstack));
	nova -> nlins = t -> nlins;
	nova -> ncols = t -> ncols;
	for (l = 0; l < (t -> nlins); l++){
		nova -> nseg_lins [l] = t -> nseg_lins [l];
	}
	for (c = 0; c < (t -> ncols); c++){
		nova -> nseg_cols [c] = t -> nseg_cols [c];
	}
	for (l = 0; l <= (t -> nlins) + 1; l++){
		for (c = 0; c <= (t -> ncols) + 1; c++){
			nova -> tab [l][c] = t -> tab [l][c];
		}
	}
	nova -> prox = (*ut);
	return nova;
}