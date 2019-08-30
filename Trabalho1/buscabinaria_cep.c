#include <stdio.h>
#include <string.h>

typedef struct _Endereco Endereco;

struct _Endereco {
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2];
};

int main(int argc, char **argv) {
	FILE *f;
	Endereco e;
	long posicao, primeiro, ultimo, meio;

	if(argc != 2) {
		fprintf(stderr, "Erro na chamada de comando.\n");
		fprintf(stderr,"USO: %s [CEP]", argv[0]);
		return 1;
	}

	f = fopen("cep_ordenado_novo.dat","r");
	if(!f) {
		fprintf(stderr,"Arquivo %s nao pode ser aberto.\n", argv[0]);
		return 1;
	}

	fseek(f, 0, SEEK_END);
	posicao = ftell(f);
	primeiro = 0;
	ultimo = (posicao/sizeof(Endereco))-1;
	meio = (primeiro + ultimo)/2;
	rewind(f);
	fseek(f, meio * sizeof(Endereco), SEEK_SET);
	fread(&e, sizeof(Endereco), 1, f);

    if(strncmp(argv[1], e.cep, 8) == 0){
		ultimo = primeiro - 1;
    }

    while(primeiro <= ultimo){
        meio = (primeiro+ultimo)/2;
        fseek(f, meio * sizeof(Endereco), SEEK_SET);
        fread(&e, sizeof(Endereco), 1, f);

        if (strncmp(argv[1], e.cep,8) == 0){
            break;
        }

        if(strncmp(argv[1], e.cep, 8) > 0){
            primeiro = meio + 1;
        } else {
            ultimo = meio - 1;
        }
    }
    if(strncmp(argv[1], e.cep, 8) == 0){
		printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n", e.logradouro, e.bairro, e.cidade, e.uf, e.sigla, e.cep);
    } else {
		printf("CEP nao encontrado.\n");
    }
	
	fclose(f);
	return 0;
}