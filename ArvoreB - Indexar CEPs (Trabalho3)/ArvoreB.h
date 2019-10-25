#ifndef __ARVORE_B__ //se não estiver definido
#define __ARVORE_B__//defino
//tudo que começa com # é diretiva de pré-processador
#define TAM_CHAVE 8
#define TAM_PAG 300 //arvore com 300 chaves

#include <stdio.h>

typedef struct _ArvoreB ArvoreB;
typedef struct _ArvoreB_Cabecalho ArvoreB_Cabecalho;
typedef struct _ArvoreB_Elemento ArvoreB_Elemento;
typedef struct _ArvoreB_Pagina ArvoreB_Pagina;
//pre def das estruturas que vou usar


struct _ArvoreB_Cabecalho
{
    long raiz;//ponteiro para a raiz. Como vou trabalhar em disco, é a posição absoluta da raiz no arquivo, parametro do fseek
};

struct _ArvoreB
{
    ArvoreB_Cabecalho* cabecalho;//ponteiro para o cabeçalho
    FILE* f;    //ponteiro para o arquivo onde a árvore está armazanada
};

struct _ArvoreB_Elemento
{
    char chave[TAM_CHAVE];//um elemento tem uma chave e o ponteiro para o arquivo de dados
    long posicaoRegistro;//posicao do registro no arquivo de dados
    long paginaDireita;//posição da página da direita, então toda chave conhece a página da direita
};

struct _ArvoreB_Pagina
{
    int quantidadeElementos;//qtd de elem. armazenados na pagina
    long paginaEsquerda;//ponteiro da esquerda da página
    ArvoreB_Elemento elementos[TAM_PAG];//vetor que armazena elementos da página, tem 300 elementos
};

#endif
