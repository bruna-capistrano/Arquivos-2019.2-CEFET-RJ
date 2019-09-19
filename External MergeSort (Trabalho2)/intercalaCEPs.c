#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

void intercala(char *a, char *b, char *c)//intercala arquivos ordenados 2 a 2
{
    
    Endereco ea, eb;
    FILE *f1, *f2, *saida;
    
    f1= fopen(a,"r");//abrindo a e b para leitura 
        
    f2 = fopen(b,"r");
        
    saida = fopen(c, "w"); //abrindo arquivo de saída para escrita
    printf("Abri saida\n");
   
    fread(&ea, sizeof(Endereco), 1, f1);//lendo f1 e f2 para ea e eb
	fread(&eb, sizeof(Endereco), 1, f2);
    
	while(!feof(f1) && !feof(f2))
	{
		if(compara(&ea,&eb)<0) //comparando os endereços ea e eb
		{//ea é menor do que eb
			fwrite(&ea,sizeof(Endereco),1,saida);//então, na saída eu escrevo o ea 
			fread(&ea, sizeof(Endereco), 1, f1);//e leio outro ea do arquivo de entrada
		}
		else //se eb for menor que ea
		{
			fwrite(&eb,sizeof(Endereco),1,saida);//escrevo eb na saída
			fread(&eb,sizeof(Endereco),1,f2);//leio outro eb do arquivo de entrada
		}
    }
     while(!feof(f1))
    {
        fwrite(&ea,sizeof(Endereco),1,saida);
        fread(&ea,sizeof(Endereco),1,f1);        
    }
    while(!feof(f2))
    {
        fwrite(&eb,sizeof(Endereco),1,saida);
        fread(&eb,sizeof(Endereco),1,f2);        
    }
    fclose(f1);
    fclose(f2);
    fclose(saida);
}

int main(int argc, char**argv)
{
    
    int i;
    int j = 8;
    
    //intercalando 8 arquivos de cep 2 a 2
    for(int i = 0; i<=6; i+=2){
        char fileName[20];
        char file_name[20];
        snprintf(fileName, sizeof(fileName),"cep%d.dat", i);
        printf("%s\n\n", fileName);
        snprintf(file_name, sizeof(file_name), "cep%d.dat", i+1);
        char outputFile[40];
        snprintf(outputFile, sizeof(outputFile), "cep%d.dat", j);
        
        intercala(fileName, file_name, outputFile);
        j++;

    }
    //intercalando os 4 arquivos de cep resultantes do 1ª fase de intercalação 
    int m = 12;
    for(int k = 8; k<=10; k+=2){
        char fileName[20];
        char file_name[20];
        snprintf(fileName, sizeof(fileName),"cep%d.dat", k);
        printf("%s\n\n", fileName);
        snprintf(file_name, sizeof(file_name), "cep%d.dat", k+1);
        char outputFile[20];
        snprintf(outputFile, sizeof(outputFile), "cep%d.dat", m);
        
        intercala(fileName, file_name, outputFile);
        m++; 
    }
    //intercalando os 2 arquivos resultantes da 2ª fase de intercação
    for(int cont=12; cont<13; cont++){
        char filesName[20];
        char file_Name[20];
        char finalOutput[] = "cep_ordenado.dat";
        snprintf(filesName, sizeof(filesName), "cep%d.dat", cont);
        snprintf(file_Name, sizeof(file_Name), "cep%d.dat", cont+1);
        intercala(filesName, file_Name, finalOutput);
         
    }
    printf("Intercalacao completa = OK \n");
   
    

    
            
}