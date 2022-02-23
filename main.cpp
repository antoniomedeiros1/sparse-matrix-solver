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
  int kmax = atoi(argv[2]);
  float epsilon = atof(argv[3]);
  float *x = new float[m->getSize()];

  cout << "\ninstance: " << filename << "\nkmax = " << kmax
       << "; tol = " << epsilon << "\n";

  // método dos gradientes
  cout << "Método dos gradientes:\n";
  m->solvebyGradient(b, x, epsilon, kmax);

  // método dos gradientes conjugado
  cout << "Método dos gradientes conjugado:\n";
  m->solvebyConjGradient(b, x, epsilon, kmax);

  // com pre-condicionamento
  cout << "Método dos gradientes conjugado com pre-condicionamento:\n";
  m->solvebyConjGradientPreCond(b, x, epsilon, kmax);

  return 0;
}
