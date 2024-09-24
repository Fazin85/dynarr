#ifndef DYNARR_H
#define DYNARR_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define DYNARR_MAKE_STRUCT(type)                                               \
  typedef struct {                                                             \
    int struct_size;                                                           \
    int size;                                                                  \
    int capacity;                                                              \
    struct type *arr;                                                          \
    bool (*cmp)(struct type * t1, struct type *t2);                            \
  } _list_##type;

#define DYNARR_CREATE(type, name, cap, cmp_func)                               \
  _list_##type _##name;                                                        \
  _##name.struct_size = sizeof(struct type);                                   \
  _##name.capacity = cap == 0 ? 1 : cap;                                       \
  _##name.size = 0;                                                            \
  _##name.arr = malloc(_##name.struct_size * _##name.capacity);                \
  _##name.cmp = cmp_func;                                                      \
  _list_##type *name = &_##name;

#define _DYNARR_GROW(dar, ptr)                                                 \
  int new_capacity = dar->capacity * 2;                                        \
  typeof(*ptr) *new_array = malloc(new_capacity * dar->struct_size);           \
  memcpy(new_array, dar->arr, dar->size * dar->struct_size);                   \
  free(dar->arr);                                                              \
  dar->capacity = new_capacity;                                                \
  dar->arr = new_array;

#define _DYNARR_SHRINK(dar, ptr)                                               \
  int new_capacity = dar->capacity / 2;                                        \
  typeof(*ptr) *new_array = malloc(new_capacity * dar->struct_size);           \
  memcpy(new_array, dar->arr, dar->size * dar->struct_size);                   \
  free(dar->arr);                                                              \
  dar->capacity = new_capacity;                                                \
  dar->arr = new_array;

#define DYNARR_ADD(dar, value)                                                 \
  if (dar->size >= dar->capacity) {                                            \
    _DYNARR_GROW(dar, value);                                                  \
  }                                                                            \
  memcpy(&dar->arr[dar->size], value, dar->struct_size);                       \
  dar->size++;

#define DYNARR_INSERT(dar, value, index)                                       \
  if (dar->size >= dar->capacity) {                                            \
    _DYNARR_GROW(dar, value);                                                  \
  }                                                                            \
  for (int i = dar->size; i > index; i--) {                                    \
    memcpy(&dar->arr[i], &dar->arr[i - 1], dar->struct_size);                  \
  }                                                                            \
  memcpy(&dar->arr[index], value, dar->struct_size);                           \
  dar->size++;

#define DYNARR_REMOVE(dar, value)                                              \
  for (int i = 0; i < dar->size; i++) {                                        \
    if (dar->cmp(&dar->arr[i], value)) {                                       \
      for (int j = 0; j < (dar->size - i - 1); j++) {                          \
        memcpy(&dar->arr[i + j], &dar->arr[i + j + 1], dar->struct_size);      \
      }                                                                        \
      memset(&dar->arr[dar->size - 1], 0, dar->struct_size);                   \
      dar->size--;                                                             \
      if (dar->size <= (int)(dar->capacity / 3)) {                             \
        _DYNARR_SHRINK(dar, value);                                            \
      }                                                                        \
      break;                                                                   \
    }                                                                          \
  }

#define DYNARR_REMOVE_AT(dar, index)                                           \
  for (int j = 0; j < (dar->size - index - 1); j++) {                          \
    memcpy(&dar->arr[index + j], &dar->arr[index + j + 1], dar->struct_size);  \
  }                                                                            \
  memset(&dar->arr[dar->size - 1], 0, dar->struct_size);                       \
  dar->size--;                                                                 \
  if (dar->size <= (int)(dar->capacity / 3)) {                                 \
    _DYNARR_SHRINK(dar, &dar->arr[0]);                                         \
  }

#define DYNARR_SEARCH(dar, value, result)                                      \
  for (int i = 0; i < dar->size; i++) {                                        \
    if (dar->cmp(&dar->arr[i], value)) {                                       \
      *result = i;                                                             \
    }                                                                          \
  }

#define DYNARR_IS_EMPTY(dar) dar->size == 0;

#define DYNARR_DESTROY(dar) free(dar->arr);

#endif
