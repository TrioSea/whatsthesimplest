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

        // get input from the player
        const IO Action = HandleInput(StartingPlayer, Home, 0);

        if (Action.Return == 1) break;

        AddSpot(&Home, Action.Play); // places the input into the line

        // reiterate the new patterns
        QuickAdd(&Player1, Home, Player1Settings.Length);
        QuickAdd(&Player2, Home, Player2Settings.Length);

        // check if the game is over
        const GameConclude Result = GameEnding(Player1, Player2, Player1Settings, Player2Settings, StartingPlayer);

        GameEnded = Result.End;

        if (GameEnded == 1) OutputResult(Result, Player1Settings, Player2Settings, StartingPlayer);
    }

    free(Home.Line);

    int PlayIndex;

    PlayIndex = 0;
    while (PlayIndex < Player1.Path.Count) {
        _Bool* Pattern = Player1.List[PlayIndex].Pattern;
        free(Pattern);
        Pattern = NULL;
        PlayIndex++;
    }
    free(Player1.List);

    PlayIndex = 0;
    while (PlayIndex < Player2.Path.Count) {
        _Bool* Pattern = Player2.List[PlayIndex].Pattern;
        free(Pattern);
        Pattern = NULL;
        PlayIndex++;
    }
    free(Player2.List);

    printf("\n");

    return 0;
}