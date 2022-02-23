#include "../include/CSR.hpp"
#include <cstdlib>
#include <string>

CSR::CSR() {
  // ctor
}

CSR::~CSR() {
  // dtor
  delete[] AA;
  delete[] JA;
  delete[] IA;
}

/**
 * @brief set CSR matrix values and initialize vectors
 *
 * @param m CSR matrix to be initialized
 * @param nnz the number of non zero values of the matrix
 * @param n number of rows and columns of the matrix
 *
 */
void CSR::initialize(int n, int nnz) {
  this->n = n;
  this->nnz = nnz;
  this->AA = new double[nnz];
  this->JA = new int[nnz];
  this->IA = new int[n + 1];
}

int CSR::getSize() { return this->n; }

/**
 * @brief reads a Matrix Market file and stores in the CSR structure
 *
 * @param m CSR matrix that will store the values
 * @param filename path to the MM file
 *
 */
void CSR::read(const char *file) {

  int ret_code;
  MM_typecode matcode;
  FILE *f;
  int M, N, nz;
  int i, *I, *J;
  double *val;

  if ((f = fopen(file, "r")) == NULL) {
    return;
  }

  if (mm_read_banner(f, &matcode) != 0) {
    printf("Could not process Matrix Market banner.\n");
    exit(1);
  }

  /*  This is how one can screen matrix types if their application */
  /*  only supports a subset of the Matrix Market data types.      */

  if (mm_is_complex(matcode) && mm_is_matrix(matcode)) {
    printf("Sorry, this application does not support ");
    printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
    exit(1);
  }

  printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));

  /* find out size of sparse matrix .... */

  if ((ret_code = mm_read_mtx_crd_size(f, &M, &N, &nz)) != 0)
    exit(1);

  /* reseve memory for matrices */

  I = (int *)malloc(nz * sizeof(int));
  J = (int *)malloc(nz * sizeof(int));
  val = (double *)malloc(nz * sizeof(double));

  /* NOTE: when reading in doubles, ANSI C requires the use of the "l"  */
  /*   specifier as in "%lg", "%lf", "%le", otherwise errors will occur */
  /*  (ANSI C X3.159-1989, Sec. 4.9.6.2, p. 136 lines 13-15)            */

  this->nnz = nz;
  this->n = M;
  this->IA = new int[M + 1];
  int *tmp = new int[M];
  for (int i = 0; i < M; i++) {
    tmp[i] = 0;
  }

  if (mm_is_symmetric(matcode)) {
    for (i = 0; i < nz; i++) {
      //        row col val
      fscanf(f, "%d %d %lg\n", &I[i], &J[i], &val[i]);
      I[i]--; /* adjust from 1-based to 0-based */
      J[i]--;
      tmp[J[i]]++; // increases counter of non zeros on row I[i]
    }
  } else {
    for (i = 0; i < nz; i++) {
      //        row col val
      fscanf(f, "%d %d %lg\n", &I[i], &J[i], &val[i]);
      I[i]--; /* adjust from 1-based to 0-based */
      J[i]--;
      tmp[I[i]]++; // increases counter of non zeros on row I[i]
    }
  }

  if (f != stdin)
    fclose(f);

  /************************/
  /* now write out matrix */
  /************************/

  // mm_write_banner(stdout, matcode);
  // mm_write_mtx_crd_size(stdout, M, N, nz);
  // for (i = 0; i < nz; i++)
  //   fprintf(stdout, "%d %d %20.19g\n", I[i] + 1, J[i] + 1, val[i]);

  this->JA = J;
  this->AA = val;

  this->IA[0] = 0;
  for (int i = 1; i <= M; i++) {
    this->IA[i] = this->IA[i - 1] + tmp[i - 1];
    // std::cout << this->IA[i] << " ";
  }
  // std::cout << "\n";

  for (int i = 0; i <= nz; i++) {
    // std::cout << this->AA[i] << " ";
  }

  free(I);
  // free(J);
  // free(val);
  // delete[] tmp;
}

/**
 * @brief set CSR matrix values and initialize vectors
 *
 * @param m matrix of the multiplication
 * @param v vector of real numbers
 *
 * @returns the resultant vector
 */
void CSR::MatrixVectorCSR(float *x, float *b) {

  int jstart, jend, j;

  for (int i = 0; i < this->n; i++) {
    jstart = IA[i];
    // std::cout << jstart << "\n";
    jend = IA[i + 1] - 1;
    b[i] = 0.0;
    for (int k = jstart; k <= jend; k++) {
      j = JA[k];
      b[i] += AA[k] * x[j];
    }
    std::cout << b[i] << "\n";
  }
}

/**
 * @brief multiplies CSR matrix with a vector of ones
 *
 * @param m matrix of the multiplication
 * @param v vector of real numbers
 *
 * @returns the resultant vector
 */
void CSR::unitary(float *b) {

  float *ones = new float[this->n];

  for (int i = 0; i < n; i++) {
    ones[i] = 1.0;
  }

  this->MatrixVectorCSR(ones, b);
}

std::string repeat(std::string s, int n) {
  std::string repeat;

  for (int i = 0; i < n; i++)
    repeat += s;

  return repeat;
}

void CSR::print() {

  int i, j;

  for (i = 1; i < this->n; i++) {
    for (j = this->IA[i - 1]; j < this->IA[i]; j++) {
      // std::cout << this->AA[j] << " "
      //           << repeat("0 ", this->JA[j + 1] - this->JA[j] - 1);
      std::cout << this->AA[j] << " ";
    }
    // std::cout << repeat("0 ", this->n - j) << "\n";
    std::cout << "\n";
  }
}

/**
 * @brief multiplies CSR matrix with a vector of ones
 *
 * @param m matrix of the multiplication
 * @param v vector of real numbers
 *
 * @returns the resultant vector
 */
void solveSystem() {}
