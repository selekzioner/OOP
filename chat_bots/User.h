#pragma once
#include "phrases.h"
#include "SList.h"

struct User {
    const void* class;
    char* name;
    void* pos;
};

extern const void* User;