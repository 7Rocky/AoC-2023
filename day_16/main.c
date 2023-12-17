#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N 256


typedef enum {
  UP,
  RIGHT,
  DOWN,
  LEFT,
} direction_t;

typedef struct beam_t beam_t;

struct beam_t {
  int x;
  int y;
  direction_t d;
  beam_t* next;
};


char** map;
int cols;
int rows;
int* tiles;
int* seen;
beam_t* beams_queue;


int count() {
  int n = 0;

  for (int i = 0; i < rows * cols; i++) {
    n += tiles[i];
  }

  return n;
}


void queue(beam_t* beam) {
  beam_t* b = beams_queue;

  if (seen[beam->x * rows + beam->y] & (1 << beam->d)) {
    return;
  }

  seen[beam->x * rows + beam->y] |= (1 << beam->d);

  if (b != NULL) {
    while (b->next != NULL) {
      b = b->next;
    }

    b->next = beam;
  } else {
    beams_queue = beam;
  }
}


void traverse(beam_t* b) {
  if (map[b->y][b->x] == '/') {
    b->d = b->d == UP ? LEFT : b->d == DOWN ? RIGHT : b->d == RIGHT ? UP : DOWN;
  } else if (map[b->y][b->x] == '\\') {
    b->d = b->d == UP ? RIGHT : b->d == DOWN ? LEFT : b->d == RIGHT ? DOWN : RIGHT;
  }

  while (1) {
    tiles[b->x * rows + b->y] = 1;

    switch (b->d) {
    case UP:
      if (b->y == 0) {
        return;
      }

      if (map[b->y - 1][b->x] == '/') {
        b->d = RIGHT;
      } else if (map[b->y - 1][b->x] == '\\') {
        b->d = LEFT;
      } else if (map[b->y - 1][b->x] == '-') {
        beam_t* b1 = malloc(sizeof(beam_t));
        b1->x = b->x;
        b1->y = b->y - 1;
        b1->d = RIGHT;
        b1->next = NULL;
        queue(b1);

        beam_t* b2 = malloc(sizeof(beam_t));
        b2->x = b->x;
        b2->y = b->y - 1;
        b2->d = LEFT;
        b2->next = NULL;
        queue(b2);

        return;
      }

      b->y--;
      break;

    case DOWN:
      if (b->y == rows - 1) {
        return;
      }

      if (map[b->y + 1][b->x] == '/') {
        b->d = LEFT;
      } else if (map[b->y + 1][b->x] == '\\') {
        b->d = RIGHT;
      } else if (map[b->y + 1][b->x] == '-') {
        beam_t* b1 = malloc(sizeof(beam_t));
        b1->x = b->x;
        b1->y = b->y + 1;
        b1->d = RIGHT;
        b1->next = NULL;
        queue(b1);

        beam_t* b2 = malloc(sizeof(beam_t));
        b2->x = b->x;
        b2->y = b->y + 1;
        b2->d = LEFT;
        b2->next = NULL;
        queue(b2);

        return;
      }

      b->y++;
      break;

    case LEFT:
      if (b->x == 0) {
        return;
      }

      if (map[b->y][b->x - 1] == '/') {
        b->d = DOWN;
      } else if (map[b->y][b->x - 1] == '\\') {
        b->d = UP;
      } else if (map[b->y][b->x - 1] == '|') {
        beam_t* b1 = malloc(sizeof(beam_t));
        b1->x = b->x - 1;
        b1->y = b->y;
        b1->d = UP;
        b1->next = NULL;
        queue(b1);

        beam_t* b2 = malloc(sizeof(beam_t));
        b2->x = b->x - 1;
        b2->y = b->y;
        b2->d = DOWN;
        b2->next = NULL;
        queue(b2);

        return;
      }

      b->x--;
      break;

    case RIGHT:
      if (b->x == cols - 1) {
        return;
      }

      if (map[b->y][b->x + 1] == '/') {
        b->d = UP;
      } else if (map[b->y][b->x + 1] == '\\') {
        b->d = DOWN;
      } else if (map[b->y][b->x + 1] == '|') {
        beam_t* b1 = malloc(sizeof(beam_t));
        b1->x = b->x + 1;
        b1->y = b->y;
        b1->d = UP;
        b1->next = NULL;
        queue(b1);

        beam_t* b2 = malloc(sizeof(beam_t));
        b2->x = b->x + 1;
        b2->y = b->y;
        b2->d = DOWN;
        b2->next = NULL;
        queue(b2);

        return;
      }

      b->x++;
      break;
    }
  }
}


void do_beams() {
  while (beams_queue != NULL) {
    beam_t* beam = beams_queue;
    beams_queue = beams_queue->next;
    traverse(beam);
    free(beam);
  }
}


int max(int a, int b) {
  return a > b ? a : b;
}


int main() {
  FILE* fp = fopen("input.txt", "r");
  char line[N];
  rows = 0;

  map = malloc(sizeof(char*) * (rows + 1));

  while (fgets(line, N, fp)) {
    line[strlen(line) - 1] = '\0';
    map = realloc(map, sizeof(char*) * (rows + 1));
    map[rows++] = strdup(line);
  }

  fclose(fp);

  cols = strlen(map[0]);
  tiles = malloc(sizeof(int) * rows * cols);
  seen = malloc(sizeof(int) * rows * cols);

  for (int i = 0; i < rows * cols; i++) {
    tiles[i] = 0;
    seen[i] = 0;
  }

  beam_t* beam = malloc(sizeof(beam_t));
  beam->x = 0;
  beam->y = 0;
  beam->d = RIGHT;
  beam->next = NULL;
  queue(beam);
  do_beams();

  printf("Energized tiles (1): %d\n", count());

  int energy = 0;

  for (int i = 0; i < cols; i++) {
    beam = malloc(sizeof(beam_t));
    beam->x = i;
    beam->y = 0;
    beam->d = DOWN;
    beam->next = NULL;

    for (int k = 0; k < rows * cols; k++) {
      tiles[k] = 0;
      seen[k] = 0;
    }

    queue(beam);
    do_beams();
    energy = max(count(), energy);
  }

  for (int i = 0; i < cols; i++) {
    beam = malloc(sizeof(beam_t));
    beam->x = i;
    beam->y = rows - 1;
    beam->d = UP;
    beam->next = NULL;

    for (int k = 0; k < rows * cols; k++) {
      tiles[k] = 0;
      seen[k] = 0;
    }

    queue(beam);
    do_beams();
    energy = max(count(), energy);
  }

  for (int j = 0; j < rows; j++) {
    beam = malloc(sizeof(beam_t));
    beam->x = 0;
    beam->y = j;
    beam->d = RIGHT;
    beam->next = NULL;

    for (int k = 0; k < rows * cols; k++) {
      tiles[k] = 0;
      seen[k] = 0;
    }

    queue(beam);
    do_beams();
    energy = max(count(), energy);
  }

  for (int j = 0; j < rows; j++) {
    beam = malloc(sizeof(beam_t));
    beam->x = cols - 1;
    beam->y = j;
    beam->d = LEFT;
    beam->next = NULL;

    for (int k = 0; k < rows * cols; k++) {
      tiles[k] = 0;
      seen[k] = 0;
    }

    queue(beam);
    do_beams();
    energy = max(count(), energy);
  }

  printf("Maximum energized tiles (2): %d\n", energy);

  for (int i = 0; i < cols; i++) {
    free(map[i]);
  }

  free(map);

  return 0;
}
