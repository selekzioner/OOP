#include "slist.h"
#include <stdlib.h>

typedef struct SListNode_t {
	void* data;
	struct SListNode_t* next;
}SListNode;

typedef struct {
	size_t itemSize;
	SListNode* head;
	size_t count;
}SList;

void* slist_create(size_t itemSize) {
	if (itemSize == 0) {
		return NULL;
	}
	SList* slist = (SList*)malloc(sizeof(SList));
	if (slist == NULL) {
		return NULL;
	}
	slist->itemSize = itemSize;
	slist->head = NULL;
	slist->count = 0;
	return slist;
}

void slist_destroy(void* slist, void(*destroy)(void*)) {
	slist_clear(slist, (*destroy));
	if (slist != NULL) {
		free(slist);
		slist = NULL;
	}
}

void* slist_init(void* slist, size_t itemSize, void(*destroy)(void*)) {
	if (slist == NULL || itemSize == 0) {
		return NULL;
	}
	slist_clear(slist, (*destroy));
	SList* s_list = (SList*)slist;
	s_list->itemSize = itemSize;
	s_list->head = NULL;
	s_list->count = 0;
	return s_list;
}

void slist_clear(void* slist, void(*destroy)(void*)) {
	if (slist == NULL) {
		return;
	}
	SList* s_list = (SList*)slist;
	while (s_list->head != NULL) {
		slist_remove(slist, (*destroy));
	}
}

size_t slist_count(const void* slist) {
	if (slist == NULL) {
		return (size_t)-1;
	}
	return ((SList*)slist)->count;
}

void* slist_item(void* slist, size_t i) {
	if (slist == NULL) {
		return NULL;
	}
	SList* s_list = (SList*)slist;
	SListNode* current = s_list->head;
	for (size_t count = 0; current != NULL; count++, current = current->next) {
		if (count == i) {
			return current->data;
		}
	}
	return NULL;
}

void* slist_prepend(void* slist) {
	if (slist == NULL) {
		return NULL;
	}
	SList* s_list = (SList*)slist;
	SListNode* new_node = (SListNode*)malloc(sizeof(SListNode));
	if (new_node == NULL) {
		return NULL;
	}
	new_node->next = s_list->head;
	new_node->data = malloc(s_list->itemSize);
	if (new_node->data == NULL) {
		return NULL;
	}
	s_list->head = new_node;
	s_list->count++;
	return s_list->head->data;
}

void slist_remove(void* slist, void(*destroy)(void*)) {
	if (slist == NULL) {
		return;
	}
	SList* s_list = (SList*)slist;
	if (s_list->head == NULL) {
		return;
	}
	SListNode* new_head = s_list->head->next;
	if ((*destroy) != 0) {
		(*destroy)(s_list->head->data);
	}
	free(s_list->head->data);
	free(s_list->head);
	s_list->count--;
	s_list->head = new_head;
}

size_t slist_first(const void* slist) {
	if (slist == NULL) {
		return slist_stop(slist);
	}
	SList* s_list = (SList*)slist;
	return (size_t)s_list->head;
}

size_t slist_next(const void* slist, size_t item_id) {
	if (slist == NULL || item_id == slist_stop(slist)) {
		return slist_stop(slist);
	}
	SList* s_list = (SList*)slist;
	SListNode* current = (SListNode*)item_id;
	return (size_t)current->next;
}

size_t slist_stop(const void* slist) {
	return 0;
}

void* slist_current(const void* slist, size_t item_id) {
	if (slist == NULL || item_id == slist_stop(slist)) {
		return NULL;
	}
	SList* s_list = (SList*)slist;
	SListNode* current = (SListNode*)item_id;
	return current->data;
}

void* slist_insert(void* slist, size_t item_id) {
	if (slist == NULL) {
		return NULL;
	}
	SList* s_list = (SList*)slist;
	if (s_list->head == NULL && item_id == slist_stop(slist)) {
		return slist_prepend(slist);
	}
	if (item_id == slist_stop(slist)) {
		return NULL;
	}
	SListNode* current = (SListNode*)item_id;
	SListNode* next = current->next;
	SListNode* new_node = (SListNode*)malloc(sizeof(SListNode));
	if (new_node == NULL) {
		return NULL;
	}
	new_node->next = next;
	new_node->data = malloc(s_list->itemSize);
	if (new_node->data == NULL) {
		return NULL;
	}
	current->next = new_node;
	s_list->count++;
	return current->next->data;
}

void slist_erase(void* slist, size_t item_id, void(*destroy)(void*)) {
	if (slist == NULL || item_id == slist_stop(slist)) {
		return;
	}
	SList* s_list = (SList*)slist;
	SListNode* current = s_list->head;
	SListNode* prev = NULL;
	if ((size_t)s_list->head == item_id) {
		slist_remove(slist, (*destroy));
		return;
	}
	for (; current != NULL; current = current->next) {
		if (s_list->head->next != NULL && (size_t)s_list->head->next == item_id) {
			prev = current;
			break;
		}
	}
	if (prev == NULL) {
		return;
	}
	SListNode* middle = prev->next;
	prev->next = middle->next;
	if ((*destroy) != 0) {
		(*destroy)(middle->data);
	}
	free(middle->data);
	free(middle);
	s_list->count--;
}