#include "include/CSR.hpp"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {

  float *b;
  CSR *m = new CSR();
  const char *filename = argv[1];

  // reading Matrix Market and storing into CSR object
  m->read(filename);
  // m->print();

  // initializes b vector
  b = new float[m->getSize()];

  // multiplication of the CSR matrix and vector of ones
  m->unitary(b);

  // conjugado

  // com pre-condicionamento

  return 0;
}
