#include "CowardGirl.h"
#include "new.h"

static void* CowardGirl_ctor(void* _self, va_list* app)
{
    struct CowardGirl* self = ((const struct Class*)Girl)->ctor(_self, app);
    return self;
}

#define name(p) (((const struct User *)(p)) -> name)
#define pos(p) (((struct User*)(p)) -> pos)

static void CowardGirl_Type(void* _self, void* buffer)
{
    ((const struct Class*)Girl)->Type(_self, buffer);
    struct CowardGirl* self = _self;
    if (slist_search(pos(self), phrases.rats)) {
        print_phrase(name(self), phrases.no_rats);
        pos(self) = slist_append(buffer, phrases.rats);
    }
}

static void CowardGirl_dtor(void* _self)
{
    ((const struct Class*)Girl)->dtor(_self);
}

static const struct Class _CowardGirl = {
    sizeof(struct CowardGirl),
    CowardGirl_ctor,
    CowardGirl_dtor,
    CowardGirl_Type
};

const void* CowardGirl = &_CowardGirl;