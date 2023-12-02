#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N_GAMES 100


typedef struct {
  int red;
  int green;
  int blue;
} set;

typedef struct {
  int num_sets;
  set* sets;
} game;


int count(char* str, char c) {
  int n = 0;

  for (int i = 0; i < strlen(str); i++) {
    n += (str[i] == c);
  }

  return n;
}


int index_of(char* str, char c) {
  for (int i = 0; i < strlen(str); i++) {
    if (str[i] == c) {
      return i;
    }
  }

  return -1;
}


int find_number_for(char* id, char* str, int length) {
  int id_length = strlen(id);
  int num = 0;

  if (length == -1) {
    length = strlen(str);
  }

  for (int i = 0; i < length; i++) {
    if (strncmp(str + i, id, id_length) == 0) {
      str[i - 1] = '\0';
      int j = i - 2;

      while (str[j] != ' ' && j != 0) {
        j--;
      }

      num = atoi(str + j);
      str[i - 1] = ' ';
      break;
    }
  }

  return num;
}


set parse_set(char* str, int length) {
  set s = {
    .red = find_number_for("red", str, length),
    .green = find_number_for("green", str, length),
    .blue = find_number_for("blue", str, length)
  };

  return s;
}


game* parse_game(char* line) {
  int num_sets = count(line, ';') + 1;
  set* sets = malloc(sizeof(set) * num_sets);
  int start = index_of(line, ':') + 2;
  int length = index_of(line, ';') - start;
  game* g = malloc(sizeof(game*));

  for (int i = 0; i < num_sets; i++) {
    sets[i] = parse_set(line + start, length);
    start += length + 2;
    length = index_of(line + start, ';');
  }

  g->num_sets = num_sets;
  g->sets = sets;

  return g;
}


int main() {
  char line[256];
  game* games[N_GAMES];
  FILE* fp = fopen("input.txt", "r");
  int i = 0;

  while (fgets(line, sizeof(line), fp) != NULL) {
    games[i++] = parse_game(line);
  }

  fclose(fp);

  int result = 0;
  int is_game_valid = 0;
  set target = { .red = 12, .green = 13, .blue = 14 };

  for (int i = 0; i < N_GAMES; i++) {
    is_game_valid = 0;

    for (int j = 0; j < games[i]->num_sets; j++) {
      if (target.red - games[i]->sets[j].red >= 0 && target.green - games[i]->sets[j].green >= 0 && target.blue - games[i]->sets[j].blue >= 0) {
        is_game_valid++;
      }
    }

    if (is_game_valid == games[i]->num_sets) {
      result += i + 1;
    }
  }

  printf("Sum of valid game IDs (1): %d\n", result);

  int sum_powers = 0;
  int min_red;
  int min_green;
  int min_blue;

  for (int i = 0; i < N_GAMES; i++) {
    min_red = -1;
    min_green = -1;
    min_blue = -1;

    for (int j = 0; j < games[i]->num_sets; j++) {
      min_red = games[i]->sets[j].red > min_red ? games[i]->sets[j].red : min_red;
      min_green = games[i]->sets[j].green > min_green ? games[i]->sets[j].green : min_green;
      min_blue = games[i]->sets[j].blue > min_blue ? games[i]->sets[j].blue : min_blue;
    }

    sum_powers += min_red * min_green * min_blue;
  }

  printf("Sum of set powers (2): %d\n", sum_powers);

  return 0;
}
