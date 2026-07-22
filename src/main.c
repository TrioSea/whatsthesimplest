#include "libs/run.h"

int main() {
    const char Input = (char) getchar();

    if (Input == 'X') {
        printf("1");
    } else if (Input == 'O') {
        printf("0");
    } else {
        printf("NIL");
    }

    /*
    Position Home;
    Game Player1;
    Game Player2;

    FillSpace(Home, Player1, Player2);

    int Times = 0;

    _Bool NewPlayer = 1;
    _Bool StartingPlayer = 1;

    while (Times < 10) {
        printf("Player %d; ", TwoWayConversion(StartingPlayer, '1', 1, '2', 0));

        int ThroughLine = 0;
        while (ThroughLine < Home.Path.Count) {
            printf("%d", Home.Line[ThroughLine]);

            ThroughLine++;
        }

        const char Input = (char) getchar();
        printf("%c", Input);
        const _Bool In = (_Bool) TwoWayConversion(Input, 'X', 1, 'O', 0);
        printf("%d", In);

        AddSpot(&Home, In);

        if (StartingPlayer == 1) NewPlayer = 0;
        if (StartingPlayer == 0) NewPlayer = 1;
        StartingPlayer = NewPlayer;

        Times++;
    }

    free(Home.Line);
    free(Player1.List);
    free(Player2.List);
    */

    return 0;
}