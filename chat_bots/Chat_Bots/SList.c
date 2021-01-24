#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "SList.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct SListNode_t {
	void* data;
	struct SListNode_t* next;
}SListNode;

typedef struct {
	SListNode* head;
	SListNode* tail;
	size_t count;
}SList;

void* slist_create() 
{
	SList* slist = (SList*)malloc(sizeof(SList));
	if (slist == NULL) {
		return NULL;
	}
	slist->head = NULL;
	slist->tail = NULL;
	slist->count = 0;
	return slist;
}

void slist_destroy(void* slist, void(*destroy)(void*)) 
{
	slist_clear(slist, (*destroy));
	if (slist != NULL) {
		free(slist);
		slist = NULL;
	}
}

void slist_clear(void* slist, void(*destroy)(void*)) 
{
	if (slist == NULL) {
		return;
	}
	SList* s_list = (SList*)slist;
	while (s_list->head != NULL) {
		slist_remove(slist, (*destroy));
	}
}

bool slist_search(void* id, void* data) 
{
	if (!id) {
		return false;
	}
	SListNode* current = id;
	current = current->next;
	for (;  current != NULL; current = current->next) {
		if (!strcmp(current->data, data)) {
			return true;
		}
	}
	return false;
}

void slist_foreach(void* slist, void(*func)(void* obj, void* buffer), void* buffer) 
{
	if (!slist) {
		return;
	}
	SList* s_list = (SList*)slist;
	SListNode* head = s_list->head;
	SListNode* current = s_list->head;
	for (; current != NULL; current = current->next)
		func(current->data, buffer);
	s_list->head = head;
}

void* slist_append(void* slist, void* data)
{
	if (slist == NULL) {
		return 0;
	}
	SList* s_list = (SList*)slist;
	SListNode* new_node = (SListNode*)malloc(sizeof(SListNode));
	if (new_node == NULL) {
		return 0;
	}
	new_node->data = data;
	new_node->next = NULL;
	if (s_list->head == NULL) {
		s_list->head = new_node;
		s_list->tail = new_node;
	}
	else {
		s_list->tail->next = new_node;
		s_list->tail = new_node;
	}
	s_list->count++;
	return s_list->tail;
}

void slist_remove(void* slist, void(*destroy)(void*)) 
{
	if (slist == NULL) {
		return;
	}
	SList* s_list = (SList*)slist;
	if (s_list->head == NULL) {
		return;
	}
	SListNode* new_head = s_list->head->next;
	if (destroy) {
		destroy(s_list->head->data);
	}
	s_list->count--;
	free(s_list->head);
	s_list->head = new_head;
}

void* slist_first(const void* slist) {
	if (slist == NULL) {
		return 0;
	}
	SList* s_list = (SList*)slist;
	return s_list->head;
}