#include "strings.h"

// STRING LIBRARY IS A WIP

void ErrorHandler(const String String) {
    const char* Throw = strcat(String.Bit, "ERROR: ");
    printf(Throw);
}

FallBack CheckNewlyAllocated(const void* Memory) {
    if (Memory == NULL) {
        const String ErrorMessage = (String) {
            .Bit = "Memory allocation failed"
        };

        ErrorHandler(ErrorMessage);

        return (FallBack) {
            .Code = ALLOCFAIL,
            .ReturnCode = 1
        };
    }

    return (FallBack) {
        .Code = SUCCESS,
        .ReturnCode = 0
    };
}

void Dump(const SizeTracker Path, const size_t ItemRoom, void **DestinationItem, void **SourceItem) {
    /*
     * //IF YOU CAN FIGURE OUT HOW TO GET THIS IDEA WORKING PLEASE DO
    int ItemIndex = 0;
    while (ItemIndex < Path.Count) {
        ((Type*) *DestinationItem)[ItemIndex] = ((Type*) *SourceItem)[ItemIndex]; // Copy old contents into new allocation
        // I dont want these to be restricted to just a boolean, is there a memory function?

        ItemIndex++;
    }
    */

    memcpy(*DestinationItem, *SourceItem, Path.Count * ItemRoom);

    free(*SourceItem); // Dump the old list
    *SourceItem = NULL;
}

void Pave(SizeTracker *Path, void **Item, const size_t ItemRoom) {
    if (Path->Count != Path->Limit) return; // Check if we need to actual run the function

    const int ChunkMemory = Path->Limit != 0 ? 0 : 4;
    const int NewLimit = (int) (Path->Limit << 1) + ChunkMemory;

    void *NewAllocation = calloc(NewLimit, ItemRoom);
    const FallBack Error = CheckNewlyAllocated(NewAllocation);
    if (Error.ReturnCode == 1) return;

    Dump(*Path, ItemRoom, Item, &NewAllocation);
    *Item = NewAllocation; // Replace the list
    Path->Limit = NewLimit; // Correct the limit
}