# Instruções para execução dos códigos da pasta

Arquivo geraMatrizBinario.c servirá para gerar duas matrizes em arquivos binários com dimensões a serem definidas pelo usuário.
Deve-se chamar esse arquivo **duas vezes** para poder gerar as duas matrizes que serão multiplicadas. 
Atente-se para garantir que o número de colunas da matriz 1 seja igual ao número de linhas da matriz 2 (caso contrário os arquivos de multiplicação reclamarão). 
Esse arquivo foi fornecido pela professora.

Modo de execução: chamada do programa, número de linhas, número de colunas, arquivo para armazenar o resultado.
Exemplo: ./geraMatriz 3 4 matriz1.bin

Arquivo multMatrizSeq.c servirá para multiplicar duas matrizes fornecidas de forma sequencial.
Modo de execução: chamada do programa, primeira matriz, segunda matriz e arquivo para armazenar o resultado.
Exemplo: ./multSeq matriz1.bin matriz2.bin resultadoSeq.bin

Arquivo multMatrizConc.c servirá para multiplicar duas matrizes fornecidas de forma concorrente.
Modo de execução: chamada do programa, primeira matriz, segunda matriz, número de threads e arquivo para armazenar o resultado.
Exemplo: ./multConc matriz1.bin matriz2.bin 4 resultadoSeq.bin

Em ambas as multiplicações, serão impressos os tempos de inicialização (alocação de recursos), 
multiplicação (produto matricial em si) e finalização (liberação da memória).

## OPCIONAL 
Arquivo leMatrizBinario.c servirá para ler os valores armazenados nas matrizes.
Esse arquivo foi fornecido pela professora.

Modo de execução: chamada do programa, arquivo a ser lido.
Exemplo: ./leMatriz matriz1.bin


### Relatório
Por fim, o relatório contém todos os valores de tempo lidos bem como cálculo da aceleração, eficiência e análise dos resultados.

