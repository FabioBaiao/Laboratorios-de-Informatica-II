#include "estrutura.h"
#include "stack.h"
#include "etapa1.h"
#include "etapa2.h"
#include "etapa3.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXBUF 1024

/**

Executa a ação correspondente ao comando V.

São adicionados dados à stack, pois como no final da execução desta função é executado o comando D, caso os passos anteriores não alterem o estado do tabuleiro, esse comando D, seria executado sobre dados que não deveriam ser mexidos neste comando.
São executadas várias estratégias para determinar se o tabuleiro atual é válido.

@param t apontador para a estrutura onde é guardado o tabuleiro

@param r apontador para a estrutura com o valor que indica se a função efetuou alterações ou se o tabuleiro é inválido

@param p apontador para a stack principal

@param ut apontador para a stack de tabuleiros

@param n índice do comando atual

@returns 0 se o tabuleiro é válido, 1 caso contrário

*/
int VComand (tabuleiro *t, data *r, undo *p, undotab *ut, int n){
	int v = 0;
	(*p) = insere (p, n, 0, 0, '~');
	EA1 (t, r, p, n);
	E1Comand (t, r, p, n);
	if (Vagua (t) == 1) v = 1;
	else { 
		E2Comand (t, r, p, n);
		E3Comand (t, r, p, n);
	}
	DComand (t, p, ut);
	if (r -> val == 1) v = 1;
	return v;
}

/**

Vagua é uma função auxiliar, que verifica se existe água em coordenadas do tabuleiro onde não podem existir devido à presença de segmentos de barcos nas redondezas dessas coordenadas.

@param t apontador para a estrutura onde é guardado o tabuleiro

@returns 0 se o tabuleiro é válido, 1 caso contrário

*/
int Vagua (tabuleiro *t){
	int l, c, r = 0;
	char cima, baixo, esq, dir;
	for (l = 1; l <= (t -> nlins); l++){
		for (c = 1; c <= (t -> ncols); c++){
			cima = t -> tab [l-1][c];
			baixo = t -> tab [l+1][c];
			esq = t -> tab [l][c-1];
			dir = t -> tab [l][c+1];
			if (t -> tab [l][c] == '#'){
				if (cima == '~' && esq == '~') r = 1;
			}
			if (t -> tab [l][c] == '<'){
				if (dir == '~') r = 1;
			}
			if (t -> tab [l][c] == '>'){
				if (esq == '~') r = 1;
			}
			if (t -> tab [l] [c] == '^'){
				if (baixo == '~') r = 1;
			}
			if (t -> tab [l] [c] == 'v'){
				if (cima == '~') r = 1;
			}
		}
	}
	return r;
}

/**

Executa a ação correspondente ao comando E1.

É percorrido todo o tabuleiro, com o objetivo de encontrar segmentos de barcos para aplicar as alterações correspondentes a esse segmento.

@param t apontador para a estrutura onde é guardado o tabuleiro

@param r apontador para a estrutura com o valor que indica se a função efetuou alterações ou se o tabuleiro é inválido

@param p apontador para a stack principal

@param n índice do comando atual

*/
void E1Comand (tabuleiro *t, data *r, undo *p, int n) {
	int l, c;
	for (l = 1; l <= (t -> nlins); l++) {
		for (c = 1; c <= (t -> ncols); c++) {
			if (t -> tab [l][c] == '#')
				E1centro (t, r, p, n, l, c);
			if (t -> tab [l][c] == 'o')
				E1diagonais (t, r, p, n, l, c);
			if (t -> tab [l][c] == '<')
				E1esquerda (t, r, p, n, l, c);
			if (t -> tab [l][c] == '>')
				E1direita (t, r, p, n, l, c);
			if (t -> tab [l] [c] == '^')
				E1cima (t, r, p, n, l, c);
			if (t -> tab [l] [c] == 'v')
				E1baixo (t, r, p, n, l, c);
			if (t -> tab [l][c] == 'O')
				E1todas (t, r, p, n, l, c);
		}
	}
}

/**

Executa a ação correspondente ao comando E1.

É percorrido todo o tabuleiro, com o objetivo de encontrar segmentos de barcos para aplicar as alterações correspondentes a esse segmento.

@param t apontador para a estrutura onde é guardado o tabuleiro

@param r apontador para a estrutura com o valor que indica se a função efetuou alterações ou se o tabuleiro é inválido

@param p apontador para a stack principal

@param n índice do comando atual

@param l índice da linha correspondente à posição recebida

@param c índice da coluna correspondente à posição recebida

*/
void E1diagonais (tabuleiro *t, data *r, undo *p, int n, int l, int c) {
	pComand (t, r, p, n, '~', l-1, c-1);
	pComand (t, r, p, n, '~', l-1, c+1);
	pComand (t, r, p, n, '~', l+1, c-1);
	pComand (t, r, p, n, '~', l+1, c+1);
}

/**

Coloca água nas diagonais e dependendo da presença de água em algumas direções da coordenada recebida coloca também na direção oposta.

@param t apontador para a estrutura onde é guardado o tabuleiro

@param r apontador para a estrutura com o valor que indica se a função efetuou alterações ou se o tabuleiro é inválido

@param p apontador para a stack principal

@param n índice do comando atual

@param l índice da linha correspondente à posição recebida

@param c índice da coluna correspondente à posição recebida

*/
void E1centro (tabuleiro *t, data *r, undo *p, int n, int l, int c) {
	char cima, baixo, esq, dir;
	cima = t -> tab [l-1][c];
	baixo = t -> tab [l+1][c];
	esq = t -> tab [l][c-1];
	dir = t -> tab [l][c+1];
	E1diagonais (t, r, p, n, l, c);
	if (cima == '~') {
		pComand (t, r, p, n, '~', l+1, c);
	}
	if (baixo == '~') {
		pComand (t, r, p, n, '~', l-1, c);
	}
	if (esq == '~') {
		pComand (t, r, p, n, '~', l, c+1);
	}
	if (dir == '~') {
		pComand (t, r, p, n, '~', l, c-1);
	}
}

/**

Coloca água em todas as coordenadas adjacentes à recebida, exceto na coordenada à direita.

@param t apontador para a estrutura onde é guardado o tabuleiro

@param r apontador para a estrutura com o valor que indica se a função efetuou alterações ou se o tabuleiro é inválido

@param p apontador para a stack principal

@param n índice do comando atual

@param l índice da linha correspondente à posição recebida

@param c índice da coluna correspondente à posição recebida

*/
void E1esquerda (tabuleiro *t, data *r, undo *p, int n, int l, int c) {
	pComand (t, r, p, n, '~', l-1, c);
	pComand (t, r, p, n, '~', l, c-1);
	pComand (t, r, p, n, '~', l+1, c);
	E1diagonais (t, r, p, n, l, c);
}

/**

Coloca água em todas as coordenadas adjacentes à recebida, exceto na coordenada à esquerda.

@param t apontador para a estrutura onde é guardado o tabuleiro

@param r apontador para a estrutura com o valor que indica se a função efetuou alterações ou se o tabuleiro é inválido

@param p apontador para a stack principal

@param n índice do comando atual

@param l índice da linha correspondente à posição recebida

@param c índice da coluna correspondente à posição recebida

*/
void E1direita (tabuleiro *t, data *r, undo *p, int n, int l, int c) {
	pComand (t, r, p, n, '~', l-1, c);
	pComand (t, r, p, n, '~', l, c+1);
	pComand (t, r, p, n, '~', l+1, c);
	E1diagonais (t, r, p, n, l, c);
}

/**

Coloca água em todas as coordenadas adjacentes à recebida, exceto na coordenada abaixo.

@param t apontador para a estrutura onde é guardado o tabuleiro

@param r apontador para a estrutura com o valor que indica se a função efetuou alterações ou se o tabuleiro é inválido

@param p apontador para a stack principal

@param n índice do comando atual

@param l índice da linha correspondente à posição recebida

@param c índice da coluna correspondente à posição recebida

*/
void E1cima (tabuleiro *t, data *r, undo *p, int n, int l, int c){
	pComand (t, r, p, n, '~', l-1, c);
	pComand (t, r, p, n, '~', l, c-1);
	pComand (t, r, p, n, '~', l, c+1);
	E1diagonais (t, r, p, n, l, c);
}

/**

Coloca água em todas as coordenadas adjacentes à recebida, exceto na coordenada acima.

@param t apontador para a estrutura onde é guardado o tabuleiro

@param r apontador para a estrutura com o valor que indica se a função efetuou alterações ou se o tabuleiro é inválido

@param p apontador para a stack principal

@param n índice do comando atual

@param l índice da linha correspondente à posição recebida

@param c índice da coluna correspondente à posição recebida

*/
void E1baixo (tabuleiro *t, data *r, undo *p, int n, int l, int c){
	pComand (t, r, p, n, '~', l, c-1);
	pComand (t, r, p, n, '~', l, c+1);
	pComand (t, r, p, n, '~', l+1, c);
	E1diagonais (t, r, p, n, l, c);
}

/**

Coloca água em todas as coordenadas adjacentes à recebida.

@param t apontador para a estrutura onde é guardado o tabuleiro

@param r apontador para a estrutura com o valor que indica se a função efetuou alterações ou se o tabuleiro é inválido

@param p apontador para a stack principal

@param n índice do comando atual

@param l índice da linha correspondente à posição recebida

@param c índice da coluna correspondente à posição recebida

*/
void E1todas (tabuleiro *t, data *r, undo *p, int n, int l, int c) {
	pComand (t, r, p, n, '~', l-1, c);
	E1baixo (t, r, p, n, l, c);
}

/**

Determina se um dado caractere é um segmento de barco determinado.

@param c caractere a analisar

@returns 1 se o caractere é um segmento de barco determinado, 0 caso contrário

*/
int barco (char c){
	int r = 0;
	if (c == '<' || c == '>' || c == '^' || c == 'v' || c == '#' || c == 'O') r = 1;
	return r;
}

/**
Executa a ação correspondente ao comando E2.

Percorre todo o tabuleiro, contando o número de segmentos de barcos existentes em cada linha/coluna e caso essa linha/coluna já tenha o número máximo de segmentos, substitui as coordenadas não determinadas dessa linha/coluna por água.
É ainda verificado se o número de caracteres contados em cada linha/coluna é superior ao número de segmento que deverá haver nessa linha/coluna, para efeitos de validação do tabuleiro. 

@param t apontador para a estrutura onde é guardado o tabuleiro

@param r apontador para a estrutura com o valor que indica se a função efetuou alterações ou se o tabuleiro é inválido

@param p apontador para a stack principal

@param n índice do comando atual

*/
void E2Comand (tabuleiro *t, data *r, undo *p, int n){
	int l, c, ch;
	for (l = 1; l <= (t -> nlins); l++){
		ch = 0;
		for (c = 1; c <= (t -> ncols); c++){
			if ((t -> tab [l][c] != '.') && (t -> tab [l][c] != '~')) ch++;
		}
		if (ch == (t -> nseg_lins [l-1])) hComand (t, r, p, n, l);
		if (ch > (t -> nseg_lins [l-1])) r -> val = 1;
	}
	for (c = 1; c <= (t -> ncols); c++){
		ch = 0;
		for (l = 1; l <= (t -> nlins); l++){
			if ((t -> tab [l][c] != '.') && (t -> tab [l][c] != '~')) ch++;
		}
		if (ch == (t -> nseg_cols [c-1])) vComand (t, r, p, n, c);
		if (ch > (t -> nseg_cols [c-1])) r -> val = 1;
	}
}

/**
Executa a ação correspondente ao comando E3.

Percorre todo o tabuleiro, contando o número de segmentos de barcos e coordenadas não determinadas existentes em cada linha/coluna e caso esse número corresponda ao número de segmentos que deverão existir nessa linha/coluna, substitui as coordenadas não determinadas por 'o'.
Por fim, o tabuleiro volta a ser percorrido, para encontrar 'o' e tentar descobrir a que segmento corresponde.
É ainda verificado se o número de caracteres contados em cada linha/coluna é inferior ao número de segmento que deverá haver nessa linha/coluna, para efeitos de validação do tabuleiro.

@param t apontador para a estrutura onde é guardado o tabuleiro

@param r apontador para a estrutura com o valor que indica se a função efetuou alterações ou se o tabuleiro é inválido

@param p apontador para a stack principal

@param n índice do comando atual

*/
void E3Comand (tabuleiro *t, data *r, undo *p, int n){
	int l, c, ch;
	for (l = 1; l <= (t -> nlins); l++){
		ch = 0;
		for (c = 1; c <= (t -> ncols); c++){
			if (t -> tab [l][c] != '~') ch++;
		}
		if (ch == (t -> nseg_lins [l-1])) E3linha (t, r, p, n, l);
		if (ch < (t -> nseg_lins [l-1])) r -> val = 1;
	}
	for (c = 1; c <= (t -> ncols); c++){
		ch = 0;
		for (l = 1; l <= (t -> nlins); l++){
			if (t -> tab [l][c] != '~') ch++;
		}
		if (ch == (t -> nseg_cols [c-1])) E3coluna (t, r, p, n, c);
		if (ch < (t -> nseg_cols [c-1])) r -> val = 1;
	}
	for (l = 1; l <= (t -> nlins); l++){
		for (c = 1; c <= (t -> ncols); c++){
			if (t -> tab [l][c] == 'o') E3ocupados (t, r, p, n, l, c);
		}
	}
}

/**

Preenche com 'o' todas as posições sem valor determinado da linha l do tabuleiro.

@param t apontador para a estrutura onde é guardado o tabuleiro

@param r apontador para a estrutura com o valor que indica se a função efetuou alterações ou se o tabuleiro é inválido

@param p apontador para a stack principal

@param n índice do comando atual

@param l índice da linha com as posições a serem preenchidas

*/
void E3linha (tabuleiro *t, data *r, undo *p, int n, int l){
	int c;
	for (c = 1; c <= (t -> ncols); c++){
		if (t -> tab [l][c] == '.') pComand (t, r, p, n, 'o', l, c);
	}
}

/**

Preenche com 'o' todas as posições sem valor determinado da coluna c do tabuleiro.

@param t apontador para a estrutura onde é guardado o tabuleiro

@param r apontador para a estrutura com o valor que indica se a função efetuou alterações ou se o tabuleiro é inválido

@param p apontador para a stack principal

@param n índice do comando atual

@param c índice da coluna com as posições a serem preenchidas

*/
void E3coluna (tabuleiro *t, data *r, undo *p, int n, int c){
	int l;
	for (l = 1; l <= (t -> nlins); l++){
		if (t -> tab [l][c] == '.') pComand (t, r, p, n, 'o', l, c);
	}
}

/**

Dependendo das coordenadas existentes à volta da coordenada recebida, tenta descobrir a que segmento pertence essa coordenada.

@param t apontador para a estrutura onde é guardado o tabuleiro

@param r apontador para a estrutura com o valor que indica se a função efetuou alterações ou se o tabuleiro é inválido

@param p apontador para a stack principal

@param n índice do comando atual

@param l índice da linha correspondente à posição recebida

@param c índice da coluna correspondente à posição recebida

*/
void E3ocupados (tabuleiro *t, data *r, undo *p, int n, int l, int c){
	char cima, baixo, esq, dir;
	cima = t -> tab [l-1][c];
	baixo = t -> tab [l+1][c];
	esq = t -> tab [l][c-1];
	dir = t -> tab [l][c+1];
	if ((cima == '~') && (baixo == '~') && (esq == '~') && (dir == '~')) pComand (t, r, p, n, 'O', l, c);
	if ((esq == '~') && ((dir == '#') || (dir == 'o') || (dir == '>'))) pComand (t, r, p, n, '<', l, c);
	if ((dir == '~') && ((esq == '#') || (esq == 'o') || (esq == '<'))) pComand (t, r, p, n, '>', l, c);
	if ((((dir == '>') || (dir == '#') || (dir == 'o')) && ((esq == '<') || (esq == '#') || (esq == 'o'))) || (((cima == '^') 
	|| (cima == '#') || (cima == 'o')) && ((baixo == 'v') || (baixo == '#') || (baixo == 'o')))) pComand (t, r, p, n, '#', l, c);
	if ((cima == '~') && ((baixo == '#') || (baixo == 'o') || (baixo == 'v'))) pComand (t, r, p, n, '^', l, c);
	if ((baixo == '~') && ((cima == '#') || (cima == 'o') || (cima == '^'))) pComand (t, r, p, n, 'v', l, c);
}

/**

Executa a ação correspondente ao comando D.
Numa stack não vazia, enquanto o número de comando for igual, é aplicado reposto o estado anterior.
Se o caracter existente na stack for 'c', significa que é necessário repor um tabuleiro completo. Caso contrário, é apenas necessário repor uma coordenada.

@param t apontador para a estrutura onde é guardado o tabuleiro

@param p apontador para a stack principal

@param ut apontador para a stack de tabuleiros

*/
void DComand (tabuleiro *t, undo *p, undotab *ut){
	if ((*p) != NULL){
		int n = (*p) -> num;
		int l, c;
		char ch;
		undo tmp;
		while (((*p) != NULL) && (((*p) -> num) == n)){
			if (((*p) -> caracter) == 'c'){
				undoToTab (t, ut);
			}
			else {
				l = (*p) -> lin;
				c = (*p) -> col;
				ch = (*p) -> caracter;
				t -> tab [l][c] = ch;
			}
			tmp = (*p) -> prox;
			free (*p);
			(*p) = tmp;
		}
	}
}

/**

Copia o tabuleiro existente na stack para a stack de tabuleiros.

@param t apontador para a estrutura onde é guardado o tabuleiro

@param ut apontador para a stack de tabuleiros

*/
void undoToTab (tabuleiro *t, undotab *ut){
	int l, c;
	undotab tmp = (*ut) -> prox;
	t -> nlins = (*ut) -> nlins;
	t -> ncols = (*ut) -> ncols;
	for (l = 0; l < (t -> nlins); l++){
		t -> nseg_lins [l] = (*ut) -> nseg_lins [l];
	}
	for (c = 0; c < (t -> ncols); c++){
		t -> nseg_cols [l] = (*ut) -> nseg_cols [l];
	}
	for (l = 0; l <= (t -> nlins) + 1; l++){
		for (c = 0; c <= (t -> ncols) + 1; c++){
			t -> tab [l][c] = (*ut) -> tab [l][c];
		}
	}
	free (*ut);
	(*ut) = tmp;
}

/**

Executa a ação correspondente ao comando e.

@param t apontador para a estrutura onde é guardado o tabuleiro

@param local localização e nome do ficheiro a ser escrito

*/
void eComand (tabuleiro *t, char *local){
	FILE *f;
	int c, l;
	f = fopen(local, "w");
	if (f == NULL) printf("Não pode ser criado/aberto o ficheiro\n"); /*Não sei se é realmente necessário, mas caso dê erro 
	já se sabe de onde vem*/
	else {
		fprintf (f, "%d %d\n", t -> nlins, t -> ncols);
		for (l = 0; l < (t -> nlins - 1); l++) fprintf (f, "%d ", t -> nseg_lins [l]);
		fprintf (f, "%d\n", t -> nseg_lins [l]);
		for (c = 0; c < (t -> ncols - 1); c++) fprintf (f,"%d ", t -> nseg_cols [c]);
		fprintf (f, "%d\n", t -> nseg_cols [c]);
		for (l = 1; l <= (t -> nlins); l++){
			for (c = 1; c < (t -> ncols); c++){
				fprintf (f,"%c", t -> tab [l][c]);
			}
			fprintf (f,"%c\n", t -> tab [l][c]);
		}
		fclose(f);
	}
}

/**

Executa a ação correspondente ao comando l.

@param t apontador para a estrutura onde é guardado o tabuleiro

@param local localização e nome do ficheiro a ser lido

@returns 0 se o ficheiro pôde ser aberto, -1 caso contrário

*/
int lComand (tabuleiro *t, char *local){
	FILE *f;
	int l, c;
	char linha[MAXBUF];
	char resto [MAXBUF];
	f = fopen(local, "r");
	if (f == NULL) printf("O ficheiro não pôde ser aberto\n");
	else {
		if (fgets (linha, MAXBUF, f) == NULL)
			return -1;
		sscanf(linha, "%d %d", &(t -> nlins), &(t -> ncols));
		if (fgets (linha, MAXBUF, f) == NULL)
			return -1;
		for (l = 0; l < (t -> nlins); l++) {
			sscanf(linha, "%d %[^\n]", &(t -> nseg_lins[l]), resto);
			strcpy(linha, resto);
		}
		if (fgets (linha, MAXBUF, f) == NULL)
			return -1;
		for (l = 0; l < (t -> ncols); l++){
			sscanf (linha, "%d %[^\n]", &(t -> nseg_cols[l]), resto);
			strcpy (linha, resto);
		}
		for (c = 0; c <= (t -> ncols + 1); c++){
			t -> tab [0][c] = '~';
			t -> tab [(t -> nlins) + 1][c] = '~';
		} 
		for (l = 0; l <= (t -> ncols + 1); l++){
			t -> tab [l][0] = '~';
			t -> tab [l][(t -> ncols) + 1] = '~';
		}
		for (l = 1; l <= (t -> nlins); l++){
			if (fgets (linha, MAXBUF, f) == NULL)
				return -1;
			for (c = 1; c <= (t -> ncols); c++){
				sscanf (linha, "%c%[^\n]", &(t -> tab[l][c]), resto);
				strcpy (linha, resto);
			}
		}
		fclose(f);
	}
	return 0;
}