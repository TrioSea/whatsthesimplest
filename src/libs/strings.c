#include "strings.h"

void ErrorHandler(const String String) {
    const char* Throw = strcat(String.Bit, "ERROR: ");
    printf(Throw);
}