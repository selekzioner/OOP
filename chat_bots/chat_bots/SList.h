#pragma once
#include <stddef.h>
#include <stdbool.h>

void* slist_create();

void slist_destroy(void* slist, void(*destroy)(void*));
void slist_clear(void* slist, void(*destroy)(void*));

bool slist_search(void* id, void* data);
void slist_foreach(void* slist, void(*func)(void* obj, void* buffer), void* buffer);

void* slist_append(void* slist, void* data);

void slist_remove(void* slist, void(*destroy)(void*));

void* slist_first(const void* slist);