#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N 256


char** map;
int rows;
int cols;


int count(char* str, char c) {
  int n = 0;

  for (int i = 0; i < strlen(str); i++) {
    n += (str[i] == c);
  }

  return n;
}


void north(int x, int y) {
  int j;

  if (y == 0) {
    return;
  }

  for (j = y - 1; j >= 0; j--) {
    if (map[j][x] != '.') {
      break;
    }
  }

  if (j + 1 != y) {
    map[j + 1][x] = 'O';
    map[y][x] = '.';
  }
}


void west(int x, int y) {
  int i;

  if (x == 0) {
    return;
  }

  for (i = x - 1; i >= 0; i--) {
    if (map[y][i] != '.') {
      break;
    }
  }

  if (i + 1 != x) {
    map[y][i + 1] = 'O';
    map[y][x] = '.';
  }
}


void south(int x, int y) {
  int j;

  if (y == cols - 1) {
    return;
  }

  for (j = y + 1; j < cols; j++) {
    if (map[j][x] != '.') {
      break;
    }
  }

  if (j - 1 != y) {
    map[j - 1][x] = 'O';
    map[y][x] = '.';
  }
}


void east(int x, int y) {
  int i;

  if (x == rows - 1) {
    return;
  }

  for (i = x + 1; i < rows; i++) {
    if (map[y][i] != '.') {
      break;
    }
  }

  if (i - 1 != x) {
    map[y][i - 1] = 'O';
    map[y][x] = '.';
  }
}


void tilt() {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (map[j][i] == 'O') {
        north(i, j);
      }
    }
  }

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (map[j][i] == 'O') {
        west(i, j);
      }
    }
  }

  for (int i = 0; i < rows; i++) {
    for (int j = cols - 1; j >= 0; j--) {
      if (map[j][i] == 'O') {
        south(i, j);
      }
    }
  }

  for (int i = rows - 1; i >= 0; i--) {
    for (int j = 0; j < cols; j++) {
      if (map[j][i] == 'O') {
        east(i, j);
      }
    }
  }
}


int total_load() {
  int load = 0;

  for (int j = 0; j < cols; j++) {
    load += (cols - j) * count(map[j], 'O');
  }

  return load;
}


int equal(int* a, int* b, int n) {
  for (int i = 0; i < n; i++) {
    if (a[i] != b[i]) {
      return 0;
    }
  }

  return 1;
}


int main() {
  FILE* fp = fopen("input.txt", "r");
  char line[N];
  cols = 0;

  map = malloc(sizeof(char*) * (cols + 1));

  while (fgets(line, N, fp)) {
    line[strlen(line) - 1] = '\0';
    map = realloc(map, sizeof(char*) * (cols + 1));
    map[cols++] = strdup(line);
  }

  fclose(fp);

  rows = strlen(map[0]);

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (map[j][i] == 'O') {
        north(i, j);
      }
    }
  }

  printf("Total load (1): %d\n", total_load());

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (map[j][i] == 'O') {
        west(i, j);
      }
    }
  }

  for (int i = 0; i < rows; i++) {
    for (int j = cols - 1; j >= 0; j--) {
      if (map[j][i] == 'O') {
        south(i, j);
      }
    }
  }

  for (int i = rows - 1; i >= 0; i--) {
    for (int j = 0; j < cols; j++) {
      if (map[j][i] == 'O') {
        east(i, j);
      }
    }
  }

  for (int i = 1; i < N; i++) {
    tilt();
  }

  int* loads = malloc(sizeof(int) * N);

  for (int i = 0; i < N; i++) {
    loads[i] = total_load();
    tilt();
  }

  int cycle;

  for (cycle = 1; cycle < N / 2; cycle++) {
    if (equal(loads, loads + cycle, cycle)) {
      break;
    }
  }

  int r = (1000000000 - N) % cycle;
  printf("Final load (2): %d\n", loads[r]);

  for (int i = 0; i < cols; i++) {
    free(map[i]);
  }

  free(map);
  free(loads);

  return 0;
}
