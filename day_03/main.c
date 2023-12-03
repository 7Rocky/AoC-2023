#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N 256


int find_number(char* str) {
  int number = -1;
  char c;

  for (int i = 0; i < strlen(str); i++) {
    if (str[i] < '0' || '9' < str[i]) {
      c = str[i];
      str[i] = '\0';
      number = atoi(str);
      str[i] = c;
      break;
    }
  }

  return number;
}


int is_digit(char c) {
  return '0' <= c && c <= '9';
}


char map[N][N];


void set_map(int* size_x, int* size_y) {
  FILE* fp = fopen("input.txt", "r");
  int i = 1;
  int length;

  while (fgets(map[i++] + 1, N, fp) != NULL) {
    map[i - 1][0] = '.';
    length = strlen(map[i - 1]);
    map[i - 1][length - 1] = '.';
    map[i - 1][length] = '\0';
  }

  fclose(fp);

  *size_x = strlen(map[1]);
  *size_y = i;

  for (i = 0; i < *size_x; i++) {
    map[0][i] = '.';
    map[*size_y - 1][i] = '.';
  }

  map[0][*size_x] = '\0';
  map[*size_y - 1][*size_x] = '\0';
}

int main() {
  int i;
  int j;
  int x;
  int k;
  int size_x;
  int size_y;
  int number;
  int is_part;
  int is_gear;
  int result = 0;

  set_map(&size_x, &size_y);

  for (j = 1; j < size_y - 1; j++) {
    for (i = 1; i < size_x - 1; i++) {
      if (is_digit(map[j][i])) {
        for (x = i; x < size_x; x++) {
          if (!is_digit(map[j][x])) {
            number = find_number(map[j] + i);
            break;
          }
        }

        is_part = (map[j][i - 1] != '.' || map[j][x] != '.');

        for (k = i - 1; k <= x; k++) {
          is_part |= (map[j - 1][k] != '.' || map[j + 1][k] != '.');
        }

        result += is_part ? number : 0;
        i = x;
      }
    }
  }

  printf("Sum of all part numbers (1): %d\n", result);

  result = 0;

  for (j = 1; j < size_y - 1; j++) {
    for (i = 1; i < size_x - 1; i++) {
      if (map[j][i] == '*') {
        is_gear  = (is_digit(map[j - 1][i - 1]) || is_digit(map[j - 1][i]) || is_digit(map[j - 1][i + 1]));
        is_gear += (is_digit(map[j + 1][i - 1]) || is_digit(map[j + 1][i]) || is_digit(map[j + 1][i + 1]));

        if (is_gear == 2) {
          number = 1;

          if (is_digit(map[j - 1][i - 1]) && is_digit(map[j - 1][i]) && is_digit(map[j - 1][i + 1])) {
            k = i - 1;
            while (is_digit(map[j - 1][--k]));
            number *= find_number(map[j - 1] + k + 1);
          } else if (is_digit(map[j - 1][i]) && is_digit(map[j - 1][i + 1])) {
            number *= find_number(map[j - 1] + i);
          } else if (is_digit(map[j - 1][i - 1])) {
            k = i - 1;
            while (is_digit(map[j - 1][--k]));
            number *= find_number(map[j - 1] + k + 1);
          } else if (is_digit(map[j - 1][i + 1])) {
            number *= find_number(map[j - 1] + i + 1);
          } else if (is_digit(map[j - 1][i])) {
            number *= find_number(map[j - 1] + i);
          }

          if (is_digit(map[j + 1][i - 1]) && is_digit(map[j + 1][i]) && is_digit(map[j + 1][i + 1])) {
            k = i - 1;
            while (is_digit(map[j + 1][--k]));
            number *= find_number(map[j + 1] + k + 1);
          } else if (is_digit(map[j + 1][i]) && is_digit(map[j + 1][i + 1])) {
            number *= find_number(map[j + 1] + i);
          } else if (is_digit(map[j + 1][i - 1])) {
            k = i - 1;
            while (is_digit(map[j + 1][--k]));
            number *= find_number(map[j + 1] + k + 1);
          } else if (is_digit(map[j + 1][i + 1])) {
            number *= find_number(map[j + 1] + i + 1);
          } else if (is_digit(map[j + 1][i])) {
            number *= find_number(map[j + 1] + i);
          }

          result += number;
          continue;
        }

        is_gear  = (is_digit(map[j - 1][i - 1]) || is_digit(map[j - 1][i]) || is_digit(map[j - 1][i + 1]));
        is_gear += is_digit(map[j][i + 1]);

        if (is_gear == 2) {
          number = find_number(map[j] + i + 1);

          if (is_digit(map[j - 1][i - 1])) {
            k = i - 1;
            while (is_digit(map[j - 1][--k]));
            number *= find_number(map[j - 1] + k + 1);
          }

          result += number;
          continue;
        }

        is_gear  = (is_digit(map[j + 1][i - 1]) || is_digit(map[j + 1][i]) || is_digit(map[j + 1][i + 1]));
        is_gear += is_digit(map[j][i + 1]);

        if (is_gear == 2) {
          number = find_number(map[j] + i + 1);

          if (is_digit(map[j + 1][i - 1])) {
            k = i - 1;
            while (is_digit(map[j + 1][--k]));
            number *= find_number(map[j + 1] + k + 1);
          }

          result += number;
          continue;
        }

        is_gear  = (is_digit(map[j - 1][i - 1]) || is_digit(map[j - 1][i]) || is_digit(map[j - 1][i + 1]));
        is_gear += is_digit(map[j][i - 1]);

        if (is_gear == 2) {
          number = 1;

          for (k = i - 1; k >= 0; k--) {
            if (!is_digit(map[j][k])) {
              number *= find_number(map[j] + k + 1);
              break;
            }
          }

          if (is_digit(map[j - 1][i + 1])) {
            number *= find_number(map[j - 1] + i + 1);
          }

          result += number;
          continue;
        }

        is_gear  = (is_digit(map[j + 1][i - 1]) || is_digit(map[j + 1][i]) || is_digit(map[j + 1][i + 1]));
        is_gear += is_digit(map[j][i - 1]);

        if (is_gear == 2) {
          number = 1;

          for (k = i - 1; k >= 0; k--) {
            if (!is_digit(map[j][k])) {
              number *= find_number(map[j] + k + 1);
              break;
            }
          }

          if (is_digit(map[j + 1][i + 1])) {
            number *= find_number(map[j + 1] + i + 1);
          }

          result += number;
          continue;
        }

        is_gear = is_digit(map[j][i - 1]) + is_digit(map[j][i + 1]);

        if (is_gear == 2) {
          number = find_number(map[j] + i + 1);

          for (k = i - 1; k >= 0; k--) {
            if (!is_digit(map[j][k])) {
              number *= find_number(map[j] + k + 1);
              break;
            }
          }

          result += number;
          continue;
        }

        if (is_digit(map[j - 1][i - 1]) && !is_digit(map[j - 1][i]) && is_digit(map[j - 1][i + 1])) {
          number = find_number(map[j - 1] + i + 1);

          for (k = i - 2; k >= 0; k--) {
            if (!is_digit(map[j - 1][k])) {
              number *= find_number(map[j - 1] + k + 1);
              break;
            }
          }

          result += number;
          continue;
        }

        if (is_digit(map[j + 1][i - 1]) && !is_digit(map[j + 1][i]) && is_digit(map[j + 1][i + 1])) {
          number = find_number(map[j + 1] + i + 1);

          for (k = i - 2; k >= 0; k--) {
            if (!is_digit(map[j + 1][k])) {
              number *= find_number(map[j + 1] + k + 1);
              break;
            }
          }

          result += number;
        }
      }
    }
  }

  printf("Sum of all gear ratios (2): %d\n", result);

  return 0;
}
