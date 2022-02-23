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

  // printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));

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
 * @brief calculates b from A * x
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
    // std::cout << b[i] << "\n";
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

float norm(float *vec, int n) {
  float big = vec[0];
  for (int i = 1; i < n; i++) {
    if (vec[i] > big) {
      big = vec[i];
    }
  }
  return big;
}

float mult(float *vec1, float *vec2, int n) {
  float sum = 0;
  for (int i = 0; i < n; i++) {
    sum += vec1[i] * vec2[i];
  }
  // std::cout << sum << std::endl;
  return sum;
}

float sum(float *vec, int n) {
  float sum = 0;
  for (int i = 0; i < n; i++) {
    sum += vec[i];
  }
  return sum;
}

/**
 * @brief finds the solution of the linear system Ax = b where A is the CSR
 * matrix itself using the optimized gradient algorithm
 *
 * @param b         vector of real numbers
 * @param x         vector with the same size as b representing the solution
 * @param epsilon   tolerance of the approximation
 * @param kmax      maximum number of iterations
 *
 */
void CSR::solvebyGradient(float *b, float *x, float epsilon, int kmax) {

  int k;                   // iterations counter
  float *r = new float[n]; // vector to store the residue | r = b - Ax
  float *v = new float[n]; // vector used for calculating alpha
  float alpha;             // paremeter to find next iteration

  // initializes the first solution
  for (int i = 0; i < n; i++) {
    x[i] = 0.5;
  }

  // calculates residue
  float *Ax = new float[n];
  this->MatrixVectorCSR(x, Ax);
  for (int i = 0; i < n; i++) {
    r[i] = b[i] - Ax[i];
  }

  for (k = 0; k < kmax; k++) {

    // calculating alpha
    this->MatrixVectorCSR(r, v);
    alpha = mult(r, r, n) / mult(v, r, n);

    // calculates next iteration of x
    for (int i = 0; i < n; i++) {
      x[i] += alpha * r[i];
    }

    // std::cout << "k:" << k << "\n";
    // std::cout << "Solution norm:" << norm(x, n) << "\n";
    // std::cout << "alpha:" << alpha << "\n";
    // std::cout << "res:" << norm(r, n) << "\n\n";

    // verifies if tolerance is satisfied
    if (norm(r, n) < epsilon) {
      break;
    }

    // calculates new residue
    for (int i = 0; i < n; i++) {
      r[i] -= alpha * v[i];
    }
  }

  // prints out the norm of the solution and the number of iterations
  // for (int i = 0; i < n; i++) {
  //   std::cout << x[i] << " ";
  // }
  // std::cout << "\n";
  std::cout << "k: " << k << "\n";
  std::cout << "Solution sum: " << sum(x, n) << "\n\n";
}

/**
 * @brief finds the solution of the linear system Ax = b where A is the CSR
 * matrix itself using the conjugate gradient method
 *
 * @param b         vector of real numbers
 * @param x         vector with the same size as b representing the solution
 * @param epsilon   tolerance of the approximation
 * @param kmax      maximum number of iterations
 *
 */
void CSR::solvebyConjGradient(float *b, float *x, float epsilon, int kmax) {

  int k;                   // iterations counter
  float *r = new float[n]; // vector to store the residue | r = b - Ax
  float *p = new float[n]; // vector
  float rsold, rsnew, e, alpha, beta;
  float *v = new float[n]; // vector used for calculating alpha

  // initializes the first solution
  for (int i = 0; i < n; i++) {
    x[i] = 0.5;
  }

  // calculates residue
  float *Ax = new float[n];
  this->MatrixVectorCSR(x, Ax);
  for (int i = 0; i < n; i++) {
    r[i] = b[i] - Ax[i];
    p[i] = r[i];
  }

  // calculate rsold
  rsold = mult(r, r, n);
  e = sqrt(rsold);

  for (k = 0; k < kmax && e > epsilon; k++) {

    // calculating alpha
    this->MatrixVectorCSR(p, v);
    alpha = rsold / mult(v, p, n);

    // calculates next iteration of x
    for (int i = 0; i < n; i++) {
      x[i] = x[i] + (alpha * p[i]);
    }

    // calculates r
    for (int i = 0; i < n; i++) {
      r[i] = r[i] - (alpha * v[i]);
    }

    // calculates p
    rsnew = mult(r, r, n);
    e = sqrt(rsnew);
    beta = rsnew / rsold;
    for (int i = 0; i < n; i++) {
      p[i] = r[i] + (beta * p[i]);
    }
    rsold = rsnew;
  }

  // prints out the norm of the solution and the number of iterations
  std::cout << "k: " << k << "\n";
  std::cout << "error: " << e << "\n";
  std::cout << "Solution sum: " << sum(x, n) << "\n\n";
}

/**
 * @brief solves the linear system Ax = b usin the conjugate gradient method
 * with preconditioner (Jacobi preconditioner)
 *
 * @param b         vector of real numbers
 * @param x         vector with the same size as b representing the solution
 * @param epsilon   tolerance of the approximation
 * @param kmax      maximum number of iterations
 *
 */
void CSR::solvebyConjGradientPreCond(float *b, float *x, float epsilon,
                                     int kmax) {

  int k;                   // iterations counter
  float *r = new float[n]; // vector to store the residue | r = b - Ax
  float *M = new float[n]; // stores the inverse diagonal of A
  float *p = new float[n]; // vector
  float *z = new float[n]; // vector
  float rzold, rznew, e, alpha, beta;
  float *v = new float[n]; // vector used for calculating alpha
  float val;

  // calculates M
  for (int i = 0; i < n; i++) {
    val = this->AA[IA[i] + i];
    M[i] = abs(val) > 0.0 ? 1 / val : 0.0;
  }

  // initializes the first solution
  for (int i = 0; i < n; i++) {
    x[i] = 0.5;
  }

  // calculates residue
  float *Ax = new float[n];
  this->MatrixVectorCSR(x, Ax);
  for (int i = 0; i < n; i++) {
    r[i] = b[i] - Ax[i];
  }

  // calculate z
  for (int i = 0; i < n; i++) {
    z[i] = M[i] * r[i];
    p[i] = z[i];
  }

  // calculate rzold
  rzold = mult(r, z, n);
  e = sqrt(abs(rzold));

  for (k = 0; k < kmax && e > epsilon; k++) {

    // calculating alpha
    this->MatrixVectorCSR(p, v);
    alpha = rzold / mult(v, p, n);

    // calculates next iteration of x, r and z
    for (int i = 0; i < n; i++) {
      x[i] = x[i] + (alpha * p[i]);
      r[i] = r[i] - (alpha * v[i]);
      z[i] = M[i] * r[i];
    }

    // calculates beta
    rznew = mult(r, z, n);
    beta = rznew / rzold;
    e = sqrt(abs(rznew));
    // std::cout << "e: " << e << "\n";

    // calculates p
    for (int i = 0; i < n; i++) {
      p[i] = z[i] + (beta * p[i]);
    }

    rzold = rznew;
  }

  // prints out the norm of the solution and the number of iterations
  std::cout << "k: " << k << "\n";
  std::cout << "error: " << e << "\n";
  std::cout << "Solution sum: " << sum(x, n) << "\n\n";
}
