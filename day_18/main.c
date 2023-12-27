#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N 256


typedef enum { U, D, L, R } dir_t;

typedef struct {
  dir_t dir;
  int length;
  char color[7];
} rule_t;

typedef struct {
  int x;
  int y;
} coordinate_t;


rule_t* parse_rule(char* line) {
  int second_space = strcspn(line + 2, " ");
  rule_t* rule = malloc(sizeof(rule_t));

  switch (line[0]) {
  case 'U':
    rule->dir = U;
    break;
  case 'D':
    rule->dir = D;
    break;
  case 'L':
    rule->dir = L;
    break;
  case 'R':
    rule->dir = R;
    break;
  }

  line[second_space + 2] = '\0';
  rule->length = atoi(line + 2);

  strncpy(rule->color, line + second_space + 5, 6);

  return rule;
}


coordinate_t move(coordinate_t current, dir_t dir) {
  switch (dir) {
  case U:
    return (coordinate_t) { .x = current.x - 1, .y = current.y };
  case D:
    return (coordinate_t) { .x = current.x + 1, .y = current.y };
  case L:
    return (coordinate_t) { .x = current.x, .y = current.y - 1 };
  case R:
    return (coordinate_t) { .x = current.x, .y = current.y + 1 };
  }
}


int main() {
  FILE* fp = fopen("input.txt", "r");
  char line[N];
  int num_rules = 0;

  rule_t** rules = malloc(sizeof(rule_t*) * (num_rules + 1));

  while (fgets(line, N, fp)) {
    line[strlen(line) - 1] = '\0';
    rules = realloc(rules, sizeof(rule_t*) * (num_rules + 1));
    rules[num_rules++] = parse_rule(line);
  }

  fclose(fp);

  long dx;
  long dy;
  long x = 0;
  long y = 0;
  long perimeter = 0;
  long inner_area = 0;

  for (int i = 0; i < num_rules; i++) {
    dx = dy = 0;

    switch (rules[i]->dir) {
    case U:
      dy = -rules[i]->length;
      break;
    case D:
      dy = rules[i]->length;
      break;
    case L:
      dx = -rules[i]->length;
      break;
    case R:
      dx = rules[i]->length;
      break;
    }

    x += dx;
    y += dy;

    inner_area += x * dy;
    perimeter += rules[i]->length;
  }

  printf("Area (1): %ld\n", inner_area + perimeter / 2 + 1);

  x = y = 0;
  perimeter = inner_area = 0;

  for (int i = 0; i < num_rules; i++) {
    dx = dy = 0;

    switch (rules[i]->color[5]) {
    case '3':
      dy = -(strtol(rules[i]->color, NULL, 16) >> 4);
      break;
    case '1':
      dy = (strtol(rules[i]->color, NULL, 16) >> 4);
      break;
    case '2':
      dx = -(strtol(rules[i]->color, NULL, 16) >> 4);
      break;
    case '0':
      dx = (strtol(rules[i]->color, NULL, 16) >> 4);
      break;
    }

    x += dx;
    y += dy;

    inner_area += x * dy;
    perimeter += (strtol(rules[i]->color, NULL, 16) >> 4);
  }

  printf("Area (2): %ld\n", inner_area + perimeter / 2 + 1);

  for (int i = 0; i < num_rules; i++) {
    free(rules[i]);
  }

  free(rules);

  return 0;
}
