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

void PrintPattern(const _Bool* Pattern, const int SequenceLength) {
    int PatternIndex = 0;
    while (PatternIndex < SequenceLength) {
        printf("%c", TwoWayConversion(Pattern[PatternIndex], 'X', 1, 'O', 0));
        PatternIndex++;
    }
}

_Bool EqualPatterns(const _Bool* PatternA, const _Bool* PatternB, const int SequenceLength) {
    int PatternIndex = 0;

    while (PatternIndex < SequenceLength) {
        if (PatternA[PatternIndex] != PatternB[PatternIndex]) {
            return 0;
        }

        PatternIndex++;
    }

    return 1;

    // return memcmp(PatternA, PatternB, sizeof(_Bool) * SequenceLength) != 0 ? 0 : 1;
}

void InsertOccurrence(Game* Game, _Bool* Pattern, const int SequenceLength) {


    int PatternIndex = 0;

    // Make sure it's not a new pattern
    while (PatternIndex < Game->Path.Count) {
        // Iterating through every pattern, skip the ones that do not match ours

        if (EqualPatterns(Game->List[PatternIndex].Pattern, Pattern, SequenceLength) == 1) {
            PrintPattern(Game->List[PatternIndex].Pattern, SequenceLength);
            printf(":Ac ");
            PrintPattern(Pattern, SequenceLength);
            printf(":Bc ");

            // If we hit, just increase the appearances and exit function
            Game->List[PatternIndex].Appearances++;
            printf("Extra ");
            return;

        }

        PatternIndex++;
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

void AddSpot(Position* Position, const _Bool ADD) {
    Pave(&Position->Path, (void**)&Position->Line, sizeof(_Bool));

    Position->Line[Position->Path.Count] = ADD;
    Position->Path.Count++;
}

void QuickAdd(Game* Game, const Position Position, const int SequenceLength) {
    // Adds Last Occurrence to a list
    if (Position.Path.Count < SequenceLength) return;

    const _Bool* Pattern = ReadPattern(Position.Line, (size_t) Position.Path.Count - SequenceLength, SequenceLength);
    InsertOccurrence(Game, Pattern, SequenceLength);

    free(Pattern);
    Pattern = NULL;
}

void ModifyList(const Position Position, Game* Game, const int SequenceLength, const size_t EndAt) {
    if (EndAt + 1 < Position.Path.Count || EndAt + 1 < SequenceLength) return;

    // Set the looping value as a back to front to decrement
    int Back = (int) EndAt + 1 - (SequenceLength - 1);

    while (Back > 0) {
        // Use the function to simplify the obtainance of the pattern
        _Bool* Pattern = ReadPattern(Position.Line, Back, SequenceLength);

        // Use the pattern to only insert it
        InsertOccurrence(Game, Pattern, SequenceLength);
        free(Pattern);
        Pattern = NULL;

        Back--;
    }
}

_Bool MetOccurrence(const Game Game, const int AppearanceRequirement) {
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

void FillSpace(Position Home, Game Player1, Game Player2) {
    FallBack Error;
    const SizeTracker DemoPath = (SizeTracker) { 0 };

    Home.Line = calloc(0, sizeof(_Bool));
    Home.Path = DemoPath;
    Error = CheckNewlyAllocated(Home.Line);
    if (Error.ReturnCode == 1) return;

    Player1.List = calloc(0, sizeof(Occurrence));
    Player1.Path = DemoPath;
    Error = CheckNewlyAllocated(Player1.List);
    if (Error.ReturnCode == 1) return;

    Player2.List = calloc(0, sizeof(Occurrence));
    Player2.Path = DemoPath;
    Error = CheckNewlyAllocated(Player2.List);
    if (Error.ReturnCode == 1) return;
}