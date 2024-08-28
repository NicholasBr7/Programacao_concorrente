Relatório do Segundo Laboratório de Programação Concorrente - 2024.2

Aluno: Nicholas de Araujo Figueira
DRE: 121088218

Os valores encontrados concorrentemente e sequencialmente foram extremamente próximos.  
Mesmo com um altíssimo número de elementos gerados nos vetores, a variância foi mínima, sendo, inclusive, necessário o uso de float para diminuir a precisão e 
possiblitar a melhor visualização da margem de erro. O uso de double gerava uma variância literalmente zero, o que impossibilitava uma análise razoável. 
Não sei se aumentando ainda mais o número de elementos (usei 25000) poderia tornar as discrepâncias ainda maiores e, especialmente, mais óbvias. 
Um outro ponto que poderia ser ponderado é o tempo de execução para verificar se vale a pena o "investimento" na concorrência a partir de que ponto.
