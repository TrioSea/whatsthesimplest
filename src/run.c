#include "libs/run.h"

// PLEASE HANDLE FOR INVALID MEMORY ACCESSING (NULL MEMORY WHEN TRYING TO GRAB) ex. if (mem == NULL) return;
// PLEASE WORK ON THE STRING AND MEMORY MANAGEMENT LIBRARY TO ALLOW FOR EASIER ERROR MESSAGES

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
    Pave(Game->Path, Game);

    // Add the pattern
    Game->List[Game->Path.Count] = (Occurrence) {
        .Pattern = Pattern,
        .Appearances = 1
    };

    Game->Path.Count++;
}

void ModifyList(const Position Position, Game* Game, const unsigned char SequenceLength, const size_t EndAt) {
    if (EndAt + 1 < Position.Path.Count || EndAt + 1 < SequenceLength) return;

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