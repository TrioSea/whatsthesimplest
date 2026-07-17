#include "libs/run.h"

Game Pose() {
    return (Game) {
        .List = (Occurrence*) calloc(0, sizeof(Occurrence)),
        .Path = {
            .Count = 0,
            .Limit = 1
        }
    };
}

_Bool* ReadPattern(const _Bool* Line, const size_t PointInLine, const unsigned char LengthOfPattern) {
    _Bool* Pattern = calloc(LengthOfPattern, sizeof(_Bool));

    unsigned char PatternDigit = 0;

    while (PatternDigit < LengthOfPattern) {
        Pattern[PatternDigit] = Line[PointInLine];

        PatternDigit++;
    }

    return Pattern;
}

void AllocateSpace(Game* Game) {
    if (Game->Path.Count != Game->Path.Limit) return;

    // Get the new capacity
    const size_t NewLimit = Game->Path.Limit << 1;

    // Replace the old allocate with a new one
    Occurrence* NewList = calloc(NewLimit, sizeof(Occurrence)); // New allocation
    *NewList = *Game->List;
    free(Game->List);

    // Correct the information in Game
    Game->List = NewList;
    Game->Path.Limit = NewLimit;
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
    AllocateSpace(Game);

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
        const String ErrorMessage = {
            .Bit = "Inputs are OUT OF BOUNDS"
        };

        ErrorHandler(ErrorMessage);
    }

    int Back = (unsigned char) EndAt - (SequenceLength - 1);

    while (Back > 0) {
        int SequenceIndex = 0;
        _Bool* Pattern = calloc(SequenceLength, sizeof(_Bool));

        while (SequenceIndex < SequenceLength) {
            const int Index = Back - (SequenceLength - 1) + SequenceIndex;
            Pattern[SequenceIndex] = Position.Line[Index];
            SequenceIndex++;
        }

        InsertOccurrence(Game, Pattern);
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