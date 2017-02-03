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

Com a função pré-definida sscanf, são separados o comando a executar com os argumentos que o acompanham para ser realizada a tarefa correspondente a esse comando.
Nos comandos c e l, antes de as funções serem executadas são adicionados dados à stack, para que mais tarde, caso seja necessário, possa ser executado o comando D.
No comando E1, caso após a estratǵia ter sido efetuada, se chegue à conclusão que o tabuleiro não é válido o tabuleiro é reposto para o estado anterior e é imprimida no ecrã uma mensagem de erro.
No comando R, se o tabuleiro for inválido é executado o comando D. 

@param t apontador para a estrutura onde é guardado o tabuleiro

@param p apontador para a stack principal

@param ut apontador para a stack de tabuleiros

@param linha string a ser utilizada para se guardar o input

@returns 1 se o comando for q, 0 caso contrário.

*/
int interpretador (tabuleiro *t, undo *p, undotab *ut, char *linha){
	int l, c, n, r = 0;
	char a;
	char comando;
	char *local;
	char resto[MAXBUF];
	data d;
	d.val = 0;
	local = (char *)malloc(1024*sizeof(char));
	sscanf (linha, "%c %[^\n]", &comando, resto);
	if ((*p) == NULL) n = 1;
	else n = (*p) -> num + 1;
	if (comando == 'q') r = 1;
	else {
		if (comando == 'c'){
			(*p) = insere (p, n, -1, -1, 'c');
			(*ut) = tabToUndo (t, ut);
			if (cComand (t) == -1) r = 1;
		}
		if (comando == 'm') mComand (t);
		if (comando == 'h'){
			sscanf (resto,"%d",&l);
			hComand (t, &d, p, n, l);
		}
		if (comando == 'v'){
			sscanf (resto,"%d",&c);
			vComand (t, &d, p, n, c);
		}
		if (comando == 'p'){
			sscanf (resto,"%c %d %d",&a, &l, &c);
			pComand (t, &d, p, n, a, l, c);
		}
		if (comando == 'E'){
			sscanf (resto,"%c",&a);
			if (a == '1') {
				E1Comand (t, &d, p, n);
				if (d.val == 1) {
					DComand (t, p, ut);
					printf ("A estratégia não foi aplicada: O tabuleiro não é válido\n");
				}
			}
			if (a == '2') E2Comand (t, &d, p, n);
			if (a == '3') E3Comand (t, &d, p, n);
		}
		if (comando == 'D') DComand (t, p, ut);
		if (comando == 'e'){
			sscanf (resto,"%s",local);
			eComand (t,local);
		}
		if (comando == 'l'){
			sscanf (resto,"%s",local);
			(*p) = insere (p, n, -1, -1, 'c');
			(*ut) = tabToUndo (t, ut);
			if (lComand (t,local) == -1) r = 1;
		}
		if (comando == 'V')	{
			if (VComand (t, &d, p, ut, n) == 1) printf ("NAO\n");
			else printf ("SIM\n");
		}
		if (comando == 'R'){
			RComand (t, &d, p, ut, n);
			if (d.val == 1) DComand (t, p, ut);
		}
	}
	return r;
}

/**

São lidos comandos do standard input, enquanto um desses comandos não retornar o valor 1 (comando q). 

Aí, o ciclo, e consequentemente o programa, termina.

@returns 0 se a função chegar ao fim

*/
int main() {
	tabuleiro t;
	undo p = NULL;
	undotab ut = NULL;
	char linha[MAXBUF];
	int sair = 0;
	while (sair != 1 && fgets (linha, MAXBUF, stdin)){
		sair = interpretador (&t, &p, &ut, linha);
	}
	return 0;
}