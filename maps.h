// maps.h
#include "assert.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"

#define DEFINE_MAP_IMPLEMENTATION(KeyT, ValueT, MapType)                       \
  struct MapType##_struct {                                                    \
    size_t count;                                                              \
    size_t capacity;                                                           \
    KeyT* keys;                                                                \
    ValueT* values;                                                            \
  };                                                                           \
  MapType* MapType##_create() {                                                \
    MapType* map = (MapType*)malloc(sizeof(MapType));                          \
    map->count = 0;                                                            \
    map->capacity = 1;                                                         \
    map->values = malloc(map->capacity * sizeof(ValueT));                      \
    map->keys = malloc(map->capacity * sizeof(KeyT));                          \
    return map;                                                                \
  }                                                                            \
  void MapType##_destroy(MapType* map) {                                       \
    free(map->values);                                                         \
    free(map->keys);                                                           \
    free(map);                                                                 \
  }                                                                            \
  static int32_t _##MapType##_search(MapType* map, KeyT key) {                 \
    for (size_t i = 0; i < map->count; i++) {                                  \
      if (map->keys[i] == key) {                                               \
        return i;                                                              \
      }                                                                        \
    }                                                                          \
    return -1;                                                                 \
  }                                                                            \
  ValueT MapType##_lookup(MapType* map, KeyT key) {                            \
    int32_t index = _##MapType##_search(map, key);                             \
    assert(index != -1);                                                       \
    return map->values[index];                                                 \
  }                                                                            \
  bool MapType##_contains(MapType* map, KeyT key) {                            \
    int32_t index = _##MapType##_search(map, key);                             \
    return index != -1;                                                        \
  }                                                                            \
  void MapType##_put(MapType* map, KeyT key, ValueT value) {                   \
    int32_t index = _##MapType##_search(map, key);                             \
    if (index == -1) {                                                         \
      if (map->count < map->capacity) {                                        \
        map->values[map->count] = value;                                       \
        map->keys[map->count] = key;                                           \
        map->count++;                                                          \
      } else {                                                                 \
        size_t newCapacity = map->capacity * 2;                                \
        ValueT* newValues = (ValueT*)malloc(newCapacity * sizeof(ValueT));     \
        KeyT* newKeys = (KeyT*)malloc(newCapacity * sizeof(KeyT));             \
        memcpy(newValues, map->values, map->capacity * sizeof(ValueT));        \
        memcpy(newKeys, map->keys, map->capacity * sizeof(KeyT));              \
        free(map->keys);                                                       \
        free(map->values);                                                     \
        map->values = newValues;                                               \
        map->keys = newKeys;                                                   \
        map->values[map->count] = value;                                       \
        map->keys[map->count] = key;                                           \
        map->count++;                                                          \
      }                                                                        \
    } else {                                                                   \
      map->values[index] = value;                                              \
    }                                                                          \
  }

#define DEFINE_MAP_INTERFACE(KeyT, ValueT, MapType)                            \
  typedef struct MapType##_struct MapType;                                     \
  MapType* MapType##_create();                                                 \
  void MapType##_destroy(MapType*);                                            \
  ValueT MapType##_lookup(MapType*, KeyT);                                     \
  bool MapType##_contains(MapType*, KeyT);                                     \
  void MapType##_put(MapType*, KeyT, ValueT);
