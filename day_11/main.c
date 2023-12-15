#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N 256


int length;
char** space;


int abs(int x) {
  return x < 0? -x : x;
}


long shortest_path(int p[], int q[]) {
  return abs(p[0] - q[0]) + abs(p[1] - q[1]);
}


int find_within(int* values, int length, int a, int b) {
  int res = 0;

  if (a > b) {
    b ^= a;
    a ^= b;
    b ^= a;
  }

  for (int i = 0; i < length; i++) {
    res += (a < values[i] && values[i] < b);
  }

  return res;
}


long sum_shortest_paths(long weight, int** galaxies, int num_galaxies, int* empty_cols, int num_empty_cols, int* empty_rows, int num_empty_rows) {
  long res = 0;

  for (int i = 0; i < num_galaxies; i++) {
    for (int j = i + 1; j < num_galaxies; j++) {
      res += shortest_path(galaxies[i], galaxies[j]);
      res += (weight - 1) * find_within(empty_cols, num_empty_cols, galaxies[i][0], galaxies[j][0]);
      res += (weight - 1) * find_within(empty_rows, num_empty_rows, galaxies[i][1], galaxies[j][1]);
    }
  }

  return res;
}


int main() {
  FILE* fp = fopen("input.txt", "r");
  char line[N];

  while (fgets(line, N, fp)) {
    line[strlen(line) - 1] = '\0';
    space = realloc(space, sizeof(char*) * (length + 1));

    space[length++] = strdup(line);
  }

  fclose(fp);

  int** galaxies = malloc(sizeof(int*) * 1);
  int num_galaxies = 0;

  for (int j = 0; j < length; j++) {
    for (int i = 0; i < strlen(space[j]); i++) {
      if (space[j][i] == '#') {
        galaxies = realloc(galaxies, sizeof(int*) * (num_galaxies + 1));
        galaxies[num_galaxies] = malloc(sizeof(int) * 2);
        galaxies[num_galaxies][0] = i;
        galaxies[num_galaxies++][1] = j;
      }
    }
  }

  int* empty_rows = malloc(sizeof(int) * 1);
  int num_empty_rows = 0;

  for (int j = 0; j < length; j++) {
    if (strcspn(space[j], "#") == strlen(space[j])) {
      empty_rows = realloc(empty_rows, sizeof(int) * (num_empty_rows + 1));
      empty_rows[num_empty_rows++] = j;
    }
  }

  int* empty_cols = malloc(sizeof(int) * 1);
  int num_empty_cols = 0;
  int found;
  int j;

  for (int i = 0; i < strlen(space[0]); i++) {
    found = 0;

    for (j = 0; j < length; j++) {
      if (space[j][i] == '#') {
        found = 1;
        break;
      }
    }

    if (!found) {
      empty_cols = realloc(empty_cols, sizeof(int) * (num_empty_cols + 1));
      empty_cols[num_empty_cols++] = i;
    }
  }

  printf("Sum of shortest paths (1): %ld\n", sum_shortest_paths(2, galaxies, num_galaxies, empty_cols, num_empty_cols, empty_rows, num_empty_rows));
  printf("Sum of shortest paths (2): %ld\n", sum_shortest_paths(1000000, galaxies, num_galaxies, empty_cols, num_empty_cols, empty_rows, num_empty_rows));

  for (int i = 0; i < num_galaxies; i++) {
    free(galaxies[i]);
  }

  for (int i = 0; i < length; i++) {
    free(space[i]);
  }

  free(empty_cols);
  free(empty_rows);
  free(galaxies);
  free(space);

  return 0;
}
