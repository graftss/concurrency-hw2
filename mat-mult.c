#include <stdio.h>
#include <stdlib.h>
#include <time.h>


long get_nano_time() {
  struct timespec *tp = malloc(sizeof(struct timespec));
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

  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      for (k = 0; k < N; k++)
        C[i * N + j] = C[i * N + j] + A[i * N + k] * B[k * N + j];
}

double random_double() {
  return (double) rand() / RAND_MAX;
}

void randomize_mat(int N, double *A) {
  for (int i = 0; i < N * N; i++) {
    A[i] = random_double();
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

  long init = get_nano_time();
  naive_matmul(N, A, B, C);
  float naive_diff = diff_nano_times(init, get_nano_time());
  printf("naive: %.10f seconds\n", naive_diff);

  init = get_nano_time();
  single_optimized_matmul(N, A, B, D);
  float optimized_diff = diff_nano_times(init, get_nano_time());
  printf("optimized: %.10f seconds\n", optimized_diff);
}

int main()
{
  srand(time(0));
  run_trial(256);
  return 0;
}