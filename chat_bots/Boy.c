#include "Boy.h"
#include "new.h"

static void* Boy_ctor(void* _self, va_list* app) 
{
    struct Boy* self = ((const struct Class*)User)->ctor(_self, app);
    return self;
}

#define name(p) (((const struct User*)(p)) -> name)
#define pos(p) (((struct User*)(p)) -> pos)

static void Boy_Type(void* _self, void* buffer)
{
    ((const struct Class*)User)->Type(_self, buffer);
    struct Boy* self = _self;
    if (slist_search(pos(self), phrases.dress)) {
        print_phrase(name(self), phrases.rats);
        pos(self) = slist_append(buffer, phrases.rats);
    }
}

static void Boy_dtor(void* _self)
{
    ((const struct Class*)User)->dtor(_self);
}

static const struct Class _Boy = {
    sizeof(struct Boy),
    Boy_ctor,
    Boy_dtor,
    Boy_Type
};

const void* Boy = &_Boy;