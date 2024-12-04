#ifndef MAP_H
#define MAP_H

#define TABLE_SIZE 128

typedef struct _Entry {
    char* key;
    int value;
    struct _Entry* next; // 체이닝을 위한 포인터
} Entry;

typedef struct _HashMap {
    Entry** table;
} HashMap;


unsigned int Hash(const char *key);
HashMap* CreateHashMap();

void Put(HashMap* map, const char* key, int value);
int Get(HashMap* map, const char* key) ;

void DestroyHashMap(HashMap* map);

#endif