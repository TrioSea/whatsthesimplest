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

typedef struct {
    _Bool* Pattern;
    unsigned char Appearances;
} Occurrence;

typedef struct {
    char Expectation;
    _Bool Return;
} Product;

Product ConvertParent(const char Subject, const char ConnectLetter, const char ConnectNumber) {
    Product End = { 0 };

    if (Subject == ConnectLetter) End = (Product) {.Expectation = ConnectNumber, .Return = 1};
    if (Subject == ConnectNumber) End = (Product) {.Expectation = ConnectNumber, .Return = 1};

    return End;
}

_Bool Convert(const char Subject) {
    Product End = { 0 };

    End = ConvertParent(Subject, '0', 0);
    if (End.Return == 1) return End.Expectation;

    End = ConvertParent(Subject, 'X', 1);
    if (End.Return == 1) return End.Expectation;

    return 0;
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

int main() {
    _Bool* Line = calloc(1, sizeof(_Bool));

    Line[0] = Convert((char) getchar());

    printf("%d", Line[0]);

    free(Line);

    return 0;
}