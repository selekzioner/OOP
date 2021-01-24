#include <string.h>
#include <assert.h>
#include <math.h>

#include "slist.h"

typedef struct {
	int array[8];
	float d_variable;
} Value;

int main(int argc, char* argv[])
{
	//Создаем односвязный список с элементами типа Value;
	void* slist = slist_create(sizeof(Value));

	assert(0 == slist_count(slist));
	assert(slist_stop(slist) == slist_first(slist));

	//Создаем объект для односвязного списка
	Value value = { {1, 2, 3, 4, 5, 6, 7, 8}, 0.f };

	//Добавляем новый элемент в односвязный список
	Value* insertedValue = (Value*)slist_prepend(slist);

	//Инициализируем добавленный элемент
	*insertedValue = value;

	Value* item = (Value*)slist_item(slist, 0);

	for (size_t i = 0; 8 > i; ++i) {
		assert(item->array[i] == value.array[i]);
	}

	assert(fabsf(item->d_variable - value.d_variable) < 1e-10f);
	assert(NULL == slist_item(slist, 1));

	assert(slist_next(slist, slist_first(slist)) == slist_stop(slist));

	slist_destroy(slist, NULL);

	return 0;
}

/*#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <assert.h>
#include <stdio.h>
#include "slist.h"

int main() {
	void* list = slist_create(4);
	for (int i = 0; i < 5; i++) {
		slist_prepend(list);
	}
	assert(slist_count(list) == 5);
	slist_erase(list, 100, 0);
	size_t frst = slist_first(list);
	size_t nxt = slist_next(list, frst);
	assert(slist_current(list, slist_next(list, slist_next(list, nxt))) == slist_item(list, 3));
	assert(slist_current(list, 0) == 0);
	assert(slist_current(list, nxt) == slist_item(list, 1));
	slist_insert(list, slist_next(list, frst));
	assert(slist_count(list) == 6);
	slist_remove(list, 0);
	assert(slist_count(list) == 5);
	slist_erase(list, nxt, 0);
	assert(slist_count(list) == 4);
	slist_remove(list, 0);
	slist_remove(list, 0);
	slist_remove(list, 0);
	slist_init(list, 10, 0);
	slist_prepend(list);
	slist_erase(list, slist_first(list), 0);
	assert(slist_count(list) == 0);
	assert(slist_count(NULL) == (size_t)-1);

	assert(slist_init(list, 0, 0) == NULL);
	assert(slist_init(list, 5, 0) == list);
	assert(slist_insert(list, slist_stop(list)) == slist_item(list, 0));
	assert(slist_insert(list, slist_stop(list)) == NULL);
	assert(slist_insert(NULL, slist_stop(list)) == NULL);
	slist_destroy(list, 0);

	assert(_CrtDumpMemoryLeaks() == 0);
	return 0;
}*/