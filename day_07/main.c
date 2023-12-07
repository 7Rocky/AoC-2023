#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N 256
#define NUM_CARDS 5

typedef enum {
  HIGH_CARD,
  ONE_PAIR,
  TWO_PAIR,
  THREE_OF_A_KIND,
  FULL_HOUSE,
  FOUR_OF_A_KIND,
  FIVE_OF_A_KIND,
} kind_t;

typedef struct {
  char cards[NUM_CARDS + 1];
  char unsorted_cards[NUM_CARDS + 1];
  int bid;
  kind_t kind;
} hand_t;


char* CARDS_PART1 = "23456789TJQKA";
char* CARDS_PART2 = "J23456789TQKA";


int index_of(char* str, char c) {
  for (int i = 0; i < strlen(str); i++) {
    if (str[i] == c) {
      return i;
    }
  }

  return -1;
}


int count(char* str, char c) {
  int n = 0;

  for (int i = 0; i < strlen(str); i++) {
    n += (str[i] == c);
  }

  return n;
}


void sort_cards(char* cards) {
  int flag = 0;
  int j;

  for (int i = 1; i < strlen(cards); i++) {
    char card = cards[i];

    for (j = i - 1; j >= 0; j--) {
      if (cards[j] > card) {
        cards[j + 1] = cards[j];
        flag = 1;
      } else {
        break;
      }
    }

    if (flag) {
      cards[j + 1] = card;
    }
  }
}


int compare(hand_t a, hand_t b, int part) {
  if (a.kind != b.kind) {
    return a.kind > b.kind;
  }

  char* CARDS = CARDS_PART1;

  if (part == 2) {
    CARDS = CARDS_PART2;
  }

  for (int i = 0; i < 5; i++) {
    if (a.unsorted_cards[i] != b.unsorted_cards[i]) {
      return index_of(CARDS, a.unsorted_cards[i]) > index_of(CARDS, b.unsorted_cards[i]);
    }
  }

  return 0;
}


void sort_hands(hand_t* hands, int length, int part) {
  int flag = 0;
  int j;

  for (int i = 1; i < length; i++) {
    hand_t hand = hands[i];

    for (j = i - 1; j >= 0; j--) {
      if (compare(hands[j], hand, part) > 0) {
        hands[j + 1] = hands[j];
        flag = 1;
      } else {
        break;
      }
    }

    if (flag) {
      hands[j + 1] = hand;
    }
  }
}


void classify_with_joker(hand_t* hand) {
  char* c = hand->cards;

  switch (count(c, 'J')) {
  case 5:
  case 4:
    hand->kind = FIVE_OF_A_KIND;
    break;

  case 3:
    if ((c[1] == 'J' && c[3] == c[4]) || (c[3] == 'J' && c[0] == c[1])) {
      hand->kind = FIVE_OF_A_KIND;
    } else {
      hand->kind = FOUR_OF_A_KIND;
    }

    break;

  case 2:
    if ((c[0] == 'J' && c[2] == c[3] && c[3] == c[4]) || (c[3] == 'J' && c[0] == c[1] && c[1] == c[2])) {
      hand->kind = FIVE_OF_A_KIND;
    } else if ((c[0] == 'J' && (c[2] == c[3] || c[3] == c[4])) || (c[1] == 'J' && c[3] == c[4]) || (c[2] == 'J' && c[0] == c[1]) || (c[3] == 'J' && (c[0] == c[1] || c[1] == c[2]))) {
      hand->kind = FOUR_OF_A_KIND;
    } else {
      hand->kind = THREE_OF_A_KIND;
    }

    break;

  case 1:
    if ((c[0] == 'J' && c[1] == c[2] && c[2] == c[3] && c[3] == c[4]) || (c[4] == 'J' && c[0] == c[1] && c[1] == c[2] && c[2] == c[3])) {
      hand->kind = FIVE_OF_A_KIND;
    } else if ((c[0] == 'J' && c[2] == c[3] && (c[1] == c[2] || c[3] == c[4])) || (c[1] == 'J' && c[2] == c[3] && c[3] == c[4]) || (c[3] == 'J' && c[0] == c[1] && c[1] == c[2]) || (c[4] == 'J' && c[1] == c[2] && (c[0] == c[1] || c[2] == c[3]))) {
      hand->kind = FOUR_OF_A_KIND;
    } else if ((c[0] == 'J' && c[1] == c[2] && c[3] == c[4]) || (c[2] == 'J' && c[0] == c[1] && c[3] == c[4]) || (c[4] == 'J' && c[0] == c[1] && c[2] == c[3])) {
      hand->kind = FULL_HOUSE;
    } else if ((c[0] == 'J' && (c[1] == c[2] || c[2] == c[3] || c[3] == c[4])) || (c[1] == 'J' && (c[2] == c[3] || c[3] == c[4])) || (c[2] == 'J' && (c[0] == c[1] || c[3] == c[4])) || (c[3] == 'J' && (c[0] == c[1] || c[1] == c[2])) || (c[4] == 'J' && (c[0] == c[1] || c[1] == c[2] || c[2] == c[3]))) {
      hand->kind = THREE_OF_A_KIND;
    } else {
      hand->kind = ONE_PAIR;
    }
  }
}


void classify(hand_t* hand, int part) {
  char* c = hand->cards;

  sort_cards(c);

  if (part == 2 && index_of(c, 'J') != -1) {
    classify_with_joker(hand);
    return;
  }

  if (c[0] == c[1] && c[1] == c[2] && c[2] == c[3] && c[3] == c[4]) {
    hand->kind = FIVE_OF_A_KIND;
  } else if (c[1] == c[2] && c[2] == c[3] && (c[0] == c[1] || c[3] == c[4])) {
    hand->kind = FOUR_OF_A_KIND;
  } else if (c[0] == c[1] && c[3] == c[4] && (c[1] == c[2] || c[2] == c[3])) {
    hand->kind = FULL_HOUSE;
  } else if ((c[0] == c[1] && c[1] == c[2]) || (c[1] == c[2] && c[2] == c[3]) || (c[2] == c[3] && c[3] == c[4])) {
    hand->kind = THREE_OF_A_KIND;
  } else if ((c[0] == c[1] && c[2] == c[3]) || (c[0] == c[1] && c[3] == c[4]) || (c[1] == c[2] && c[3] == c[4])) {
    hand->kind = TWO_PAIR;
  } else if (c[0] == c[1] || c[1] == c[2] || c[2] == c[3] || c[3] == c[4]) {
    hand->kind = ONE_PAIR;
  } else {
    hand->kind = HIGH_CARD;
  }
}


int solve(hand_t hands[], int length, int part) {
  for (int i = 0; i < length; i++) {
    classify(&hands[i], part);
  }

  sort_hands(hands, length, part);

  int result = 0;

  for (int i = 0; i < length; i++) {
    result += (i + 1) * hands[i].bid;
  }

  return result;
}


int main() {
  FILE* fp = fopen("input.txt", "r");
  hand_t* hands = malloc(sizeof(hand_t) * 1);
  int length = 0;
  char line[N];
  hand_t hand;

  while (fscanf(fp, "%5s %d\n", hand.cards, &hand.bid) != -1) {
    strncpy(hand.unsorted_cards, hand.cards, sizeof(hand.cards));
    hands = realloc(hands, sizeof(hand_t) * (length + 1));
    hands[length++] = hand;
  }

  fclose(fp);

  printf("Total winnings (1): %d\n", solve(hands, length, 1));
  printf("Total winnings (2): %d\n", solve(hands, length, 2));

  free(hands);

  return 0;
}
