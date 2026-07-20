#include "strings.h"

void Pave(SizeTracker* Path, void** Item, const size_t ItemRoom) {
    if (Path->Count != Path->Limit) return; // Check if we need to actual run the function

    const size_t NewItemRule = ItemRoom * (Path->Limit != 0 ? 0 : 4); // got the idea from gemini
    const size_t OldLimit = ItemRoom * Path->Limit;
    const size_t NewLimit = (OldLimit << 1) + NewItemRule;

    void* NewAllocation = realloc(*Item, NewLimit);
    memset((char*) NewAllocation + OldLimit, 0, NewLimit - OldLimit); // this line and the last are structurely still written by gemini
    *Item = NewAllocation; // Replace the list

    Path->Limit = (int) NewLimit / ItemRoom; // Correct the limit, gemini also found a little copying bug i missed here
}

void ErrorHandler(const String String) {
    const char* Throw = strcat(String.Bit, "ERROR: ");
    printf(Throw);
}