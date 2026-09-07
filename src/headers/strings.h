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

enum RETURNCODES {
    ALLOCFAIL = -1,
    SUCCESS = 0
};

typedef struct {
    enum RETURNCODES Code;
    _Bool ReturnCode;
} FallBack;

void ErrorHandler(String String);
FallBack CheckNewlyAllocated(const void* Memory);

void Dump(SizeTracker Path, size_t ItemRoom, void** DestinationItem, void** SourceItem);
void Pave(SizeTracker* Path, void** Item, size_t ItemRoom);

#endif // WITSPG_STRINGS_H
