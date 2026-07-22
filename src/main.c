#include "libs/run.h"

int main() {
    Position Home;
    Game Player1;
    Game Player2;

    //FillSpace(Home, Player1, Player2);
    // i dont like how it keeps on bugging me that it might not be initialized

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

    //

    _Bool NewPlayer = 0;
    _Bool StartingPlayer = 0;

    _Bool GameEnded = 0;

    while (GameEnded == 0) {
        if (StartingPlayer == 1) NewPlayer = 0;
        if (StartingPlayer == 0) NewPlayer = 1;
        StartingPlayer = NewPlayer;

        const char PlayerNumeration = TwoWayConversion(StartingPlayer, '1', 1, '2', 0);
        printf("Player %c; ", PlayerNumeration);

        int ThroughLine = 0;
        while (ThroughLine < Home.Path.Count) {
            const char PreviousInput = TwoWayConversion(Home.Line[ThroughLine], 'X', 1, 'O', 0);
            printf("%c", PreviousInput);

            ThroughLine++;
        }

        const char Input = (char) getchar();
        const _Bool In = (_Bool) TwoWayConversion(Input, 'X', 1, 'O', 0);

        const char Garbage = (char) getchar();

        //AddSpot(&Home, In);

        if (Home.Path.Count == Home.Path.Limit) {
            const int ChunkMemory = Home.Path.Limit != 0 ? 0 : 4;
            const int NewLimit = (int) (Home.Path.Limit << 1) + ChunkMemory;

            void* NewAllocation = calloc(NewLimit, sizeof(_Bool));
            const FallBack ErrorHere = CheckNewlyAllocated(NewAllocation);
            if (Error.ReturnCode == 1) return ErrorHere.Code;

            memcpy(NewAllocation, Home.Line, Home.Path.Count * sizeof(_Bool));

            free(Home.Line);
            Home.Line = NewAllocation;
            Home.Path.Limit = NewLimit;
        }

        Home.Line[Home.Path.Count] = In;
        Home.Path.Count++;

        QuickAdd(&Player1, Home, 5);
        QuickAdd(&Player2, Home, 3);

        const _Bool Player1Won = MetOccurrence(Player1, 2);
        const _Bool Player2Won = MetOccurrence(Player2, 4);

        if (Player1Won || Player2Won) {
            GameEnded = 1;

            if (Player1Won && Player2Won) {
                printf("Game Drew!");
            } else {
                if (Player1Won == 1 && Player2Won == 0) {
                    printf("Player 1 won the game!");
                }
                if (Player1Won == 0 && Player2Won == 1) {
                    printf("Player 2 won the game!");
                }
            }
        }
    }

    free(Home.Line);
    free(Player1.List);
    free(Player2.List);

    return 0;
}