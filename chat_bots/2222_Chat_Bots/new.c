#include "new.h"
#include <stdlib.h>

void* new(const void* _class, ...)
{
    const struct Class* class = _class;
    void* p = calloc(1, class->size);
    if (!p) {
        return NULL;
    }
    *(const struct Class**)p = class;
    if (class->ctor) {
        va_list ap;
        va_start(ap, _class);
        p = class->ctor(p, &ap);
        va_end(ap);
    }
    return p;
}

void delete(void* self)
{
    const struct Class** cp = self;
    if (self && *cp && (*cp)->dtor) {
        (*cp)->dtor(self);
    }
    free(self);
}

void Type(void* self, void* buffer)
{
    const struct Class* const* cp = self;
    if (self && *cp && (*cp)->Type) {
        (*cp)->Type(self, buffer);
    }
}
