#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "new.h"
#include "User.h"
#include "Boy.h"
#include "Girl.h"
#include "RichBoy.h"
#include "CowardGirl.h"
#include "SList.h"
#include "File.h"
#define MAX_LEN 20

static void* bot_create(void* list, char* character, char* name) 
{
	_strlwr_s(character, MAX_LEN);
	if (!strcmp(character, "user")) {
		return new(User, name);
	}
	else if (!strcmp(character, "boy")) {
		return new(Boy, name);
	}
	else if (!strcmp(character, "girl")) {
		return new(Girl, name);
	}
	else if (!strcmp(character, "richboy")) {
		return new(RichBoy, name);
	}
	else if (!strcmp(character, "cowardgirl")) {
		return new(CowardGirl, name);
	}
	return NULL;
}

static void read_bots(FILE* fp, void* list)
{
	char line[MAX_LEN] = { 0 };
	while (fgets(line, MAX_LEN, fp) != NULL) {
		char name[MAX_LEN] = { 0 };
		char character[MAX_LEN] = { 0 };
		if (sscanf_s(line, "%s%s", name, MAX_LEN, character, MAX_LEN) == 2) {
			void* bot = bot_create(list, character, name);
			if (bot) {
				slist_append(list, bot);
			}
		}
	}
}

void* read_file(char* file_name) 
{
	FILE* fp;
	fopen_s(&fp, file_name, "r");
	if (!fp || (fgetc(fp) == EOF)) {
		return NULL;
	}
	rewind(fp);
	void* list = slist_create();
	read_bots(fp, list);
	fclose(fp);
	return list;
}