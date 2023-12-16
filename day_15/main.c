#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N 256


typedef struct lens_t lens_t;

struct lens_t {
  char* label;
  char operation;
  int power;
  lens_t* prev;
  lens_t* next;
};


lens_t* hashmap[N];


int hash(char* str) {
  int h = 0;

  for (int i = 0; i < strlen(str); i++) {
    h += str[i];
    h *= 17;
    h %= 256;
  }

  return h;
}


void box(lens_t* lens) {
  int h = hash(lens->label);
  lens_t* l;
  int found;

  if (hashmap[h] != NULL) {
    found = 0;

    for (l = hashmap[h]; l->next != NULL; l = l->next) {
      if (strcmp(l->label, lens->label) == 0) {
        found = 1;
        break;
      }
    }

    if (found || strcmp(l->label, lens->label) == 0) {
      if (lens->operation == '-') {
        if (l->next != NULL) {
          l->next->prev = l->prev;
        }

        if (l->prev != NULL) {
          l->prev->next = l->next;
        } else {
          hashmap[h] = l->next;
        }

        free(l->label);
        free(l);
      } else {
        l->power = lens->power;
        free(lens->label);
        free(lens);
      }
    } else if (lens->operation == '=') {
      l->next = lens;
      lens->prev = l;
    }
  } else if (lens->operation == '=') {
    hashmap[h] = lens;
  }
}


int main() {
  FILE* fp = fopen("input.txt", "r");
  char line[N];
  int i;
  lens_t* lens;

  int sum_hashes = 0;

  for (i = 0; i < N; i++) {
    hashmap[i] = NULL;
  }

  while (!feof(fp)) {
    i = 0;
    fread(line + i, 1, 1, fp);

    while (line[i] != ',' && !feof(fp)) {
      i++;
      fread(line + i, 1, 1, fp);
    }

    line[i] = '\0';
    sum_hashes += hash(line);

    lens = malloc(sizeof(lens_t));

    if (line[strlen(line) - 1] == '-') {
      lens->label = strndup(line, strlen(line) - 1);
      lens->operation = '-';
    } else {
      lens->label = strndup(line, strcspn(line, "="));
      lens->operation = '=';
      lens->power = atoi(strchr(line, '=') + 1);
    }

    lens->next = NULL;
    lens->prev = NULL;

    box(lens);
  }

  fclose(fp);

  printf("Sum of hashes (1): %d\n", sum_hashes);

  long focusing_power = 0;

  for (int h = 0; h < N; h++) {
    if (hashmap[h] == NULL) {
      continue;
    }

    for (lens = hashmap[h], i = 0; lens->next != NULL; lens = lens->next, i++) {
      focusing_power += (h + 1) * (i + 1) * lens->power;
    }

    focusing_power += (h + 1) * (i + 1) * lens->power;

    if (lens->prev != NULL) {
      while (lens->prev != NULL) {
        lens = lens->prev;
        free(lens->next->label);
        free(lens->next);
      }
    }

    free(lens->label);
    free(lens);
  }

  printf("Focusing power (2): %ld\n", focusing_power);

  return 0;
}
