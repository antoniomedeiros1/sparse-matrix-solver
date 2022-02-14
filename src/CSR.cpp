#include "../include/CSR.hpp"

CSR::CSR(){
  // ctor
}

/**
 * @brief set CSR matrix values and initialize vectors
 *
 * @param m CSR matrix to be initialized
 * @param nnz the number of non zero values of the matrix
 * @param n number of rows and columns of the matrix
 *
 */
CSR::CSR(CSR * m, int nnz, int n){
  this->n = n;
  this->nnz = nnz;
  this->AA = new int[nnz];
  this->JA = new int[nnz];
  this->IA = new int[n + 1];
}

CSR::~CSR(){
  // dtor
  delete [] AA;
  delete [] JA;
  delete [] IA;
}

/**
 * @brief reads a Matrix Market file and stores in the CSR structure
 *
 * @param m CSR matrix that will store the values
 * @param filename path to the MM file
 *
 */
void CSR::readMatrixMarket(std::string filename){
  std::fstream file;
  file.open(filename);
  if (file.is_open()){
    std::string line;
    getline(file, line);
    std::cout << &line[0] << "\n";
    /* while (&line[0] == "%"){ */

    /* } */
  }
  else {
    std::cout << "Falha: arquivo não encontrado \n";
  }
}

/**
 * @brief set CSR matrix values and initialize vectors
 *
 * @param m matrix of the multiplication
 * @param v vector of real numbers
 *
 * @returns the resultant vector
 */
void CSR::MatrixVectorCSR(float * v){
  
}

