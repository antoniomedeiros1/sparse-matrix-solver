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
void CSR::initialize(int n, int nnz){
  this->n = n;
  this->nnz = nnz;
  this->AA = new float[nnz];
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

  int n1, n2, n3;
  float value;
  std::fstream file;
  file.open(filename);

  if (file.is_open()){

    std::string line;

    getline(file, line);
    std::cout << line << "\n";

    // skip header
    bool skip = false;
    while (line.at(0) == '%'){
      // ignore lines that starts with %
      if (line.at(1) == '-'){
        if (skip){
          break;
        }
        skip = true;
      }
      getline(file, line);
    }

    // rows >> columns >> entries
    file >> n1 >> n2 >> n3;
    std::cout << n1 << "\n"; // n
    std::cout << n2 << "\n"; // n
    std::cout << n3 << "\n"; // nnz

    // initialize vectors
    this->initialize(n1, n3);

    // filling AA and JA vectors
    int count = 0;
    while(getline(file, line)){
      // row >> column >> value
      file >> n1 >> n2 >> value;
      this->JA[count] = n2;
      this->AA[count] = value;
      // std::cout << value << " ";
      count++;
    }

    // filling IA vector
    count = 1;
    this->IA[0] = 0;
    for(int i = 1; i < nnz; i++){
      if(JA[i] < JA[i - 1]){
        std::cout << i << " ";
        IA[count] = i;
        count++;
      }
    }
    std::cout << "\n";

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

