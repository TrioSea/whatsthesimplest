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

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

typedef struct {
    _Bool* Previous;
    _Bool* Next;
    _Bool Stored;
} Segment;

typedef struct {
    Segment* Pattern;
    uint8_t Appearances;
} Occurrence;

_Bool Convert(const char Subject) {
    if (Subject == 'X') {
        return 1;
    }
    if (Subject == 'O') {
        return 0;
    }

    return 0;
}

int main() {
    Segment* Line = calloc(1, sizeof(Segment));

    Line[0].Stored = Convert((char) getchar());

    printf("%d", Line[0].Stored);

    free(Line);

    return 0;
}