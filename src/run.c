/*
 *
 *Take in or make a Line (if directly took without prior, split up the line, start with line of 1, 2, 3, ... N)
 *
 *Have a function that takes in point in line and length of a sequence
 *  In: Line
 *      6
 *      3
 *  Out: XXX
 *
 *Have a function that makes an occurrence list
 *Have a function that adds one to an occurrence in the list - notes: needed to split
 *  In: occurrence List
 *      OOX
 *
 *Have a function that returns the line length - notes: doesnt need, one-liner
 *  In: Line
 *  Out: 19
 *
 *Have a function that modifies an occurrence list based on a sequence length - notes: i am very confused about this, probably the 5 is meant to be a 3
 *  In: Line
 *      5
 *      occurrence List
 *  Out: occurrences ex.
 *      OXO: 2
 *      XOO: 1
 *      ...
 *
 *Have a function that reads an occurrence list and returns a boolean to whether any occurrence is present at least any set time multiplier
 *  In: occurrence List
 *      4
 *  Out: false
 *
 *Have a function that receives input from the player on request
 *Have a function that outputs to the player whether the game is won for who and why
 *
 *Rules: Starting player needs 2x 5 length identical sequences
 *Other player needs 4x 3 length identical sequences
 *Whoever reaches there faster wins
 *
 *Later
 *Time Control
 *Game Host
 *Rated Matches
 *Friends List
 *
 */

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

_Bool* ReadPattern(const _Bool* Line, const unsigned char PointInLine, const unsigned char LengthOfPattern) {
    _Bool* Pattern = calloc(LengthOfPattern, sizeof(_Bool));

    unsigned char PatternDigit = 0;

    while (PatternDigit < LengthOfPattern) {
        Pattern[PatternDigit] = Line[PointInLine];

        PatternDigit++;
    }

    return Pattern;
}

_Bool NewPattern(const Game Game, const _Bool* Pattern) {
    int PatternIndex = 0;

    while (PatternIndex < Game.Path.Count) {
        if (Game.List[PatternIndex].Pattern != Pattern) {
            PatternIndex++;
        } else {
            Game.List[PatternIndex].Appearances++;
            return 1;
        }
    }

    return 0;
}

void InsertOccurrence(Game* Game, _Bool* Pattern) {
    if (NewPattern(*Game, Pattern) == 1) return;

    if (Game->Path.Count == Game->Path.Limit) {
        const size_t NewLimit = Game->Path.Limit << 1;

        Occurrence* NewList = calloc(NewLimit, sizeof(Occurrence));
        *NewList = *Game->List;

        free(Game->List);

        Game->List = NewList;
        Game->Path.Limit = NewLimit;
    }

    Game->List[Game->Path.Count] = (Occurrence) {
        .Pattern = Pattern,
        .Appearances = 1
    };

    Game->Path.Count++;
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