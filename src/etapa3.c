#include "estrutura.h"
#include "stack.h"
#include "etapa1.h"
#include "etapa2.h"
#include "etapa3.h"
#include <stdio.h>

/**

Executa a ação correspondente ao comando R.

É iniciado um ciclo em que este só termina quando o tabuleiro estiver resolvido ou quando o tabuleiro não for válido ou quando não se verificarem alterações no tabuleiro.
É iniciado um novo ciclo, o qual só termina quando aplicando as estratégias presentes neste ciclo, não efetuem alterações ou o tabuleiro não seja válido. São aplicadas as estratégias básicas.
Quando, aplicando as estratégias básicas, o tabuleiro não sofre alterações, caso o tabuleiro seja válido, é iniciada uma estratégia avançada.

@param t apontador para a estrutura onde é guardado o tabuleiro

@param r apontador para a estrutura com o valor que indica se a função efetuou alterações ou se o tabuleiro é inválido

@param p apontador para a stack principal

@param ut apontador para a stack de tabuleiros

@param n índice do comando atual

*/
void RComand (tabuleiro *t, data *r, undo *p, undotab *ut, int n){
	r -> alt = 1;
	(*p) = insere (p, n, 0, 0, '~');
	while (resolvido (t) == 0 && r -> val == 0 && r -> alt == 1){
		while (r -> alt == 1 && r -> val == 0){
			r -> alt = 0;
			EA1 (t, r, p, n);
			E1Comand (t, r, p, n);
			E2Comand (t, r, p, n);
			E3Comand (t, r, p, n);
		}
		if (r -> val == 0){
			descobrir (t, r, p, ut, n+1);
		}
		else {
			printf ("O tabuleiro não é válido\n");
		}
	}
}

/**

É percorrido todo o tabuleiro procurando segmentos de barcos, nos quais se tem ou pode ter a certeza que terá de haver um segmento numa coordenada adjacente a este segmento.

@param t apontador para a estrutura onde é guardado o tabuleiro

@param r apontador para a estrutura com o valor que indica se a função efetuou alterações ou se o tabuleiro é inválido

@param p apontador para a stack principal

@param n índice do comando atual

*/
void EA1 (tabuleiro *t, data *r, undo *p, int n){
	int l, c;
	for (l = 1; l <= (t-> nlins); l++){
		for (c = 1; c <= (t-> ncols); c++){
			if (t -> tab [l][c] == '#'){
				EA1centro (t, r, p, n, l, c);
			}
			if (t -> tab [l][c] == '^'){
				pComand (t, r, p, n, 'o', l+1, c);
			}
			if (t -> tab [l][c] == 'v'){
				pComand (t, r, p, n, 'o', l-1, c);
			}
			if (t -> tab [l][c] == '<'){
				pComand (t, r, p, n, 'o', l, c+1);
			}
			if (t -> tab [l][c] == '>'){
				pComand (t, r, p, n, 'o', l, c-1);
			}
		}
	}
}

/**

Nesta função, as coordenadas recebidas pertencem sempre à parte central de um barco. Logo, se o número de segmentos que deverá haver na linha correspondente, for menor ou igual a dois, as coordenadas da direita e da esquerda terão de ser água, pois quando um barco tem uma parte central, este terá um comprimento mínimo 3.
É aplicado o mesmo raciocinio para as colunas.
De seguida, caso exista água em cima ou em baixo, sabemos que o barco terá de ser horizontal, logo serão preenchidos à direita e à esquerda um segmento de barco indeterminada.
É aplicado o mesmo raciocionio caso exita água à esquerda ou à direita.

@param t apontador para a estrutura onde é guardado o tabuleiro

@param r apontador para a estrutura com o valor que indica se a função efetuou alterações ou se o tabuleiro é inválido

@param p apontador para a stack principal

@param n índice do comando atual

@param l índice da linha correspondente à posição recebida

@param c índice da coluna correspondente à posição recebida

*/
void EA1centro (tabuleiro *t, data *r, undo *p, int n, int l, int c){
	char cima, baixo, esq, dir;
	if (t -> nseg_lins [l-1] <= 2){
		pComand (t, r, p, n, '~', l, c-1);
		pComand (t, r, p, n, '~', l, c+1);
	}
	if (t -> nseg_cols [c-1] <= 2){
		pComand (t, r, p, n, '~', l-1, c);
		pComand (t, r, p, n, '~', l+1, c);
	}
	cima = t -> tab [l-1][c];
	baixo = t -> tab [l+1][c];
	esq = t -> tab [l][c-1];
	dir = t -> tab [l][c+1];
	if (cima == '~' || baixo == '~'){
			pComand (t, r, p, n, 'o', l, c-1);
			pComand (t, r, p, n, 'o', l, c+1);
	}
	if (esq == '~' || dir == '~'){
			pComand (t, r, p, n, 'o', l-1, c);
			pComand (t, r, p, n, 'o', l+1, c);
	}
}

/**

É percorrido todo o tabuleiro, de forma a determinar se o tabuleiro já está resolvido, isto é, se já não existem coordenadas e segmentos indeterminados.

@param t apontador para a estrutura onde é guardado o tabuleiro

*/
int resolvido (tabuleiro *t){
	int l, c, r = 1;
	for (l = 1; l <= (t -> nlins); l++){
		for (c = 1; c <= (t -> ncols); c++){
			if (t -> tab [l][c] == '.' || t -> tab [l][c] == 'o') r = 0;
		}
	}
	return r;
}

/**

É precorrido todo o tabuleiro, com o objetivo de encontrar coordenadas não determinadas. Neste caso, é colocada um segmento indeterminado e verifica-se se esse segmento é válido nessas coordenadas. Se não for é colocado água nessa coordenada, pois naquela coordenada nunca poderá haver um segmento.

@param t apontador para a estrutura onde é guardado o tabuleiro

@param r apontador para a estrutura com o valor que indica se a função efetuou alterações ou se o tabuleiro é inválido

@param p apontador para a stack principal

@param ut apontador para a stack de tabuleiros

@param n índice do comando atual

*/
void descobrir (tabuleiro *t, data *r, undo *p, undotab *ut, int n){
	int l, c, a = 0;
	for (l = 1; l <= (t -> nlins); l++){
		for (c = 1; c <= (t -> ncols); c++){
			if (t -> tab[l][c] == '.'){
				pComand (t, r, p, n, 'o', l, c);
				if (VComand (t, r, p, ut, n) == 1){
					a = 1;
					pComand (t, r, p, n-1, '~', l, c);
				}
				r -> val = 0;
			}
		}
	}
	r -> alt = a;
}