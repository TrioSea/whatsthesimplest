#include "libs/run.h"

int main() {
    Position Home;
    Game Player1;
    Game Player2;

    FallBack Error;
    const SizeTracker DemoPath = (SizeTracker) { 0 };

    Home.Line = calloc(0, sizeof(_Bool));
    Home.Path = DemoPath;
    Error = CheckNewlyAllocated(Home.Line);
    if (Error.ReturnCode == 1) return Error.Code;

    Player1.List = calloc(0, sizeof(Occurrence));
    Player1.Path = DemoPath;
    Error = CheckNewlyAllocated(Player1.List);
    if (Error.ReturnCode == 1) return Error.Code;

    Player2.List = calloc(0, sizeof(Occurrence));
    Player2.Path = DemoPath;
    Error = CheckNewlyAllocated(Player2.List);
    if (Error.ReturnCode == 1) return Error.Code;

    const Settings Player1Settings = {
        .Repeats = 2,
        .Length = 5
    };

    const Settings Player2Settings = {
        .Repeats = 4,
        .Length = 3
    };

    _Bool NewPlayer = 0;
    _Bool StartingPlayer = 0;

    _Bool GameEnded = 0;

    while (GameEnded == 0) {
        // switch player state
        if (StartingPlayer == 1) NewPlayer = 0;
        if (StartingPlayer == 0) NewPlayer = 1;
        StartingPlayer = NewPlayer;

        // print out to the player for input
        const char PlayerNumeration = TwoWayConversion(StartingPlayer, '1', 1, '2', 0);
        printf("Player %c; ", PlayerNumeration);

        // reiterate the line to the player
        int ThroughLine = 0;
        while (ThroughLine < Home.Path.Count) {
            const char PreviousInput = TwoWayConversion(Home.Line[ThroughLine], 'X', 1, 'O', 0);
            printf("%c", PreviousInput);

            ThroughLine++;
        }

        // get input from the player
        const char Input = (char) getchar();
        const _Bool In = (_Bool) TwoWayConversion(Input, 'X', 1, 'O', 0);

        getchar(); // trashes grabs the new line when returning

        AddSpot(&Home, In); // places the input into the line

        // reiterate the new patterns
        QuickAdd(&Player1, Home, Player1Settings.Length);
        QuickAdd(&Player2, Home, Player2Settings.Length);

        // check if the game is over
        const GameResult Player1Won = MetOccurrence(Player1, Player1Settings.Repeats);
        const GameResult Player2Won = MetOccurrence(Player2, Player2Settings.Repeats);

        if (Player1Won.End || Player2Won.End) {
            GameEnded = 1;

            if (Player1Won.End && Player2Won.End) {
                printf("Game Drew!");
            }

            if (Player1Won.End == 1) {
                if (Player2Won.End == 0) {
                    printf("Player 1 won the game!");
                }

                printf("\n Player1: ");
                PrintPattern(Player1Won.Pattern, Player1Settings.Length);
            }

            if (Player2Won.End == 1) {
                if (Player1Won.End == 0) {
                    printf("Player 2 won the game!");
                }

                printf("\n Player2: ");
                PrintPattern(Player2Won.Pattern, Player2Settings.Length);
            }
        }
    }

    free(Home.Line);
    free(Player1.List);
    free(Player2.List);

    printf("\n");

    return 0;
}