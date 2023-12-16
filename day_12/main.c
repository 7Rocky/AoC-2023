#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N 256


typedef struct {
  int* values;
  int length;
} int_array_t;

typedef struct {
  char* spring;
  int_array_t* groups;
} record_t;


int equal(int_array_t* a, int_array_t* b) {
  if (a->length != b->length) {
    return 0;
  }

  for (int i = 0; i < a->length; i++) {
    if (a->values[i] != b->values[i]) {
      return 0;
    }
  }

  return 1;
}


int_array_t* parse_groups(char* str) {
  int* values = malloc(sizeof(int) * 1);
  char* num_str;
  int length = 0;

  for (num_str = strtok(str, ","); num_str != NULL; num_str = strtok(NULL, ",")) {
    values = realloc(values, sizeof(int) * (length + 1));
    values[length++] = atoi(num_str);
  }

  int_array_t* groups = malloc(sizeof(int_array_t));
  groups->values = values;
  groups->length = length;

  return groups;
}


int_array_t* find_groups(char* spring) {
  int* values = malloc(sizeof(int) * 1);
  int length = 0;
  int value = 0;
  int stop = 0;

  for (int i = 0; i < strlen(spring); i++) {
    while (spring[i] == '#') {
      value++;
      stop = 1;
      i++;
    }

    if (stop) {
      values = realloc(values, sizeof(int) * (length + 1));
      values[length++] = value;
      value = 0;
      stop = 0;
    }
  }

  int_array_t* groups = malloc(sizeof(int_array_t));
  groups->values = values;
  groups->length = length;

  return groups;
}


int count(char* str, char c) {
  int n = 0;

  for (int i = 0; i < strlen(str); i++) {
    n += (str[i] == c);
  }

  return n;
}


char* spring;
int* groups;
int num_groups;
int num_springs;
long cached_answers[N][N];


long count_possibilities(int group_index, int spring_index) {
  if (group_index == num_groups) {
    return 1;
  }

  if (spring_index < num_springs && cached_answers[group_index][spring_index] != -1) {
    return cached_answers[group_index][spring_index];
  }

  long count = 0;

  for (int i = spring_index; i <= num_springs - groups[group_index]; i++) {
    int valid = 1;

    for (int j = 0; j < groups[group_index]; j++) {
      if (spring[i + j] == '.') {
        valid = 0;
      }
    }

    if (i + groups[group_index] < num_springs && spring[i + groups[group_index]] == '#') {
      valid = 0;
    }

    if (i > 0 && spring[i - 1] == '#') {
      valid = 0;
    }

    if (group_index == num_groups - 1) {
      for (int j = i + groups[group_index] + 1; j < num_springs; j++) {
        if (spring[j] == '#') {
          valid = 0;
        }
      }
    }

    if (valid) {
      count += count_possibilities(group_index + 1, i + groups[group_index] + 1);
    }

    if (spring[i] == '#') {
      break;
    }
  }

  if (spring_index < num_springs) {
    cached_answers[group_index][spring_index] = count;
  }

  return count;
}


int main() {
  FILE* fp = fopen("input.txt", "r");
  char line[N];
  record_t** records = malloc(sizeof(record_t*) * 1);
  record_t* record = malloc(sizeof(record_t));
  int num_records = 0;
  int space_index;

  while (fgets(line, N, fp)) {
    line[strlen(line) - 1] = '\0';
    space_index = strcspn(line, " ");
    record = malloc(sizeof(record_t));
    line[space_index] = '\0';
    record->spring = strdup(line);
    record->groups = parse_groups(line + space_index + 1);

    records = realloc(records, sizeof(record_t*) * (num_records + 1));
    records[num_records++] = record;
  }

  fclose(fp);

  int questions;
  int n;
  long possibilities = 0;

  for (int k = 0; k < num_records; k++) {
    questions = count(records[k]->spring, '?');
    spring = malloc(strlen(records[k]->spring) + 1);

    for (int i = 0; i < (1 << questions); i++) {
      n = i;
      strncpy(spring, records[k]->spring, strlen(records[k]->spring) + 1);

      for (int j = 0; j < strlen(records[k]->spring); j++) {
        while (spring[j] != '?' && spring[j] != '\0') {
          j++;
        }

        spring[j] = n & 1 ? '#' : '.';
        n >>= 1;
      }

      possibilities += equal(find_groups(spring), records[k]->groups);
    }

    free(spring);
  }

  printf("Possible arrangements (1): %ld\n", possibilities);

  for (int k = 0; k < num_records; k++) {
    spring = strdup(records[k]->spring);
    records[k]->spring = realloc(records[k]->spring, strlen(records[k]->spring) * 5 + 5);

    for (int i = 1; i < 5; i++) {
      records[k]->spring[i * strlen(spring) + i - 1] = '?';
      strncpy(records[k]->spring + i * strlen(spring) + i, spring, strlen(spring));
    }

    records[k]->spring[5 * strlen(spring) + 4] = '\0';
    free(spring);

    records[k]->groups->values = realloc(records[k]->groups->values, sizeof(int) * records[k]->groups->length * 5);

    for (int i = 1; i < 5; i++) {
      for (int j = 0; j < records[k]->groups->length; j++) {
        records[k]->groups->values[records[k]->groups->length * i + j] = records[k]->groups->values[j];
      }
    }

    records[k]->groups->length *= 5;
  }

  possibilities = 0;

  for (int k = 0; k < num_records; k++) {
    spring = records[k]->spring;
    groups = records[k]->groups->values;
    num_groups = records[k]->groups->length;
    num_springs = strlen(spring);

    for (int h = 0; h < num_groups; h++) {
            for (int s = 0; s < num_springs; s++) {
                cached_answers[h][s] = -1;
            }
        }

    possibilities += count_possibilities(0, 0);
  }

  printf("Possible arrangements (2): %ld\n", possibilities);

  for (int i = 0; i < num_records; i++) {
    free(records[i]->groups->values);
    free(records[i]->groups);
    free(records[i]->spring);
    free(records[i]);
  }

  free(records);

  return 0;
}
