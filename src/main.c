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
        const char PlayerNumeration = TwoWayConversion(StartingPlayer, '1', 1, '2', 0);
        printf("Player %c; ", PlayerNumeration);

        /*
        int ThroughLine = 0;
        while (ThroughLine < Home.Path.Count) {
            printf("%d", Home.Line[ThroughLine]);

            ThroughLine++;
        }
        */

        const char Input = (char) getchar();
        const _Bool In = (_Bool) TwoWayConversion(Input, 'X', 1, 'O', 0);

        const char Garbage = (char) getchar();

        /*
        AddSpot(&Home, In);
        */

        if (StartingPlayer == 1) NewPlayer = 0;
        if (StartingPlayer == 0) NewPlayer = 1;
        StartingPlayer = NewPlayer;

        Times++;
    }

    free(Home.Line);
    free(Player1.List);
    free(Player2.List);

    return 0;
}