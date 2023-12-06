#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N 256


typedef struct {
  unsigned long dst;
  unsigned long src;
  unsigned long len;
} map_t;

typedef struct {
  unsigned long seed;
  unsigned long size;
} range_t;


FILE* fp;

int length_seed_soil            = 0;
int length_soil_fertilizer      = 0;
int length_fertilizer_water     = 0;
int length_water_light          = 0;
int length_light_temperature    = 0;
int length_temperature_humidity = 0;
int length_humidity_location    = 0;

map_t* seed_soil;
map_t* soil_fertilizer;
map_t* fertilizer_water;
map_t* water_light;
map_t* light_temperature;
map_t* temperature_humidity;
map_t* humidity_location;


int count(char c, char* str, int length) {
  int n = 0;

  for (int i = 0; i < length; i++) {
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


void fill_numbers(char* line, unsigned long numbers[]) {
  int j = 0;
  int index_space;

  for (int i = 0; i < strlen(line); i++) {
    if ((index_space = index_of(line + i, ' ')) == -1) {
      numbers[j++] = atol(line + i);
      break;
    }

    line[i + index_space] = '\0';
    numbers[j++] = atol(line + i);
    line[i + index_space] = ' ';
    i += index_space;
  }
}


map_t* get_map(int* size) {
  int i = 0;
  char line[N];
  map_t* map = malloc(sizeof(map_t) * 1);
  unsigned long map_n[3];

  fgets(line, N, fp);
  fgets(line, N, fp);

  do {
    if (feof(fp)) {
      break;
    }

    fill_numbers(line, map_n);
    fgets(line, N, fp);
    map = realloc(map, sizeof(map_t) * (i + 1));
    map[i++] = (map_t) { .dst = map_n[0], .src = map_n[1], .len = map_n[2] };
  } while (strlen(line) > 1);

  *size = i;
  return map;
}


unsigned long find_next(unsigned long src, map_t map[], int length) {
  for (int i = 0; i < length; i++) {
    if (map[i].src <= src && src < map[i].src + map[i].len) {
      src += map[i].dst - map[i].src;
      break;
    }
  }

  return src;
}


unsigned long find_location(unsigned long seed) {
  seed = find_next(seed, seed_soil,            length_seed_soil);
  seed = find_next(seed, soil_fertilizer,      length_soil_fertilizer);
  seed = find_next(seed, fertilizer_water,     length_fertilizer_water);
  seed = find_next(seed, water_light,          length_water_light);
  seed = find_next(seed, light_temperature,    length_light_temperature);
  seed = find_next(seed, temperature_humidity, length_temperature_humidity);
  return find_next(seed, humidity_location,    length_humidity_location);
}


unsigned long min(unsigned long a, unsigned long b) {
  return a < b ? a : b;
}


void* find_min_location_range(void* p_range) {
  range_t* range = p_range;
  unsigned long min_location = find_location(range->seed);

  for (int j = 1; j < range->size; j++) {
    min_location = min(find_location(range->seed + j), min_location);
  }

  free(p_range);

  return (void*) min_location;
}


int main() {
  char line[N];

  fp = fopen("input.txt", "r");
  fgets(line, N, fp);

  int length_seeds = count(' ', line, strlen(line));
  unsigned long* seeds = malloc(sizeof(unsigned long) * length_seeds);

  fill_numbers(line + index_of(line, ' ') + 1, seeds);
  fgets(line, N, fp);

  seed_soil            = get_map(&length_seed_soil);
  soil_fertilizer      = get_map(&length_soil_fertilizer);
  fertilizer_water     = get_map(&length_fertilizer_water);
  water_light          = get_map(&length_water_light);
  light_temperature    = get_map(&length_light_temperature);
  temperature_humidity = get_map(&length_temperature_humidity);
  humidity_location    = get_map(&length_humidity_location);

  fclose(fp);

  unsigned long min_location = find_location(seeds[0]);

  for (int i = 1; i < length_seeds; i++) {
    min_location = min(find_location(seeds[i]), min_location);
  }

  printf("Minimum location (1): %lu\n", min_location);

  unsigned long* min_locations = malloc(sizeof(unsigned long) * length_seeds / 2);
  pthread_t* threads = malloc(sizeof(pthread_t) * length_seeds / 2);
  range_t* range;

  for (int i = 0; i < length_seeds; i += 2) {
    range = malloc(sizeof(range_t));
    *range = (range_t) { .seed = seeds[i], .size = seeds[i + 1] };
    pthread_create(&threads[i / 2], NULL, find_min_location_range, range);
  }

  for (int i = 0; i < length_seeds / 2; i++) {
    pthread_join(threads[i], (void**) &min_locations[i]);
  }

  min_location = min_locations[0];

  for (int i = 1; i < length_seeds / 2; i++) {
    min_location = min(min_locations[i], min_location);
  }

  printf("Minimum location with ranges (2): %lu\n", min_location);

  free(seeds);
  free(min_locations);
  free(threads);
  free(seed_soil);
  free(soil_fertilizer);
  free(fertilizer_water);
  free(water_light);
  free(light_temperature);
  free(temperature_humidity);
  free(humidity_location);

  return 0;
}
