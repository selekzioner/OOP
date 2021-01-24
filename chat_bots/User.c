#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "User.h"
#include "new.h"

static void* User_ctor(void* _self, va_list* app)
{
    struct User* self = _self;
    char* name = va_arg(*app, char*);
    size_t len = strlen(name) + 1;
    self->name = malloc(len);
    strcpy_s(self->name, len, name);
    self->pos = NULL;
    return self;
}

static void User_Type(void* _self, void* buffer)
{
    struct User* self = _self;
    if (!self->pos) {
        print_phrase(self->name, phrases.hey);
        slist_append(buffer, phrases.hey);
        self->pos = slist_first(buffer);
    }
}

static void User_dtor(void* _self) {
    struct User* self = _self;
    if (self) {
        free(self->name);
    }
}

static const struct Class _User = {
    sizeof(struct User),
    User_ctor,
    User_dtor,
    User_Type
};

const void* User = &_User;