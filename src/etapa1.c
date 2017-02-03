#include "estrutura.h"
#include "stack.h"
#include "etapa1.h"
#include "etapa2.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXBUF 1024

/**

Executa a ação correspondente ao comando c.

Lendo a partir do standard input, cria o tabuleiro, isto é, o tamanho do tabuleiro, o número de segmentos em cada linha e coluna e por fim o estado de cada posição.

@param t apontador para a estrutura onde é guardado o tabuleiro

@returns 0 se tudo correr bem, ou -1 se houver algum erro na leitura do tabuleiro

*/
int cComand (tabuleiro *t){
	int l, c;
	char linha[MAXBUF];
	char resto [MAXBUF];
	if (fgets (linha, MAXBUF, stdin) == NULL)
		return -1;
	sscanf(linha, "%d %d", &(t -> nlins), &(t -> ncols));
	if (fgets (linha, MAXBUF, stdin) == NULL)
		return -1;
	for (l = 0; l < (t -> nlins); l++) {
		sscanf(linha, "%d %[^\n]", &(t -> nseg_lins[l]), resto);
		strcpy(linha, resto);
	}
	if (fgets (linha, MAXBUF, stdin) == NULL)
		return -1;
	for (l = 0; l < (t -> ncols); l++){
		sscanf (linha, "%d %[^\n]", &(t -> nseg_cols[l]), resto);
		strcpy (linha, resto);
	}
	for (c = 0; c <= (t -> ncols + 1); c++){
		t -> tab [0][c] = '~';
		t -> tab [(t -> nlins) + 1][c] = '~';
	} 
	for (l = 0; l <= (t -> nlins + 1); l++){
		t -> tab [l][0] = '~';
		t -> tab [l][(t -> ncols) + 1] = '~';
	}
	for (l = 1; l <= (t -> nlins); l++){
		if (fgets (linha, MAXBUF, stdin) == NULL)
			return -1;
		for (c = 1; c <= (t -> ncols); c++){
			sscanf (linha, "%c%[^\n]", &(t -> tab[l][c]), resto);
			strcpy (linha, resto);
		}
	}
	return 0;
}

/**

Executa a ação correspondente ao comando m.

Mostra o estado do tabuleiro no standard output, bem como o número de segmentos em cada linha e coluna.

@param t apontador para a estrutura onde é guardado o tabuleiro

*/
void mComand (tabuleiro *t){
	int c, l;
	for (l = 1; l <= (t -> nlins); l++){
		for (c = 1; c <= (t -> ncols); c++){
			printf ("%c", t -> tab [l][c]);
		}
		printf (" %d\n", t -> nseg_lins [l-1]);
	}
	for (c = 0; c < (t -> ncols); c++){
		printf ("%d", t -> nseg_cols [c]);
	}
	printf ("\n");
}

/**

Executa a ação correspondente ao comando h.

Preenche com água todas as posições sem valor determinado da linha l do tabuleiro e guarda na stack a linha, a coluna e o caracter que lá existia (nesta função será sempre o '.') antes de ser colocado o novo.
É ainda atribuído o valor 1 ao campo alterações (alt) da estrutura data, pois é importante saber que houve alterações na execução do comando R.

@param t apontador para a estrutura onde é guardado o tabuleiro

@param r apontador para a estrutura com o valor que indica se a função efetuou alterações ou se o tabuleiro é inválido

@param p apontador para a stack principal

@param n indice do comando atual

@param l índice da linha a ser preenchida

*/
void hComand (tabuleiro *t, data *r, undo *p, int n, int l){
	int c;
	for (c = 1; c <= (t -> ncols); c++){
		if (t -> tab [l][c] == '.'){ 
			r -> alt = 1;
			(*p) = insere (p, n, l, c, '.');
			t -> tab [l][c] = '~';
		}
	}
} 

/**

Executa a ação correspondente ao comando v.

Preenche com água todas as posições sem valor determinado da coluna c do tabuleiro e guarda na stack a linha, a coluna e o caracter que lá existia (nesta função será sempre o '.') antes de ser colocado o novo.
É ainda atribuído o valor 1 ao campo alterações (alt) da estrutura data, pois é importante saber que houve alterações na execução do comando R.

@param t apontador para a estrutura onde é guardado o tabuleiro

@param r apontador para a estrutura com o valor que indica se a função efetuou alterações ou se o tabuleiro é inválido

@param p apontador para a stack principal

@param n indice do comando atual

@param c índice da coluna a ser preenchida

*/
void vComand (tabuleiro *t, data *r, undo *p, int n, int c){
	int l;
	for (l = 1; l <= (t -> nlins); l++){
		if (t -> tab [l][c] == '.'){
			r -> alt = 1;
			(*p) = insere (p, n, l, c, '.');
			t -> tab [l][c] = '~';
		}
	}
}

/**

Executa a ação correspondente ao comando p.

Se o caracter que existe no tabuleiro nas coordenadas (l,c) for igual ao caracter a alterar a função termina sem serem efetuadas alterações.
Caso contrário, dependendo do caracter que se pretende acrescentar, a função é executada. Se a alteração não for válida, é atribuído o valor 1 ao campo validar (val) da estrutura data, informação esta, relevante para a execução do comando V.
É ainda atribuído o valor 1 ao campo alterações (alt) da estrutura data, pois, na execução do comando R, será importante saber se houve alterações.

@param t apontador para a estrutura onde é guardado o tabuleiro

@param r apontador para a estrutura com o valor que indica se a função efetuou alterações ou se o tabuleiro é inválido

@param p apontador para a stack principal

@param n indice do comando atual

@param a caractere a ser colocado no tabuleiro

@param l índice da linha correspondente à posição a ser preenchida

@param c índice da coluna correspondente à posição a ser preenchida

*/
void pComand (tabuleiro *t, data *r, undo *p, int n, char a, int l, int c){
	char ch = t -> tab [l][c];
	if (ch != a){
		if (a == '~'){
			if (ch == '.') {
				(*p) = insere (p, n, l, c, ch);
				t -> tab [l][c] = a;
				r -> alt = 1;
			}
			if (barco (ch) == 1 || ch == 'o'){
				(*p) = insere (p, n, l, c, ch);
				t -> tab [l][c] = a;
				r -> val = 1;
				r -> alt = 1;
			}
		}
		if (a == 'o'){
			if (ch == '.'){
				(*p) = insere (p, n, l, c, ch);
				t -> tab [l][c] = a;
				r -> alt = 1;
			}
			if (ch == '~'){
				(*p) = insere (p, n, l, c, ch);
				t -> tab [l][c] = a;
				r -> val = 1;
				r -> alt = 1;
			}
		}
		if (barco (a) == 1){
			if (ch == '.' || ch == 'o'){
				(*p) = insere (p, n, l, c, ch);
				t -> tab [l][c] = a;
				r -> alt = 1;
			}
			if (ch == '~'){
				(*p) = insere (p, n, l, c, ch);
				t -> tab [l][c] = a;
				r -> val = 1;
				r -> alt = 1;
			}
		}
		if (a == '.'){
			(*p) = insere (p, n, l, c, ch);
			t -> tab [l][c] = a;
			r -> alt = 1;
		}
	}
}