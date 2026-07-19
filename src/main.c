#include "libs/run.h"

int main() {
    int Times = 0;

    _Bool NewPlayer = 1;
    _Bool StartingPlayer = 1;

    while (Times < 10) {
        printf("Player %c; ", TwoWayConversion(StartingPlayer, '1', 1, '2', 2));

        int ThroughLine = 0;
        while (ThroughLine < )

        if (StartingPlayer == 1) NewPlayer = 0;
        if (StartingPlayer == 0) NewPlayer = 1;
        StartingPlayer = NewPlayer;

        Times++;
    }

    Pose.Line[0] = TwoWayConversion((char) getchar(), 'O', 0, 'X', 1);

    printf("%d", Pose.Line[0]);

    return 0;
}