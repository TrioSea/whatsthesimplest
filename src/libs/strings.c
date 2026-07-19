#include "strings.h"

void Pave(SizeTracker* Path, void** Item, const size_t ItemRoom) {
    if (Path->Count != Path->Limit) return; // Check if we need to actual run the function

    const int NewItemRule = Path->Limit != 0 ? 0 : 4;
    const size_t NewLimit = (Path->Limit << 1) + NewItemRule; // Get the new capacity, got 4 from gemini
    int ItemIndex = 0;

    _Bool* NewAllocation = calloc(NewLimit, ItemRoom); // Allocate a new list with greater capacity
    while (ItemIndex < Path->Count) {
        NewAllocation[ItemIndex] = *(_Bool*) Item[ItemIndex]; // Copy old contents into new allocation
        ItemIndex++;
    }
    free(*Item); // Dump the old list
    *Item = NewAllocation; // Replace the list

    Path->Limit = NewLimit; // Correct the limit
}

void ErrorHandler(const String String) {
    const char* Throw = strcat(String.Bit, "ERROR: ");
    printf(Throw);
}