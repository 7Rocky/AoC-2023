#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/time.h>


char* RESET = "\033[0m";

char* RED_BACKGROUND = "\033[41m";
char* GREEN_BACKGROUND = "\033[42m";

char* RED_BOLD_BRIGHT = "\033[1;91m";
char* GREEN_BOLD_BRIGHT = "\033[1;92m";
char* YELLOW_BOLD_BRIGHT = "\033[1;93m";
char* WHITE_BOLD_BRIGHT = "\033[1;97m";

char* answer1 = "Reflections score (1): 27505";
char* answer2 = "Patched reflections score (2): 22906";


void print_pass_message() {
  printf("%s%s PASS %s\n", WHITE_BOLD_BRIGHT, GREEN_BACKGROUND, RESET);
}


void print_fail_message() {
  printf("%s%s FAIL %s\n", WHITE_BOLD_BRIGHT, RED_BACKGROUND, RESET);
}


void print_time(double t) {
  printf("%s Time %s %.3f s%s\n", WHITE_BOLD_BRIGHT, (t < 3 ? GREEN_BOLD_BRIGHT : t < 20 ? YELLOW_BOLD_BRIGHT : RED_BOLD_BRIGHT), t, RESET);
}

double current_time() {
  struct timeval current_time;
  gettimeofday(&current_time, NULL);
  return (current_time.tv_sec * 1000000 + current_time.tv_usec) / 1000000.0;
}


int main(int argc, char** argv) {
  if (argc == 2 && strncmp(argv[1], "time", 4) == 0) {
    printf("%.3lf\n", current_time());
    return 0;
  }

  char outputs[3][256];

  FILE* fp = fopen("output.txt", "r");

  for (int i = 0; i < 3; i++) {
    fgets(outputs[i], sizeof(outputs[i]), fp);
    outputs[i][strlen(outputs[i]) - 1] = '\0';
  }

  fclose(fp);

  double init;
  sscanf(outputs[0], "%lf", &init);
  double t = current_time() - init;
  int pass = 0;

  if (strncmp(outputs[1], answer1, strlen(answer1)) == 0) {
    pass++;
  }

  if (strncmp(outputs[2], answer2, strlen(answer2)) == 0) {
    pass++;
  }

  if (pass == 2) {
    print_pass_message();
  } else {
    print_fail_message();
  }

  print_time(t);

  return 0;
}
