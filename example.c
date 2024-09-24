#include "dynarr.h"
#include <stdio.h>

struct my_struct {
  int x;
  int y;
  int id;
};

bool my_struct_cmp(struct my_struct *p1, struct my_struct *p2) {
  return p1->x == p2->x && p1->y == p2->y && p1->id == p2->id;
}

int main() {
  DYNARR_MAKE_STRUCT(my_struct);

  DYNARR_CREATE(my_struct, my_dynamic_array, 10, my_struct_cmp);

  struct my_struct m1 = {.x = 10, .y = 30, .id = 18173121};
  struct my_struct m2 = {.x = 10, .y = 45, .id = 85};

  struct my_struct m3 = {.x = 10, .y = 30, .id = 18173121};

  DYNARR_ADD(my_dynamic_array, &m1);
  DYNARR_ADD(my_dynamic_array, &m2);
  DYNARR_ADD(my_dynamic_array, &m3);

  DYNARR_REMOVE(my_dynamic_array, &m1);
  DYNARR_REMOVE_AT(my_dynamic_array, 2);

  for (int i = 0; i < my_dynamic_array->size; i++) {
    printf("%d\n", my_dynamic_array->arr[i].id);
  }

  DYNARR_DESTROY(my_dynamic_array);

  return 0;
}