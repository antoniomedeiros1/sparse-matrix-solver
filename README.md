# Introdução
Sistemas lineares surgem naturalmente de diversos problemas das mais diversas áreas.
Este trabalho visa explorar o método dos Gradientes Conjugados, analisando a convergência
deste para a solução de um sistema linear.

O sistema linear considerado se baseia no resultado da multiplicação de uma matriz simétrica por um vetor
unitário. Através de uma solução inicial, deseja-se observar a quantidade de iterações necessárias
para o vetor solução convergir para o vetor unitário.

As instâncias de matrizes simétricas utilizadas foram encontradas no site SuiteSparse Matrix Collection^1, no
formato Matrix Market. Para fazer a leitura das mesmas utilizou-se auxílio da biblioteca mmio. 

Devido a propriedade de esparsidade das matrizes, foi implementada uma uma estrutura Compressed Sparse Row (CSR)
para armazenar as mesmas e facilitar as operações de multiplicação matriz por vetor.

# Instruções
Compilação por meio do Makefile utilizando o comando `make`.

Para executar o programa deve ser indicado o caminho para o arquivo da matriz, seguido do máximo de iterações e da tolernância. Exemplo:
``` 
./main instances/1138_bus.mtx 1000 0.001
```

# Resultados
Foram implementados os métodos dos Gradientes otimizado e o método de Gradientes Conjugados com 
e sem precondicionamento, observando para cada instância o resíduo final após uma certa quantidade
de iterações.

A tabela a seguir apresenta a soma do vetor solução de cada método para cada instância, 
que em teoria deve convergir para a dimensão da matriz, considerando um máximo de 100 iterações,

Instância | Esperado | Gradiente Otimizado | Gradiente Conjugado | G.C com Precondicionamento
--- | --- | --- | --- | ---
bcsstk01 | 48 | 36.4652 | 46.7096 | 42.1113
1138_bus | 1138 | 755.348 | 982.65 | 626.621

Para verificar a rapidez na convergência para os casos sem e com precondicionador, foi executado a instância
1138_bus para diferentes valores de kmax. Os resultados podem ser observados no gráfico:

![grafico](https://drive.google.com/uc?export=view&id=1wyFfGy9K3zWeOFsBiCqR1Ya-JL-8Qmdl)


# Referências
https://sparse.tamu.edu/

https://math.nist.gov/MatrixMarket/mmio-c.html
