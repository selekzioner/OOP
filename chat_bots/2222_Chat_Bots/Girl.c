#include "Girl.h"
#include "new.h"

static void* Girl_ctor(void* _self, va_list* app)
{
    struct Girl* self = ((const struct Class*)User)->ctor(_self, app);
    return self;
}

#define name(p) (((const struct User *)(p)) -> name)
#define pos(p) (((struct User*)(p)) -> pos)

static void Girl_Type(void* _self, void* buffer)
{
    struct Girl* self = _self;
    if (!pos(self)) {
        ((const struct Class*)User)->Type(_self, buffer);
        print_phrase(name(self), phrases.dress);
        slist_append(buffer, phrases.dress);
        pos(self) = slist_first(buffer);
    }
}

static void Girl_dtor(void* _self)
{
    ((const struct Class*)User)->dtor(_self);
}

static const struct Class _Girl = {
    sizeof(struct Girl),
    Girl_ctor,
    Girl_dtor,
    Girl_Type
};

const void* Girl = &_Girl;