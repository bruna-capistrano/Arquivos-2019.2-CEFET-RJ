#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ArvoreB.h"

int acessos;

typedef struct _Endereco Endereco;
 
struct _Endereco
{
    char logradouro[72];
    char bairro[72];
    char cidade[72];
    char uf[72];
    char sigla[2];
    char cep[8];
    char lixo[2];
};

ArvoreB_Pagina* ArvoreB_alocaPagina()
{
    int i;
    ArvoreB_Pagina* resp = (ArvoreB_Pagina*) malloc(sizeof(ArvoreB_Pagina));
    resp->quantidadeElementos = 0;
    resp->paginaEsquerda = 0;
    for(i=0; i<TAM_PAG; i++)
    {
        memset(resp->elementos[i].chave,'\0',TAM_CHAVE);
        resp->elementos[i].paginaDireita = 0;
        resp->elementos[i].posicaoRegistro = 0;
    }
    return resp;
}

void ArvoreB_desalocaPagina(ArvoreB_Pagina* p)
{
    if(p)
    {
        free(p);
    }
}
ArvoreB* ArvoreB_Abre(const char* nomeArquivo)
{
    ArvoreB* resp = (ArvoreB*) 0;
    ArvoreB_Cabecalho* cabecalho = (ArvoreB_Cabecalho*) 0;
    ArvoreB_Pagina* raiz = (ArvoreB_Pagina*) 0;
    FILE* f = fopen(nomeArquivo, "r");
    if(!f)
    {
        f = fopen(nomeArquivo,"w");
        if(!f)
        {
            fprintf(stderr,"Arquivo %s não pode ser criado\n", nomeArquivo);
            return resp;
        }
        cabecalho = (ArvoreB_Cabecalho*) malloc(sizeof(ArvoreB_Cabecalho));
        cabecalho->raiz = sizeof(ArvoreB_Cabecalho);
        fwrite(cabecalho,sizeof(ArvoreB_Cabecalho),1,f);
        raiz = ArvoreB_alocaPagina();
        fwrite(raiz,sizeof(ArvoreB_Pagina),1,f);
        ArvoreB_desalocaPagina(raiz);
        free(cabecalho);
    }
    fclose(f);
    f = fopen(nomeArquivo, "rb+");
    resp = (ArvoreB*) malloc(sizeof(ArvoreB));
    resp->f = f;
    resp->cabecalho = (ArvoreB_Cabecalho*) malloc(sizeof(ArvoreB_Cabecalho));
    fread(resp->cabecalho,sizeof(ArvoreB_Cabecalho),1,f);
    return resp;
}

int ArvoreB_Compara(const void *e1, const void* e2)
{
    return strncmp(((ArvoreB_Elemento*)e1)->chave,((ArvoreB_Elemento*)e2)->chave,TAM_CHAVE); 
}

void ArvoreB_escreveCabecalho(ArvoreB* arvore)
{
    fseek(arvore->f, 0, SEEK_SET);
    fwrite(arvore->cabecalho, sizeof(ArvoreB_Cabecalho), 1, arvore->f);
}

void ArvoreB_Fecha(ArvoreB* arvore)
{
    if(arvore)
    {
        ArvoreB_escreveCabecalho(arvore);
        fclose(arvore->f);
        free(arvore->cabecalho);
        free(arvore);
    }
}

long ArvoreB_Busca_Recursiva(ArvoreB* arvore, long posicaoPagina, char chave[TAM_CHAVE])
{
    if(posicaoPagina == 0)
    {
        return -1;
    }
    ArvoreB_Pagina *pagina = ArvoreB_alocaPagina();
    fseek(arvore->f, posicaoPagina, SEEK_SET);
    fread(pagina, sizeof(ArvoreB_Pagina),1,arvore->f);
    acessos++;
    long posicaoPaginaFilho = pagina->paginaEsquerda;
    for(int i=0; i<pagina->quantidadeElementos; i++)
    {
        
        if(strncmp(chave,pagina->elementos[i].chave,TAM_CHAVE)==0)
        {
            // Achei
            
            long resp = pagina->elementos[i].posicaoRegistro;
            ArvoreB_desalocaPagina(pagina);
            return resp;
           
        }
        if(strncmp(chave,pagina->elementos[i].chave,TAM_CHAVE)<0)
        {
            break;
        }
        posicaoPaginaFilho = pagina->elementos[i].paginaDireita;
    }
    ArvoreB_desalocaPagina(pagina);
    return ArvoreB_Busca_Recursiva(arvore, posicaoPaginaFilho, chave);
}

long ArvoreB_Busca(ArvoreB* arvore, char chave[TAM_CHAVE])
{
    return ArvoreB_Busca_Recursiva(arvore, arvore->cabecalho->raiz, chave);
}
int main(int argc, char** argv)
{
    
    char chave[TAM_CHAVE+1];
    long posicao;
    ArvoreB* a = ArvoreB_Abre("arvore.dat");
    FILE *f1;
    if(argc != 2) 
    {
        fprintf(stderr, "USO: %s [CEP]", argv[0]);
        return 1;
    }
    char nomeArq[] = "cep.dat";
    f1 = fopen(nomeArq, "r");
    if(!f1) {
        fprintf(stderr,"Arquivo %s nao pode ser aberto para leitura\n", nomeArq);
        return 1;
    }
    Endereco *e = (Endereco*)malloc(sizeof(Endereco));
    posicao = ArvoreB_Busca(a,argv[1]);
    if(posicao>-1){
        fseek(f1, posicao, SEEK_SET);
        if(fread(e,sizeof(Endereco),1,f1) == 1){
            printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n",e->logradouro,e->bairro,e->cidade,e->uf,e->sigla,e->cep);
            printf("%ld\n",posicao);
            printf("Quantidade de acessos: %d\n", acessos);
        }
    }
    else{
        printf("CEP %s nao encontrado.\n", argv[1]);
    }
    
    
    ArvoreB_Fecha(a);
    free(e);
    fclose(f1);
    return 0;
}

