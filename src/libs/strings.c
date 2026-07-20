#include "strings.h"

void Pave(SizeTracker* Path, void** Item, const size_t ItemRoom) {
    if (Path->Count != Path->Limit) return; // Check if we need to actual run the function

    const size_t NewItemRule = ItemRoom * (Path->Limit != 0 ? 0 : 4);
    const size_t OldLimit = ItemRoom * Path->Limit;
    const size_t NewLimit = (OldLimit << 1) + NewItemRule; // Get the new capacity, got 4 from gemini
    int ItemIndex = 0;

    void* NewAllocation = malloc(NewLimit); // Allocate a new list with greater capacity
    while (ItemIndex < Path->Count) {
        ((_Bool*) NewAllocation)[ItemIndex] = ((_Bool*) *Item)[ItemIndex]; // Copy old contents into new allocation
        ItemIndex++;
    }
    free(*Item); // Dump the old list
    *Item = NewAllocation; // Replace the list

    Path->Limit = (int) NewLimit / ItemRoom; // Correct the limit
}

void ErrorHandler(const String String) {
    const char* Throw = strcat(String.Bit, "ERROR: ");
    printf(Throw);
}