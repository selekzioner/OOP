#include "RichBoy.h"
#include "new.h"

static void* RichBoy_ctor(void* _self, va_list* app)
{
    struct RichBoy* self = ((const struct Class*)Boy)->ctor(_self, app);
    return self;
}

#define name(p) (((const struct User*)(p)) -> name)
#define pos(p) (((struct User*)(p)) -> pos)

static void RichBoy_Type(void* _self, void* buffer)
{
    struct RichBoy* self = _self;
    if (!pos(self)) {
        ((const struct Class*)User)->Type(_self, buffer);
        slist_append(buffer, phrases.car);
        print_phrase(name(self), phrases.car);
        pos(self) = slist_first(buffer);
    }
    if (slist_search(pos(self), phrases.dress)) {
        print_phrase(name(self), phrases.richest_boy);
        pos(self) = slist_append(buffer, phrases.richest_boy);
    }
}

static void RichBoy_dtor(void* _self)
{
    ((const struct Class*)User)->dtor(_self);
}

static const struct Class _RichBoy = {
    sizeof(struct RichBoy),
    RichBoy_ctor,
    RichBoy_dtor,
    RichBoy_Type
};

const void* RichBoy = &_RichBoy;