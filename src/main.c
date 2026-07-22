#include "libs/run.h"

int main() {
    Position Home;
    Game Player1;
    Game Player2;

    FillSpace(Home, Player1, Player2);

    int Times = 0;

    _Bool NewPlayer = 1;
    _Bool StartingPlayer = 1;

    while (Times < 10) {
        printf("Player %c; ", TwoWayConversion(StartingPlayer, '1', 1, '2', 0));

        int ThroughLine = 0;
        while (ThroughLine < Home.Path.Count) {
            printf("%d", Home.Line[ThroughLine]);

            ThroughLine++;
        }

        AddSpot(&Home, TwoWayConversion(getchar(), 'X', 1, 'O', 0));

        if (StartingPlayer == 1) NewPlayer = 0;
        if (StartingPlayer == 0) NewPlayer = 1;
        StartingPlayer = NewPlayer;

        Times++;
    }

    return 0;
}