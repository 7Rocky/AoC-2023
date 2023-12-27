#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N 256

typedef enum { UNRATED, REJECTED, ACCEPTED } rate_t;

typedef struct {
  char var;
  int bound;
  int lt;
  char* next;
} rule_t;

typedef struct {
  char* name;
  rule_t* rules[4];
} workflow_t;

typedef struct {
  int x;
  int m;
  int a;
  int s;
  rate_t state;
} part_t;

typedef struct workflow_stack_t workflow_stack_t;

struct workflow_stack_t {
  workflow_t* workflow;
  int x[2];
  int m[2];
  int a[2];
  int s[2];
  workflow_stack_t* next;
};


int num_workflows = 2;
workflow_t** workflows;
workflow_stack_t* workflow_stack;


rule_t* parse_rule(char* line) {
  rule_t* rule = malloc(sizeof(rule_t));

  if (strchr(line, ':')) {
    rule->var = line[0];
    rule->next = strdup(line + strcspn(line, ":") + 1);
    rule->lt = strchr(line, '<') != NULL;
    rule->bound = atoi(line + strcspn(line, rule->lt ? "<" : ">") + 1);
  } else {
    rule->var = '\0';
    rule->next = strndup(line, strlen(line) - 1);
  }

  return rule;
}


workflow_t* parse_workflow(char* line) {
  workflow_t* workflow = malloc(sizeof(workflow_t));
  char* s;
  int i = 0;

  int brace_index = strcspn(line, "{");
  workflow->name = strndup(line, brace_index);

  for (s = strtok(line + brace_index + 1, ","); s; s = strtok(NULL, ",")) {
    workflow->rules[i++] = parse_rule(s);
  }

  return workflow;
}


part_t* parse_part(char* line) {
  part_t* part = malloc(sizeof(part_t));
  char* s;
  int i;

  part->state = UNRATED;

  for (s = strtok(line, ","), i = 0; s; s = strtok(NULL, ",")) {
    int index_equals = strcspn(s, "=");

    switch (s[index_equals - 1]) {
    case 'x':
      part->x = atoi(s + index_equals + 1);
      break;

    case 'm':
      part->m = atoi(s + index_equals + 1);
      break;

    case 'a':
      part->a = atoi(s + index_equals + 1);
      break;

    case 's':
      part->s = atoi(s + index_equals + 1);
      break;
    }
  }

  return part;
}


void sort_workflows() {
  int flag = 0;
  int j;

  for (int i = 1; i < num_workflows; i++) {
    workflow_t* workflow = workflows[i];

    for (j = i - 1; j >= 0; j--) {
      if (strcmp(workflows[j]->name, workflow->name) > 0) {
        workflows[j + 1] = workflows[j];
        flag = 1;
      } else {
        break;
      }
    }

    if (flag) {
      workflows[j + 1] = workflow;
    }
  }
}


int find_workflow(char* name) {
  int a = 0;
  int b = num_workflows;
  int m;
  int ret;

  while (a < b) {
    m = (a + b) / 2;
    ret = strcmp(name, workflows[m]->name);

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


void rate(part_t* part) {
  workflow_t* workflow;
  int i = find_workflow("in");

  while (part->state == UNRATED) {
    if (i >= 2) {
      workflow = workflows[i];
    } else {
      part->state = i == 0 ? ACCEPTED : REJECTED;
      return;
    }

    for (int k = 0; k < 4; k++) {
      if (workflow->rules[k]) {
        if (workflow->rules[k]->var == 'x') {
          if (workflow->rules[k]->lt) {
            if (part->x < workflow->rules[k]->bound) {
              i = find_workflow(workflow->rules[k]->next);
              break;
            }
          } else {
            if (part->x > workflow->rules[k]->bound) {
              i = find_workflow(workflow->rules[k]->next);
              break;
            }
          }
        } else if (workflow->rules[k]->var == 'm') {
          if (workflow->rules[k]->lt) {
            if (part->m < workflow->rules[k]->bound) {
              i = find_workflow(workflow->rules[k]->next);
              break;
            }
          } else {
            if (part->m > workflow->rules[k]->bound) {
              i = find_workflow(workflow->rules[k]->next);
              break;
            }
          }
        } else if (workflow->rules[k]->var == 'a') {
          if (workflow->rules[k]->lt) {
            if (part->a < workflow->rules[k]->bound) {
              i = find_workflow(workflow->rules[k]->next);
              break;
            }
          } else {
            if (part->a > workflow->rules[k]->bound) {
              i = find_workflow(workflow->rules[k]->next);
              break;
            }
          }
        } else if (workflow->rules[k]->var == 's') {
          if (workflow->rules[k]->lt) {
            if (part->s < workflow->rules[k]->bound) {
              i = find_workflow(workflow->rules[k]->next);
              break;
            }
          } else {
            if (part->s > workflow->rules[k]->bound) {
              i = find_workflow(workflow->rules[k]->next);
              break;
            }
          }
        } else {
          if (strcmp(workflow->rules[k]->next, "A") == 0) {
            part->state = ACCEPTED;
            return;
          } else if (strcmp(workflow->rules[k]->next, "R") == 0) {
            part->state = REJECTED;
            return;
          } else {
            i = find_workflow(workflow->rules[k]->next);
            break;
          }
        }
      }
    }
  }
}


void stack(workflow_t* workflow, int x[], int m[], int a[], int s[]) {
  workflow_stack_t* ws = workflow_stack;
  workflow_stack_t* w = malloc(sizeof(workflow_stack_t));

  w->workflow = workflow;
  w->x[0] = x[0];
  w->x[1] = x[1];
  w->m[0] = m[0];
  w->m[1] = m[1];
  w->a[0] = a[0];
  w->a[1] = a[1];
  w->s[0] = s[0];
  w->s[1] = s[1];

  if (ws != NULL) {
    w->next = ws;
    workflow_stack = w;
  } else {
    workflow_stack = w;
  }
}


int min(int a, int b) {
  return a < b ? a : b;
}


int max(int a, int b) {
  return a > b ? a : b;
}


int main() {
  FILE* fp = fopen("input.txt", "r");
  char line[N];

  workflows = malloc(sizeof(workflow_t*) * num_workflows);

  workflows[0] = malloc(sizeof(workflow_t));
  workflows[1] = malloc(sizeof(workflow_t));

  workflows[0]->name = strdup("A");
  workflows[1]->name = strdup("R");

  while (fgets(line, N, fp)) {
    line[strlen(line) - 1] = '\0';

    if (strlen(line) == 0) {
      break;
    }

    workflows = realloc(workflows, sizeof(workflow_t*) * (num_workflows + 1));
    workflows[num_workflows++] = parse_workflow(line);
  }

  sort_workflows();

  int num_parts = 0;
  part_t** parts = malloc(sizeof(part_t*) * (num_parts + 1));

  while (fgets(line, N, fp)) {
    line[strlen(line) - 1] = '\0';

    parts = realloc(parts, sizeof(part_t*) * (num_parts + 1));
    parts[num_parts++] = parse_part(strndup(line + 1, strlen(line) - 2));
  }

  fclose(fp);

  for (int i = 0; i < num_parts; i++) {
    rate(parts[i]);
  }

  unsigned long result = 0;

  for (int i = 0; i < num_parts; i++) {
    if (parts[i]->state == ACCEPTED) {
      result += parts[i]->x + parts[i]->m + parts[i]->a + parts[i]->s;
    }
  }

  printf("Result (1): %lu\n", result);

  for (int i = 0; i < num_parts; i++) {
    free(parts[i]);
  }

  free(parts);

  result = 0;

  int x[2] = {1, 4000};
  int m[2] = {1, 4000};
  int a[2] = {1, 4000};
  int s[2] = {1, 4000};
  int xt[2];
  int mt[2];
  int at[2];
  int st[2];

  stack(workflows[find_workflow("in")], x, m, a, s);
  workflow_stack_t* prev = NULL;

  while (workflow_stack) {
    workflow_t* workflow = workflow_stack->workflow;

    x[0] = workflow_stack->x[0];
    x[1] = workflow_stack->x[1];
    m[0] = workflow_stack->m[0];
    m[1] = workflow_stack->m[1];
    a[0] = workflow_stack->a[0];
    a[1] = workflow_stack->a[1];
    s[0] = workflow_stack->s[0];
    s[1] = workflow_stack->s[1];
    prev = workflow_stack;
    workflow_stack = workflow_stack->next;
    free(prev);

    if (strcmp(workflow->name, "R") == 0) {
      continue;
    }

    if (strcmp(workflow->name, "A") == 0) {
      result += (x[1] - x[0] + 1UL) * (m[1] - m[0] + 1UL) * (a[1] - a[0] + 1UL) * (s[1] - s[0] + 1UL);
      continue;
    }

    for (int k = 0; k < 4; k++) {
      xt[0] = x[0];
      xt[1] = x[1];
      mt[0] = m[0];
      mt[1] = m[1];
      at[0] = a[0];
      at[1] = a[1];
      st[0] = s[0];
      st[1] = s[1];

      if (workflow->rules[k]) {
        if (workflow->rules[k]->var == 'x') {
          if (workflow->rules[k]->lt) {
            x[1] = min(x[1], workflow->rules[k]->bound - 1);
          } else {
            x[0] = max(x[0], workflow->rules[k]->bound + 1);
          }

          stack(workflows[find_workflow(workflow->rules[k]->next)], x, m, a, s);

          if (workflow->rules[k]->lt) {
            x[0] = max(xt[0], workflow->rules[k]->bound);
            x[1] = xt[1];
          } else {
            x[0] = xt[0];
            x[1] = min(xt[1], workflow->rules[k]->bound);
          }
        } else if (workflow->rules[k]->var == 'm') {
          if (workflow->rules[k]->lt) {
            m[1] = min(m[1], workflow->rules[k]->bound - 1);
          } else {
            m[0] = max(m[0], workflow->rules[k]->bound + 1);
          }

          stack(workflows[find_workflow(workflow->rules[k]->next)], x, m, a, s);

          if (workflow->rules[k]->lt) {
            m[0] = max(mt[0], workflow->rules[k]->bound);
            m[1] = mt[1];
          } else {
            m[0] = mt[0];
            m[1] = min(mt[1], workflow->rules[k]->bound);
          }
        } else if (workflow->rules[k]->var == 'a') {
          if (workflow->rules[k]->lt) {
            a[1] = min(a[1], workflow->rules[k]->bound - 1);
          } else {
            a[0] = max(a[0], workflow->rules[k]->bound + 1);
          }

          stack(workflows[find_workflow(workflow->rules[k]->next)], x, m, a, s);

          if (workflow->rules[k]->lt) {
            a[0] = max(at[0], workflow->rules[k]->bound);
            a[1] = at[1];
          } else {
            a[0] = at[0];
            a[1] = min(at[1], workflow->rules[k]->bound);
          }
        } else if (workflow->rules[k]->var == 's') {
          if (workflow->rules[k]->lt) {
            s[1] = min(s[1], workflow->rules[k]->bound - 1);
          } else {
            s[0] = max(s[0], workflow->rules[k]->bound + 1);
          }

          stack(workflows[find_workflow(workflow->rules[k]->next)], x, m, a, s);

          if (workflow->rules[k]->lt) {
            s[0] = max(st[0], workflow->rules[k]->bound);
            s[1] = st[1];
          } else {
            s[0] = st[0];
            s[1] = min(st[1], workflow->rules[k]->bound);
          }
        } else {
          stack(workflows[find_workflow(workflow->rules[k]->next)], x, m, a, s);
        }
      }
    }
  }

  printf("Possibilities (2): %lu\n", result);

  for (int i = 0; i < num_workflows; i++) {
    free(workflows[i]->name);

    for (int k = 0; k < 4; k++) {
      if (workflows[i]->rules[k]) {
        free(workflows[i]->rules[k]->next);
        free(workflows[i]->rules[k]);
      }
    }

    free(workflows[i]);
  }

  free(workflows);

  return 0;
}
