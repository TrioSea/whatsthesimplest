#include "libs/run.h"

int main() {
    _Bool* Line = calloc(0, sizeof(_Bool));
    _Bool** Reference = &Line;
    Position Player1 = {
        .Line = *Reference,
        .Path = {
            .Count = 0,
            .Limit = 1
        }
    };
    Position Player2 = {
        .Line = *Reference,
        .Path = {
            .Count = 0,
            .Limit = 1
        }
    };

    int Times = 0;

    _Bool NewPlayer = 1;
    _Bool StartingPlayer = 1;

    while (Times < 10) {
        printf("Player %c; ", TwoWayConversion(StartingPlayer, '1', 1, '2', 2));

        int ThroughLine = 0;
        while (ThroughLine < *Reference->)

        if (StartingPlayer == 1) NewPlayer = 0;
        if (StartingPlayer == 0) NewPlayer = 1;
        StartingPlayer = NewPlayer;

        Times++;
    }

    Line[0] = TwoWayConversion((char) getchar(), 'O', 0, 'X', 1);

    printf("%d", Line[0]);

    free(Line);

    return 0;
}