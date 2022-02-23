#include "include/CSR.hpp"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {

  // Parte 1
  float *b;
  CSR *m = new CSR();
  const char *filename = argv[1];

  // reading Matrix Market and storing into CSR object
  m->read(filename);

  // initializes b vector
  b = new float[m->getSize()];

  // multiplication of the CSR matrix and vector of ones
  m->unitary(b);

  // Parte 2
  float epsilon = 0.01;
  int kmax = 100;
  float *x = new float[m->getSize()];

  // método dos gradientes
  cout << "Método dos gradientes:\n";
  m->solvebyGradient(b, x, epsilon, kmax);

  // método dos gradientes conjugado
  cout << "Método dos gradientes conjugado:\n";
  m->solvebyConjGradient(b, x, epsilon, kmax);

  // com pre-condicionamento

  return 0;
}
