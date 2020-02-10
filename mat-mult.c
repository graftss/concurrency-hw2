#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

struct timespec *tp = 0;

long get_nano_time() {
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, tp);
  return 1e9 * tp->tv_sec + tp->tv_nsec;
}

// return units are seconds
float diff_nano_times(long start, long finish) {
  return (double) (finish - start) / 1e9;
}

void naive_matmul(int N, double *A, double *B, double *C) {
  int i, j, k;

  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      for (k = 0; k < N; k++)
        C[i * N + j] = C[i * N + j] + A[i * N + k] * B[k * N + j];
}

void single_optimized_matmul(int N, double *A, double *B, double *C) {
  int i, j, k;

  // keep track of the index of A and B corresponding to
  // the row being iterated over in their respective loops
  int a_offset = 0, b_offset = 0;

  // local space to store the row of C being computed. once
  // finished, it is copied to C all at once
  // it needs to start at all 0's, since that's the assumption on C.
  double temp_row[N];
  for (i = 0; i < N; i++) temp_row[i] = 0;

  for (i = 0; i < N; i++, a_offset += N, b_offset = 0) {
    for (k = 0; k < N; k++, b_offset += N) {
      for (j = 0; j < N; j++) {
        temp_row[j] += A[a_offset + k] * B[b_offset + j];
      }
    }

    // copy `temp_row` to `C` and reset `temp_row` to 0's
    for (j = 0; j < N; j++) {
      C[a_offset + j] = temp_row[j];
      temp_row[j] = 0;
    }
  }
}

int equal_mats(int N, double *A, double *B) {
  for (int i = 0; i < N * N; i++) {
    if (fabs(A[i] - B[i]) > 0.001) {
      return 0;
    }
  }

  return 1;
}

double random_double() {
  return (double) rand() / RAND_MAX;
}

void randomize_mat(int N, double *A) {
  for (int i = 0; i < N * N; i++) {
    A[i] = random_double();
  }
}

void clear_mat(int N, double *A) {
  for (int i = 0; i < N * N; i++) {
    A[i] = 0;
  }
}

double *alloc_mat(int N) {
  return malloc(N * N * sizeof(double));
}

void run_trial(int N) {
  double *A = alloc_mat(N);
  double *B = alloc_mat(N);
  double *C = alloc_mat(N);
  double *D = alloc_mat(N);

  randomize_mat(N, A);
  randomize_mat(N, B);
  clear_mat(N, C);
  clear_mat(N, D);

  long init = get_nano_time();
  naive_matmul(N, A, B, C);
  float naive_diff = diff_nano_times(init, get_nano_time());
  printf("naive: %.10f seconds\n", naive_diff);

  init = get_nano_time();
  single_optimized_matmul(N, A, B, D);
  float optimized_diff = diff_nano_times(init, get_nano_time());
  printf("optimized: %.10f seconds\n", optimized_diff);

  // we take it for granted that the naive implementation is correct,
  // so if the optimized result agrees, then that's correct too.
  char *equal = equal_mats(N, C, D) == 1 ? "true" : "false";
  printf("algorithms agree: %s\n", equal);
}

int main()
{
  srand(time(0));
  tp = malloc(sizeof(struct timespec));

  run_trial(1024);
  run_trial(2048);

  return 0;
}
