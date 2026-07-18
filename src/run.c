#include "libs/run.h"

Position Construct() {
    return (Position) {
        .Line = (_Bool*) calloc(0, sizeof(_Bool)),
        .Path = {
            .Count = 0,
            .Limit = 1
        }
    };
}

Game ConstructPlay() {
    return (Game) {
        .List = (Occurrence*) calloc(0, sizeof(Occurrence)),
        .Path = {
            .Count = 0,
            .Limit = 1
        }
    };
}

void Dump(const Position* Source, _Bool** Destination) {


    free(Source->Line);
}

void DumpPlay(const Game* Source, Occurrence** Destination) {


    free(Source->List);
}

void AllocateSpace(SizeTracker Path, Position* Position) {
    if (Path.Count != Path.Limit) return; // Check if we need to actual run the function

    const size_t NewLimit = Path.Limit << 1; // Get the new capacity

    _Bool* NewLine = calloc(NewLimit, sizeof(_Bool)); // Allocate a new list with greater capacity
    Dump(Position, &NewLine); // Dump the old list contents in
    Position->Line = NewLine; // Replace the list

    Path.Limit = NewLimit; // Correct the limit
}

void AllocatePlaySpace(SizeTracker Path, Game* Game) {
    if (Path.Count != Path.Limit) return; // Check if we need to actual run the function

    const size_t NewLimit = Path.Limit << 1; // Get the new capacity

    Occurrence* NewList = calloc(NewLimit, sizeof(Occurrence)); // Allocate a new list with greater capacity
    DumpPlay(Game, &NewList); // Dump the old list contents in
    Game->List = NewList; // Replace the list

    Path.Limit = NewLimit; // Correct the limit
}

_Bool* ReadPattern(const _Bool* Line, const size_t PointInLine, const unsigned char SequenceLength) {
    _Bool* Pattern = calloc(SequenceLength, sizeof(_Bool)); // Clear up a pattern

    unsigned char SequenceIndex = 0; // Incremental Looping Index

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
    AllocatePlaySpace(Game->Path, Game);

    // Add the pattern
    Game->List[Game->Path.Count] = (Occurrence) {
        .Pattern = Pattern,
        .Appearances = 1
    };

    Game->Path.Count++;
}

void ModifyList(const Position Position, Game* Game, const unsigned char SequenceLength, const size_t EndAt) {
    // If the inputs are out of bounds in any way, we will throw an error
    if (EndAt + 1 < Position.Path.Count || EndAt + 1 < SequenceLength) {
        // NOT GOING TO WORK ON ERROR HANDLING AND STRING LIBRARY FOR A LITTLE BIT
        const String ErrorMessage = {
            .Bit = "Inputs are OUT OF BOUNDS"
        };

        ErrorHandler(ErrorMessage);
    }

    // Set the looping value as a back to front to decrement
    int Back = (unsigned char) EndAt + 1 - (SequenceLength - 1);

    while (Back > 0) {
        // Use the function to simplify the obtainance of the pattern
        _Bool* Pattern = ReadPattern(Position.Line, Back, SequenceLength);

        // Use the pattern to only insert it
        InsertOccurrence(Game, Pattern);
        free(Pattern);

        Back--;
    }
}


_Bool MetOccurrence(const Game Game, const size_t AppearanceRequirement) {
    int OccurrenceIndex = 0;

    while (OccurrenceIndex < Game.Path.Count) {
        if (Game.List[OccurrenceIndex].Appearances < AppearanceRequirement) {
            OccurrenceIndex++;
        } else {
            return 1;
        }
    }

    return 0;
}