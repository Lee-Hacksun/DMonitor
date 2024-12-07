#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"

unsigned int Hash(const char* key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash << 5) + *key++; 
    }
    return hash % TABLE_SIZE;
}

HashMap* CreateHashMap() {
    HashMap *map = malloc(sizeof(HashMap));
    map->table = malloc(sizeof(Entry *) * TABLE_SIZE);
    for (int i = 0; i < TABLE_SIZE; i++) {
        map->table[i] = NULL;
    }
    return map;
}

void Put(HashMap* map, const char* key, int value) {
    unsigned int index = Hash(key);
    Entry *entry = map->table[index];

    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return; 
        }
        entry = entry->next; 
    }

    Entry *newEntry = malloc(sizeof(Entry));
    newEntry->key = strdup(key); 
    newEntry->value = value;
    newEntry->next = map->table[index]; 
    map->table[index] = newEntry;
}

int Get(HashMap* map, const char* key) {
    unsigned int index = Hash(key);
    Entry *entry = map->table[index];
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next; 
    }
    return -1; 
}

void DestroyHashMap(HashMap* map) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry *entry = map->table[i];
        while (entry != NULL) {
            Entry *temp = entry;
            entry = entry->next;
            free(temp->key);
            free(temp);
        }
    }
    free(map->table);
    free(map);
}