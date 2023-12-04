#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N 256


int count(char c, char* str, int length) {
  int n = 0;

  for (int i = 0; i < length; i++) {
    n += (str[i] == c);
  }

  return n;
}

void find_lengths(char* line, int* length_numbers, int* length_winning_numbers) {
  int index_colon = strcspn(line, ":");
  int index_bar = strcspn(line, "|");
  *length_winning_numbers = (index_bar - index_colon) / 3;
  *length_numbers = (strlen(line) - index_bar) / 3;
}

void parse_numbers(char* line, char* delimiter, int length, int numbers[]) {
  int index_delimiter = strcspn(line, delimiter);
  line += index_delimiter + 2;

  for (int i = 0; i < length; i++) {
    while (line[0] == ' ') {
      line++;
    }

    int index_space = strcspn(line, " ");
    line[index_space] = '\0';
    numbers[i] = atoi(line);
    line[index_space] = ' ';
    line += index_space + 1;
  }
}

int find_matches(int a[], int length_a, int b[], int length_b) {
  int n = 0;

  for (int i = 0; i < length_a; i++) {
    for (int j = 0; j < length_b; j++) {
      n += (a[i] == b[j]);
    }
  }

  return n;
}

int main() {
  FILE* fp = fopen("input.txt", "r");
  int i = 0;
  int length_numbers;
  int length_winning_numbers;
  int* numbers[N];
  int* winning_numbers[N];
  char line[N];

  fgets(line, N, fp);
  find_lengths(line, &length_numbers, &length_winning_numbers);

  do {
    winning_numbers[i] = malloc(sizeof(int) * length_winning_numbers);
    numbers[i] = malloc(sizeof(int) * length_numbers);

    parse_numbers(line, ":", length_winning_numbers, winning_numbers[i]);
    parse_numbers(line, "|", length_numbers, numbers[i]);

    i++;
  } while (fgets(line, N, fp) != NULL);

  fclose(fp);

  int length_cards = i;

  int points = 0;
  int card_points[N];

  for (int i = 0; i < length_cards; i++) {
    card_points[i] = 1;
  }

  for (int i = 0; i < length_cards; i++) {
    int matches = find_matches(winning_numbers[i], length_winning_numbers, numbers[i], length_numbers);
    free(winning_numbers[i]);
    free(numbers[i]);

    points += matches ? 1 << (matches - 1) : 0;

    for (int j = i + 1; j < i + matches + 1; j++) {
      card_points[j] += card_points[i];
    }
  }

  printf("Points (1): %d\n", points);

  int total_points = 0;

  for (int i = 0; i < length_cards; i++) {
    total_points += card_points[i];
  }

  printf("Total points (2): %d\n", total_points);

  return 0;
}
