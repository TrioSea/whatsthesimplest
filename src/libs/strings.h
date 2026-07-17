#ifndef WITSPG_STRINGS_H
#define WITSPG_STRINGS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char* Bit;
} String;

typedef struct {
    size_t Count;
    size_t Limit;
} SizeTracker;

void ErrorHandler(String String);

#endif // WITSPG_STRINGS_H
