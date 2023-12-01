#include <stdio.h>
#include <string.h>


int find_spelled_number(char* str) {
  if (strncmp(str, "one", 3) == 0) {
    return 1;
  } else if (strncmp(str, "two", 3) == 0) {
    return 2;
  } else if (strncmp(str, "three", 5) == 0) {
    return 3;
  } else if (strncmp(str, "four", 4) == 0) {
    return 4;
  } else if (strncmp(str, "five", 4) == 0) {
    return 5;
  } else if (strncmp(str, "six", 3) == 0) {
    return 6;
  } else if (strncmp(str, "seven", 5) == 0) {
    return 7;
  } else if (strncmp(str, "eight", 5) == 0) {
    return 8;
  } else if (strncmp(str, "nine", 4) == 0) {
    return 9;
  }

  return 0;
}


int get_first_number(char* str, int round) {
  int number;

  for (int i = 0; i < strlen(str); i++) {
    if (round == 2) {
      if ((number = find_spelled_number(str + i))) {
        return number;
      }
    }

    if ('0' <= str[i] && str[i] <= '9') {
      return str[i] - '0';
    }
  }

  return 0;
}


int get_last_number(char* str, int round) {
  int number;

  for (int i = strlen(str) - 1; i >= 0; i--) {
    if (round == 2) {
      if ((number = find_spelled_number(str + i))) {
        return number;
      }
    }

    if ('0' <= str[i] && str[i] <= '9') {
      return str[i] - '0';
    }
  }

  return 0;
}


int main() {
  char line[256];
  int results[2] = {0, 0};
  FILE* fp = fopen("input.txt", "r");

  while (fgets(line, sizeof(line), fp) != NULL) {
    for (int r = 1; r <= 2; r++) {
      results[r - 1] += 10 * get_first_number(line, r) + get_last_number(line, r);
    }
  }

  fclose(fp);

  printf("Sum of all of the calibration values (1): %d\n", results[0]);
  printf("Sum of all of the calibration values (2): %d\n", results[1]);

  return 0;
}
