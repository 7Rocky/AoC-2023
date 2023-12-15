#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N 256


int length;
char** map;


int* next_pos(int* cur, int* prev) {
  int x = cur[0];
  int y = cur[1];
  int* next = malloc(sizeof(int) * 2);
  next[0] = x;
  next[1] = y;

  switch (map[y][x]) {
  case '|':
    if (prev[1] != y - 1 && y > 0) {
      if (map[y - 1][x] == '7' || map[y - 1][x] == 'F' || map[y - 1][x] == '|' || map[y - 1][x] == 'S') {
        next[1] = y - 1;
        break;
      }
    }

    if (prev[1] != y + 1 && y + 1 < length) {
      if (map[y + 1][x] == 'J' || map[y + 1][x] == 'L' || map[y + 1][x] == '|' || map[y + 1][x] == 'S') {
        next[1] = y + 1;
        break;
      }
    }

    break;

  case '-':
    if (prev[0] != x - 1 && x > 0) {
      if (map[y][x - 1] == 'F' || map[y][x - 1] == 'L' || map[y][x - 1] == '-' || map[y][x - 1] == 'S') {
        next[0] = x - 1;
        break;
      }
    }

    if (prev[0] != x + 1 && x + 1 < strlen(map[y])) {
      if (map[y][x + 1] == 'J' || map[y][x + 1] == '7' || map[y][x + 1] == '-' || map[y][x + 1] == 'S') {
        next[0] = x + 1;
        break;
      }
    }

    break;

  case 'F':
    if (prev[0] != x + 1 && x + 1 < strlen(map[y])) {
      if (map[y][x + 1] == 'J' || map[y][x + 1] == '7' || map[y][x + 1] == '-' || map[y][x + 1] == 'S') {
        next[0] = x + 1;
        break;
      }
    }

    if (prev[1] != y + 1 && y + 1 < length) {
      if (map[y + 1][x] == 'J' || map[y + 1][x] == 'L' || map[y + 1][x] == '|' || map[y + 1][x] == 'S') {
        next[1] = y + 1;
        break;
      }
    }

    break;

  case 'J':
    if (prev[0] != x - 1 && x > 0) {
      if (map[y][x - 1] == 'F' || map[y][x - 1] == 'L' || map[y][x - 1] == '-' || map[y][x - 1] == 'S') {
        next[0] = x - 1;
        break;
      }
    }

    if (prev[1] != y - 1 && y > 0) {
      if (map[y - 1][x] == '7' || map[y - 1][x] == 'F' || map[y - 1][x] == '|' || map[y - 1][x] == 'S') {
        next[1] = y - 1;
        break;
      }
    }

    break;

  case 'L':
    if (prev[0] != x + 1 && x + 1 < strlen(map[y])) {
      if (map[y][x + 1] == 'J' || map[y][x + 1] == '7' || map[y][x + 1] == '-' || map[y][x + 1] == 'S') {
        next[0] = x + 1;
        break;
      }
    }

    if (prev[1] != y - 1 && y > 0) {
      if (map[y - 1][x] == '7' || map[y - 1][x] == 'F' || map[y - 1][x] == '|' || map[y - 1][x] == 'S') {
        next[1] = y - 1;
        break;
      }
    }

    break;

  case '7':
    if (prev[0] != x - 1 && x > 0) {
      if (map[y][x - 1] == 'F' || map[y][x - 1] == 'L' || map[y][x - 1] == '-' || map[y][x - 1] == 'S') {
        next[0] = x - 1;
        break;
      }
    }

    if (prev[1] != y + 1 && y + 1 < length) {
      if (map[y + 1][x] == 'J' || map[y + 1][x] == 'L' || map[y + 1][x] == '|' || map[y + 1][x] == 'S') {
        next[1] = y + 1;
        break;
      }
    }

    break;

  case 'S':
    if (prev[0] != x - 1 && x > 0) {
      if (map[y][x - 1] == 'F' || map[y][x - 1] == 'L' || map[y][x - 1] == '-' || map[y][x - 1] == 'S') {
        next[0] = x - 1;
        break;
      }
    }

    if (prev[0] != x + 1 && x + 1 < strlen(map[y])) {
      if (map[y][x + 1] == 'J' || map[y][x + 1] == '7' || map[y][x + 1] == '-' || map[y][x + 1] == 'S') {
        next[0] = x + 1;
        break;
      }
    }

    if (prev[1] != y - 1 && y > 0) {
      if (map[y - 1][x] == '7' || map[y - 1][x] == 'F' || map[y - 1][x] == '|' || map[y - 1][x] == 'S') {
        next[1] = y - 1;
        break;
      }
    }

    if (prev[1] != y + 1 && y + 1 < length) {
      if (map[y + 1][x] == 'J' || map[y + 1][x] == 'L' || map[y + 1][x] == '|' || map[y + 1][x] == 'S') {
        next[1] = y + 1;
        break;
      }
    }

    break;
  }

  prev[0] = x;
  prev[1] = y;

  return next;
}


int cycle(int x, int y, int*** animal) {
  int* cur = malloc(sizeof(int) * 2);
  int* prev = malloc(sizeof(int) * 2);
  int steps = 0;

  cur[0] = x;
  cur[1] = y;
  prev[0] = -1;
  prev[1] = -1;

  (*animal)[steps] = malloc(sizeof(int) * 2);
  (*animal)[steps][0] = cur[0];
  (*animal)[steps][1] = cur[1];

  do {
    cur = next_pos(cur, prev);
    steps++;

    (*animal) = realloc((*animal), sizeof(int*) * (steps + 1));
    (*animal)[steps] = malloc(sizeof(int) * 2);
    (*animal)[steps][0] = cur[0];
    (*animal)[steps][1] = cur[1];
  } while (cur[0] != x || cur[1] != y);

  free(cur);
  free(prev);

  return steps;
}


int min(int x, int y) {
  return x < y ? x : y;
}


int index_of(int x, int y, int** coords, int length) {
  for (int i = 0; i < length; i++) {
    if (coords[i][0] == x && coords[i][1] == y) {
      return i;
    }
  }

  return -1;
}


int is_inside(int x, int y, int** animal, int length_x, int length) {
  if (index_of(x, y, animal, length) != -1) {
    return 0;
  }

  int nF = 0;
  int n7 = 0;
  int nL = 0;
  int nJ = 0;
  int nB = 0;
  int index = -1;

  for (int i = x; i < length_x; i++) {
    if ((index = index_of(i, y, animal, length)) != -1) {
      nF += (map[animal[index][1]][animal[index][0]] == 'F');
      n7 += (map[animal[index][1]][animal[index][0]] == '7');
      nL += (map[animal[index][1]][animal[index][0]] == 'L');
      nJ += (map[animal[index][1]][animal[index][0]] == 'J');
      nB += (map[animal[index][1]][animal[index][0]] == '|');
      nB += (map[animal[index][1]][animal[index][0]] == 'S');
    }
  }

  return (nF + nJ + nL + n7 + nB - min(nF, nJ) - min(nL, n7)) % 2;
}


int main() {
  FILE* fp = fopen("input.txt", "r");
  char line[N];
  int x = -1;
  int y = -1;

  while (fgets(line, N, fp)) {
    line[strlen(line) - 1] = '\0';
    map = realloc(map, sizeof(char*) * (length + 1));

    if (strcspn(line, "S") < strlen(line)) {
      x = strcspn(line, "S");
      y = length;
    }

    map[length++] = strdup(line);
  }

  fclose(fp);

  int** animal = malloc(sizeof(int*));
  int steps = cycle(x, y, &animal);

  printf("Steps (1): %d\n", steps / 2);

  int inside = 0;

  for (int j = 0; j < length; j++) {
    for (int i = 0; i < strlen(map[j]); i++) {
      inside += is_inside(i, j, animal, strlen(map[j]), steps);
    }
  }

  printf("Inside points (2): %d\n", inside);

  for (int i = 0; i < steps; i++) {
    free(animal[i]);
  }

  free(animal);

  for (int i = 0; i < length; i++) {
    free(map[i]);
  }

  free(map);

  return 0;
}
