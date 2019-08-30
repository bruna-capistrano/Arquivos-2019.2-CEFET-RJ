#include <stdio.h>
 
int main(int argc, char** argv)
{
    FILE *entrada, *saida;
    int c;
    int qtdbytes = 0;
    int qtdlinhas = 0; 
     
    if(argc != 3)
    {
        fprintf(stderr, "Erro na chamada do comando.\n");
        fprintf(stderr, "Uso: %s [ARQUIVO ORIGEM] [ARQUIVO DESTINO]", argv[0]);
        return 1;
    }
     
    entrada = fopen(argv[1], "r");
    if(!entrada)
    {
        fprintf(stderr, "Arquivo %s nao pode ser lido.\n", argv[1]);
        return 1;
    }
    saida = fopen(argv[2], "w");
    if(!saida)
    {
        fclose(entrada);
        fprintf(stderr, "Arquivo %s nao pode ser aberto.\n", argv[2]);
        return 1;
    }
    c = fgetc(entrada);
    while(c != EOF){
        fputc(c, saida);
        if(c == '\n')
            qtdlinhas++;
        qtdbytes++;
        c = fgetc(entrada);
    }
    printf("O arquivo %s tem %d linhas.\n", argv[1], qtdlinhas);
    printf("O arquivo %s tem %d bytes.\n", argv[1], qtdbytes);
    fclose(entrada);
    fclose(saida);
     
    return 0;
}