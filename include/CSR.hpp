
#include <iostream>
#include <string>
#include <fstream>

class CSR {

  private:
    int n;          // size of the matrix
    int nnz;        // number of non-zero values
    int * AA;       // non zero values
    int * JA;       // indexes of the column of the element added to AA
    int * IA;       // position on vector where row starts
    bool symmetric; // indicates if the matrix is symmetric

  public:
    CSR();
    CSR(CSR * m, int nnz, int n);
    ~CSR();
    void readMatrixMarket(std::string filename);
    void MatrixVectorCSR(float * v);

};
