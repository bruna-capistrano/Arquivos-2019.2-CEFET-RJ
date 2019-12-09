# Arquivos-2019.2-CEFET-RJ
 Códigos da Disciplina Organização de Estruturas de Arquivos

# Trabalho 1:
Implementar busca binária para realizar busca de endereço por CEP no arquivo "cep_ordenado_novo.dat
Arquivo: buscabinaria_cep.c

# External MergeSort (Trabalho2):
Implementar o external mergesort para o arquivo "cep.dat". Dividir o arquivo em 8 partes, ordená-las e intercalá-las.
Arquivo: separaeordena.c faz a partição e ordenação dos arquivos.
Arquivo: intercalaCEPs.c faz a intercalação dois a dois das partições geradas pelo código acima.
Para execução, o arquivo "cep.dat" deve estar na mesma pasta que os arquivos do código-fonte.

# Trabalho 3 - Indexar arquivo do cep na árvore B:
Fazer a indexação do arquivo dos CEPs utizando a Árvore B.

# Indexação Bolsa Família (Trabalho 4):
Indexação com árvore B de um arquivo do bolsa família, realização de uma operação de conjuntos (diferença) entre as informações de pagamento de dois meses seguidos e relatório reportando o desempenho das operações. Serão utilizados os arquivos referentes aos meses de março e abril de 2019 para realizar a operação de diferença. 
Os arquivos de pagamentos do bolsa família podem ser baixados neste link: http://www.portaltransparencia.gov.br/download-de-dados/bolsa-familia-pagamentos.
Para executar o programa btree_bolsa.c: passar o nome do arquivo do bolsa família a ser indexado por comando de linha.
Para executar o programas de diferença entre arquivos (bolsa_diferenca.c): passar o nome do arquivo de índice e do arquivo de dados do bolsa família do mês seguinte, nesta ordem, por comando de linha. 


*Programas compilados usando o compilador GCC no Windows 10.
