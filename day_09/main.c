#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N 256


typedef struct {
  long* values;
  int length;
} numbers_t;


double lagrange_basis(long x_i, long x, int length) {
  double num = 1;
  double den = 1;

  for (long j = 0; j < length; j++) {
    if (j != x_i) {
      num *= x - j;
      den *= x_i - j;
    }
  }

  return (double) (num / den);
}


long lagrange_evaluate(numbers_t numbers, long x) {
  long y = 0;

  for (int i = 0; i < numbers.length; i++) {
    y += (long) (numbers.values[i] * lagrange_basis(i, x, numbers.length));
  }

  return y;
}


long solve(numbers_t* numbers_list, int length, int part) {
  long result = 0;

  for (int i = 0; i < length; i++) {
    if (part == 1) {
      result += lagrange_evaluate(numbers_list[i], numbers_list[i].length);
    } else {
      result += lagrange_evaluate(numbers_list[i], -1);
    }
  }

  return result;
}


int main() {
  FILE* fp = fopen("input.txt", "r");
  int length = 0;
  char line[N];
  numbers_t* numbers_list = malloc(sizeof(numbers_t) * 1);
  numbers_t numbers;
  long* values;
  char* s;
  int i;

  while (fgets(line, N, fp)) {
    line[strlen(line) - 1] = '\0';
    numbers_list = realloc(numbers_list, sizeof(numbers_t) * (length + 1));
    values = malloc(sizeof(long) * 1);

    for (s = strtok(line, " "), i = 0; s; s = strtok(NULL, " ")) {
      values = realloc(values, sizeof(long) * (i + 1));
      values[i++] = atol(s);
    }

    numbers_list[length++] = (numbers_t) { .values = values, .length = i };
  }

  fclose(fp);

  printf("Result (1): %ld\n", solve(numbers_list, length, 1));
  printf("Result (2): %ld\n", solve(numbers_list, length, 2));

  for (int i = 0; i < length; i++) {
    free(numbers_list[i].values);
  }

  free(numbers_list);

  return 0;
}
