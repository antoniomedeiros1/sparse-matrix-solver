#include "include/CSR.hpp"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {

  float *b;
  CSR *m = new CSR();
  const char *filename = argv[1];

  m->read(filename);

  b = new float[m->getSize()];

  m->unitary(b);

  // for (int i = 0; i < m->getSize(); i++) {
  //   cout << b[i] << " ";
  // }
  // cout << "\n";

  return 0;
}
