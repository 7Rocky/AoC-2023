#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N 512


typedef struct room_t room_t;

struct room_t {
  char name[4];
  char left_name[4];
  char right_name[4];
  int left;
  int right;
};


void sort(room_t* rooms, int length) {
  int flag = 0;
  int j;

  for (int i = 1; i < length; i++) {
    room_t room = rooms[i];

    for (j = i - 1; j >= 0; j--) {
      if (strcmp(rooms[j].name, room.name) > 0) {
        rooms[j + 1] = rooms[j];
        flag = 1;
      } else {
        break;
      }
    }

    if (flag) {
      rooms[j + 1] = room;
    }
  }
}


int binary_search(char* name, room_t* rooms, int length) {
  int a = 0;
  int b = length;
  int m;
  int ret;

  while (a < b) {
    m = (a + b) / 2;
    ret = strcmp(name, rooms[m].name);

    if (ret < 0) {
      b = m;
    } else if (ret > 0) {
      a = m;
    } else {
      break;
    }
  }

  return m;
}


int check_all_end(room_t* rooms, int length) {
  for (int i = 0; i < length; i++) {
    if (rooms[i].name[2] != 'Z') {
      return 0;
    }
  }

  return 1;
}


long gcd(long a, long b) {
	return a % b > 0 ? gcd(b, a % b) : b;
}


long lcm(long a, long b) {
  return a * b / gcd(a, b);
}


int main() {
  FILE* fp = fopen("input.txt", "r");
  int length = 0;
  char line[N];
  room_t* rooms = malloc(sizeof(room_t) * 1);
  room_t room;

  fgets(line, N, fp);
  line[strlen(line) - 1] = '\0';
  char* moves = strdup(line);

  fgets(line, N, fp);

  while (fscanf(fp, "%3s = (%3s, %3s)\n", room.name, room.left_name, room.right_name) == 3) {
    rooms = realloc(rooms, sizeof(room_t) * (length + 1));
    rooms[length++] = room;
  }

  fclose(fp);

  sort(rooms, length);

  for (int i = 0; i < length; i++) {
    rooms[i].left = binary_search(rooms[i].left_name, rooms, length);
    rooms[i].right = binary_search(rooms[i].right_name, rooms, length);
  }

  room_t cur = rooms[0];
  long steps = 0;
  int length_moves = strlen(moves);

  for (steps = 0; strcmp(cur.name, "ZZZ"); steps++) {
    switch (moves[steps % length_moves]) {
    case 'L':
      cur = rooms[cur.left];
      break;

    case 'R':
      cur = rooms[cur.right];
      break;
    }
  }

  printf("Number of steps (1): %ld\n", steps);

  room_t* start_rooms = malloc(sizeof(room_t) * 1);
  int length_start_rooms = 0;

  for (int i = 0; i < length; i++) {
    if (rooms[i].name[2] == 'A') {
      start_rooms = realloc(start_rooms, sizeof(room_t) * (length_start_rooms + 1));
      start_rooms[length_start_rooms++] = rooms[i];
    }
  }

  int* round_steps = malloc(sizeof(int) * length_start_rooms);
  int filled = 0;

  for (steps = 0; filled < length_start_rooms; steps++) {
    for (int i = 0; i < length_start_rooms; i++) {
      if (start_rooms[i].name[2] == 'Z') {
        filled++;
        round_steps[i] = steps;
      }

      switch (moves[steps % length_moves]) {
      case 'L':
        start_rooms[i] = rooms[start_rooms[i].left];
        break;

      case 'R':
        start_rooms[i] = rooms[start_rooms[i].right];
      }
    }
  }

  steps = round_steps[0];

  for (int i = 1; i < length_start_rooms; i++) {
    steps = lcm(steps, round_steps[i]);
  }

  printf("Number of steps (2): %ld\n", steps);

  free(round_steps);
  free(start_rooms);
  free(rooms);

  return 0;
}
