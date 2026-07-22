#include "libs/run.h"

_Bool* ReadPattern(const _Bool* Line, const size_t PointInLine, const int SequenceLength) {
    _Bool* Pattern = calloc(SequenceLength, sizeof(_Bool)); // Clear up a pattern
    const FallBack Error = CheckNewlyAllocated(Pattern);
    if (Error.ReturnCode == 1) return Pattern;

    int SequenceIndex = 0; // Incremental Looping Index

    while (SequenceIndex < SequenceLength) {
        Pattern[SequenceIndex] = Line[PointInLine + SequenceIndex]; // Translate pattern from the line directly
        SequenceIndex++;
    }

    return Pattern;
}

void InsertOccurrence(Game* Game, _Bool* Pattern) {
    int PatternIndex = 0;

    // Make sure it's not a new pattern
    while (PatternIndex < Game->Path.Count) {
        // Iterating through every pattern, skip the ones that do not match ours
        if (Game->List[PatternIndex].Pattern != Pattern) {
            PatternIndex++;
        } else {
            // If we hit, just increase the appearances and exit function
            Game->List[PatternIndex].Appearances++;
            return;
        }
    }

    // Make sure we have space to add the pattern
    Pave(&Game->Path, (void**)&Game->List, sizeof(Occurrence));

    // Add the pattern
    Game->List[Game->Path.Count] = (Occurrence) {
        .Pattern = Pattern,
        .Appearances = 1
    };

    Game->Path.Count++;
}

void ModifyList(const Position Position, Game* Game, const int SequenceLength, const size_t EndAt) {
    if (EndAt + 1 < Position.Path.Count || EndAt + 1 < SequenceLength) return;

    // Set the looping value as a back to front to decrement
    int Back = (int) EndAt + 1 - (SequenceLength - 1);

    while (Back > 0) {
        // Use the function to simplify the obtainance of the pattern
        _Bool* Pattern = ReadPattern(Position.Line, Back, SequenceLength);

        // Use the pattern to only insert it
        InsertOccurrence(Game, Pattern);
        free(Pattern);
        Pattern = NULL;

        Back--;
    }
}

_Bool MetOccurrence(const Game Game, const size_t AppearanceRequirement) {
    int OccurrenceIndex = 0;

    while (OccurrenceIndex < Game.Path.Count) {
        if (Game.List[OccurrenceIndex].Appearances < AppearanceRequirement) {
            OccurrenceIndex++; // Running through all occurrences until one meets requirement
        } else {
            return 1;
        }
    }

    return 0; // Return the results
}

void AddSpot(Position* Position, const _Bool ADD) {
    //Pave(&Position->Path, (void**)&Position->Line, sizeof(_Bool));

    if (Position->Path.Count == Position->Path.Limit) {
        const int ChunkMemory = Position->Path.Limit != 0 ? 0 : 4;
        const int NewLimit = (int) (Position->Path.Limit << 1) + ChunkMemory;

        void* NewAllocation = calloc(NewLimit, sizeof(_Bool));
        const FallBack Error = CheckNewlyAllocated(NewAllocation);
        if (Error.ReturnCode == 1) return;

        memcpy(NewAllocation, Position->Line, Position->Path.Count * sizeof(_Bool));

        free(Position->Line);
        Position->Line = NewAllocation;
        Position->Path.Limit = NewLimit;
    }

    Position->Line[Position->Path.Count] = ADD;
    Position->Path.Count++;
}

void FillSpace(Position Home, Game Player1, Game Player2) {
    FallBack Error;

    Home.Line = calloc(0, sizeof(_Bool));
    Error = CheckNewlyAllocated(Home.Line);
    if (Error.ReturnCode == 1) return;

    Player1.List = calloc(0, sizeof(Occurrence));
    Error = CheckNewlyAllocated(Player1.List);
    if (Error.ReturnCode == 1) return;

    Player2.List = calloc(0, sizeof(Occurrence));
    Error = CheckNewlyAllocated(Player2.List);
    if (Error.ReturnCode == 1) return;
}