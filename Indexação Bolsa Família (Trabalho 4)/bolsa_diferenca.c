#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ArvoreB.h"
 

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

void diferenca(ArvoreB* a, FILE *f){
    FILE *saida = fopen("diferenca_bolsa.csv", "w");
    char linha[2048];
	char nis[15];
	int coluna = 0;
	//long posicao;
	char* campo;
    char chave[TAM_CHAVE+1];
    //posicao=0;
    long posArq;
    fgets(linha, 2048, f);
	
	fgets(linha, 2048, f);
    
        while(!feof(f))
        {
            coluna = 0;
            char linhaArq[2048];
		    strcpy(linhaArq, linha);
            campo = strtok(linha,";");
            while(campo)
            {
                if(coluna == 5)
                {
                    strcpy(nis, campo);
                    posArq = ArvoreB_Busca(a,nis);
                    if(posArq<0){
                        fputs(linhaArq, saida);
                    }
                }
                coluna++;
                campo = strtok(NULL,";");
            }
            //posicao = ftell(f);
            fgets(linha, 2048, f);

        }
    fclose(saida);
     
}
int main(int argc, char **argv)
{
    
    if(argc != 3)
    {
        fprintf(stderr, "Erro na chamada do comando.\n");
        fprintf(stderr, "Uso: %s [ARQUIVO DE ÍNDICE] [ARQUIVO BOLSA FAMILIA]", argv[0]);
        return 1;
    }   
    ArvoreB* a = ArvoreB_Abre(argv[1]);
    FILE *f1;
    f1 = fopen(argv[2], "r");
    if(!f1) {
        fprintf(stderr,"Arquivo nao pode ser aberto para leitura\n");
        return 1;
    }
    diferenca(a, f1); 
    ArvoreB_Fecha(a);
    fclose(f1);
   
    return 0;
}

