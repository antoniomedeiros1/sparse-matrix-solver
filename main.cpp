#include <iostream>
#include "include/CSR.hpp"

using namespace std;

int main(int argc, char const *argv[]){

  CSR * m = new CSR();
  string filename(argv[1]);

  m->readMatrixMarket(filename);

  delete m;

  return 0;
}
