#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N 256


int index_of(char* str, char c) {
  for (int i = 0; i < strlen(str); i++) {
    if (str[i] == c) {
      return i;
    }
  }

  return -1;
}


long* get_numbers(char* line, int* length) {
  int j = 0;
  int index_space;
  long* numbers = malloc(sizeof(long) * 1);

  for (int i = 0; i < strlen(line); i++) {
    while (line[++i] == ' ');

    numbers = realloc(numbers, sizeof(long) * (j + 1));

    if ((index_space = index_of(line + i, ' ')) == -1) {
      numbers[j++] = atoi(line + i);
      break;
    }

    line[i + index_space] = '\0';
    numbers[j++] = atol(line + i);
    line[i + index_space] = ' ';
    i += index_space;
  }

  *length = j;
  return numbers;
}


long get_number_without_spaces(char* line) {
  char str[N];
  int j = 0;

  for (int i = 0; i < strlen(line); i++) {
    if (line[i] != ' ') {
      str[j++] = line[i];
    }
  }

  str[j] = '\0';

  return atol(str);
}


long find_winning_ways(long time, long distance) {
  long wins = 0;

  for (int i = 0; i < time; i++) {
    if ((time - i) * i > distance) {
      wins++;
    }
  }

  return wins;
}


int main() {
  char line[N];
  int length = 0;
  FILE* fp = fopen("input.txt", "r");

  fgets(line, N, fp);
  int index = index_of(line, ' ') + 1;
  long* times = get_numbers(line + index, &length);
  long final_time = get_number_without_spaces(line + index);

  fgets(line, N, fp);
  index = index_of(line, ' ') + 1;
  long* distances = get_numbers(line + index, &length);
  long final_distance = get_number_without_spaces(line + index);

  fclose(fp);

  long result = 1;

  for (int i = 0; i < length; i++) {
    result *= find_winning_ways(times[i], distances[i]);
  }

  printf("Product of winning ways (1): %ld\n", result);

  free(times);
  free(distances);

  result = find_winning_ways(final_time, final_distance);
  printf("Product of winning ways of final race (2): %ld\n", result);

  return 0;
}
