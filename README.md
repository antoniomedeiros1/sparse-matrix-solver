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

Para executar o programa deve ser indicado o caminho para o arquivo da matriz. Exemplo:
``` 
./main instances/1138_bus.mtx
```

# Resultados
Foram implementados os métodos dos Gradientes otimizado e o método de Gradientes Conjugados, observando,
para cada instância, a soma dos elementos da solução, que deveria convergir para para a dimensão da matriz.

A tabela a seguir apresenta os resultados de cada método para cada instância.

Instância | Esperado | Gradiente Otimizado | Gradiente Conjugado
--- | --- | --- | --- 
bcsstk01 | 48 | 27 | 47
1138_bus | 1138 | 449 | 1106


# Referências
![https://sparse.tamu.edu/]
![https://math.nist.gov/MatrixMarket/mmio-c.html]
