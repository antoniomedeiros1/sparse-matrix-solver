
#include "mmio.h"
#include <fstream>
#include <iostream>
#include <string>

class CSR {

private:
  int n;          // number of lines and columns
  int nnz;        // number of non-zero values
  double *AA;     // non zero values
  int *JA;        // indexes of the column of the element added to AA
  int *IA;        // position on vector where row starts
  bool symmetric; // indicates if the matrix is symmetric

public:
  CSR();
  ~CSR();
  void initialize(int n, int nnz);
  int getSize();
  void read(const char *file);
  void print();
  void MatrixVectorCSR(float *x, float *b);
  void unitary(float *b);
  void solva(float *b, float *x);
};
