#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(1)

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

int compara(const void *e1, const void *e2)
{
	return strncmp(((Endereco*)e1)->cep,((Endereco*)e2)->cep,8);
}

int main(int argc, char**argv)
{
	FILE *f, *saida;
	Endereco *e;
	long posicao, qtd, oitavo;
    
	f = fopen("cep.dat","r");//abrindo o arquivo do CEP para leitura
	fseek(f,0,SEEK_END);//buscando final do arquivo
	
    posicao = ftell(f);//guardando o tamanho em bytes do arquivo
    qtd = posicao/sizeof(Endereco);//obtendo o número de registros no arquivo (699.307)
	oitavo = qtd/8;//obtendo 1/8 do arquivo 
    long resto = qtd%8; //obtendo o resto da partição do arquivo
    rewind(f);//voltando o file pointer para o início do arquivo
    for(int i = 0; i<8; i++){
        if(i==7){
            e = (Endereco*) malloc((oitavo+resto)*sizeof(Endereco));//alocando na memória um espaco que cabe 1/8 do arquivo mais o resto (3 registros)
	        
	        if(fread(e,sizeof(Endereco),oitavo+resto,f) == oitavo+resto)
	        {
		        printf("Particao %d lida = OK\n", i+1);
	        }
    
            qsort(e, (oitavo+resto), sizeof(Endereco), compara);
            char nomeArq[20];
        
            snprintf(nomeArq, sizeof(nomeArq), "cep%d.dat", i);
        
            saida  = fopen(nomeArq,"w");
	        printf("Ordenado %d = OK\n", i+1);
            fwrite(e,sizeof(Endereco), oitavo+resto, saida);
            fclose(saida);
            printf("Escrito %d = OK\n", i+1);
            free(e);
        } 
        else
        {
            e = (Endereco*) malloc(oitavo*sizeof(Endereco));//alocando na memória um espaco que cabe 1/8 do arquivo
	        
        
	        if(fread(e,sizeof(Endereco),oitavo,f) == oitavo)
	        {
		        printf("Particao %d lida = OK\n", i+1);
	        }
    
            qsort(e, oitavo, sizeof(Endereco), compara);
            char nomeArq[20];
        
            snprintf(nomeArq, sizeof(nomeArq), "cep%d.dat", i);
        
            saida  = fopen(nomeArq,"w");
	        printf("Ordenado %d = OK\n", i+1);
            fwrite(e,sizeof(Endereco), oitavo, saida);
            fclose(saida);
            printf("Escrito %d = OK\n", i+1);
            free(e);
        }          
    }
	
    fclose(f);
    
}
