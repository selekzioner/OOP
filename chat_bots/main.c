#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <assert.h>
#include "new.h"
#include "User.h"
#include "Boy.h"
#include "Girl.h"
#include "RichBoy.h"
#include "CowardGirl.h"
#include "File.h"
#include "SList.h"
#define COUNT 10

int main () {
	void* buffer = slist_create();
	void* bot_list = read_file("users9.txt");
	if (bot_list) {
		for (int j = 0; j < COUNT; j++) {
			slist_foreach(bot_list, Type, buffer);
		}
	}
	slist_destroy(bot_list, delete);
	slist_destroy(buffer, 0);
	assert(!_CrtDumpMemoryLeaks());
	return 0;
}