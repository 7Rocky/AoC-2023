#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N 256


typedef struct {
  char** pattern;
  int length;
} pattern_t;


int colcmp(int a, int b, pattern_t* p) {
  int equal = 0;

  for (int j = 0; j < p->length; j++) {
    if (p->pattern[j][a] != p->pattern[j][b]) {
      equal = 1;
      break;
    }
  }

  return equal;
}


int test_reflection_x(int r, pattern_t* p) {
  int i = 0;

  while (r - i >= 0 && r + i + 1 < p->length && strcmp(p->pattern[r - i], p->pattern[r + i + 1]) == 0) {
    i++;
  }

  return (r - i < 0 || r + i + 1 >= p->length);
}


int test_reflection_y(int c, pattern_t* p) {
  int i = 0;

  while (c - i >= 0 && c + i + 1 < strlen(p->pattern[0]) && colcmp(c - i, c + i + 1, p) == 0) {
    i++;
  }

  return (c - i < 0 || c + i + 1 >= strlen(p->pattern[0]));
}


int find_reflection(pattern_t* p, int avoid) {
  int res = 0;

  for (int j = 1; j < p->length; j++) {
    if (strcmp(p->pattern[j - 1], p->pattern[j]) == 0) {
      if (test_reflection_x(j - 1, p) && 100 * j != avoid) {
        res += 100 * j;
        break;
      }
    }
  }

  if (res) {
    return res;
  }

  for (int i = 1; i < strlen(p->pattern[0]); i++) {
    if (colcmp(i - 1, i, p) == 0) {
      if (test_reflection_y(i - 1, p) && i != avoid) {
        res += i;
        break;
      }
    }
  }

  return res;
}


void flip(int j, int i, char** pattern) {
  if (pattern[j][i] == '.') {
    pattern[j][i] = '#';
  } else {
    pattern[j][i] = '.';
  }
}


int main() {
  FILE* fp = fopen("input.txt", "r");
  char line[N];
  pattern_t** patterns = malloc(sizeof(pattern_t*) * 1);
  int num_patterns = 0;
  pattern_t* pattern = malloc(sizeof(pattern_t));
  int length = 0;

  pattern->pattern = malloc(sizeof(char*) * 1);
  pattern->length = 0;

  int reflections = 0;

  while (fgets(line, N, fp)) {
    line[strlen(line) - 1] = '\0';

    if (strlen(line) <= 1) {
      patterns = realloc(patterns, sizeof(char**) * (num_patterns + 1));
      patterns[num_patterns++] = pattern;

      pattern = malloc(sizeof(pattern_t));
      pattern->pattern = malloc(sizeof(char*) * 1);
      pattern->length = 0;
      continue;
    }

    pattern->pattern = realloc(pattern->pattern, sizeof(char*) * (pattern->length + 1));
    pattern->pattern[pattern->length++] = strdup(line);
  }

  fclose(fp);

  patterns = realloc(patterns, sizeof(char**) * (num_patterns + 1));
  patterns[num_patterns++] = pattern;

  for (int i = 0; i < num_patterns; i++) {
    reflections += find_reflection(patterns[i], 0);
  }

  printf("Reflections score (1): %d\n", reflections);

  reflections = 0;

  for (int k = 0; k < num_patterns; k++) {
    int found = 0;
    int initial = find_reflection(patterns[k], 0);

    for (int j = 0; j < patterns[k]->length; j++) {
      for (int i = 0; i < strlen(patterns[k]->pattern[j]); i++) {
        flip(j, i, patterns[k]->pattern);
        int reflection = find_reflection(patterns[k], initial);
        flip(j, i, patterns[k]->pattern);

        if (reflection) {
          reflections += reflection;
          found = 1;
          break;
        }
      }

      if (found) {
        break;
      }
    }
  }

  printf("Patched reflections score (2): %d\n", reflections);

  for (int i = 0; i < num_patterns; i++) {
    free(patterns[i]->pattern);
    free(patterns[i]);
  }
  
  free(patterns);

  return 0;
}
