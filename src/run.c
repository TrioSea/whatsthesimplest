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
 *Have a function that adds one to an occurrence in the list
 *  In: occurrence List
 *      OOX
 *
 *Have a function that returns the line length
 *  In: Line
 *  Out: 19
 *
 *Have a function that modifies an occurrence list based on a sequence length
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


_Bool* ReadPattern(const _Bool* Line, const unsigned char PointInLine, const unsigned char LengthOfPattern) {
    _Bool* Pattern = calloc(LengthOfPattern, sizeof(_Bool));

    unsigned char PatternDigit = 0;

    while (PatternDigit < LengthOfPattern) {
        Pattern[PatternDigit] = Line[PointInLine];

        PatternDigit++;
    }

    return Pattern;
}

Game Pose() {
    return (Game) {
        .List = (Occurrence*) calloc(0, sizeof(Occurrence)),
        .Path = {
            .Count = 0,
            .Limit = 1
        }
    };
}

void InsertOccurrence(Game* Game, _Bool* Pattern) {
    int index = 0;
    _Bool found = 0;

    while (index < Game->Path.Count) {
        if (Game->List[index].Pattern == Pattern) {
            Game->List[index].Appearances++;
            found = 1;
        }

        index++;
    }

    if (found == 1) return;

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

size_t LengthOfLine(const Position Line) {
    return Line.Path.Count;
}



/*
Occurrence* List;
_Bool* LastOccurrence;
_Bool** Line;
size_t Count;
size_t Limit;
*/

