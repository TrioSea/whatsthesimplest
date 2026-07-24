#include "libs/run.h"

_Bool* ReadPattern(const _Bool* Line, const size_t PointInLine, const int SequenceLength) {
    _Bool* Pattern = calloc(SequenceLength, sizeof(_Bool)); // Clear up a pattern
    const FallBack Error = CheckNewlyAllocated(Pattern);
    if (Error.ReturnCode == 1) return Pattern;

    int SequenceIndex = 0; // Incremental Looping Index

    while (SequenceIndex < SequenceLength) {
        Pattern[SequenceIndex] = Line[PointInLine + SequenceIndex]; // Translate pattern from the line directly
        SequenceIndex++;
    }

    return Pattern;
}

void PrintPattern(const _Bool* Pattern, const int SequenceLength) {
    int PatternIndex = 0;
    while (PatternIndex < SequenceLength) {
        printf("%c", TwoWayConversion(Pattern[PatternIndex], 'X', 1, 'O', 0));
        PatternIndex++;
    }
}

_Bool EqualPatterns(const _Bool* PatternA, const _Bool* PatternB, const int SequenceLength) {
    int PatternIndex = 0;

    while (PatternIndex < SequenceLength) {
        if (PatternA[PatternIndex] != PatternB[PatternIndex]) {
            return 0;
        }

        PatternIndex++;
    }

    return 1;

    // return memcmp(PatternA, PatternB, sizeof(_Bool) * SequenceLength) != 0 ? 0 : 1;
}



IO HandleInput(const _Bool StartingPlayer, const Position Position, const char Disregard) {
    char Input = Disregard;

    if (Disregard == 0) {
        // print out to the player for input
        const char PlayerNumeration = TwoWayConversion(StartingPlayer, '1', 1, '2', 0);
        printf("Player %c; ", PlayerNumeration);

        // reiterate the line to the player
        int ThroughLine = 0;
        while (ThroughLine < Position.Path.Count) {
            const char PreviousInput = TwoWayConversion(Position.Line[ThroughLine], 'X', 1, 'O', 0);
            printf("%c", PreviousInput);

            ThroughLine++;
        }

        Input = (char) getchar();
        getchar(); // trashes grabs the new line when returning
    }

    IO Out = { 0 };
    
    if (Input == 'x') Input = 'X';
    if (Input == 'o') Input = 'O';
    if (Input == 'e') Input = 'E';
    if (Input == 'a') Input = 'A';

    if (Input != 'X' && Input != 'O' && Input != 'E' && Input != 'A') {
        printf("No option is case sensitive. Please pick either X or an O. You can resign the game with an E. Ask for a draw with an A.\n");

        Out = HandleInput(StartingPlayer, Position, 0);
    }

    if (Input == 'X' || Input == 'O') {
        Out.Play = (_Bool) TwoWayConversion(Input, 'X', 1, 'O', 0);
    }

    const char OtherPlayerNumeration = TwoWayConversion(StartingPlayer, '2', 1, '1', 0);

    if (Input == 'E') {
        printf("Player %c has Won to resignation!\n", OtherPlayerNumeration);

        Out.Return = 1;
    }

    if (Input == 'A') {
        printf("What would you like to respond with regarding the possibility of declination? (X O A E) ");

        const char Disregards = (char) getchar();
        getchar();

        printf("Player %c, would you like to accept a draw? (A D) ", OtherPlayerNumeration);

        char AcceptanceInput = (char) getchar();
        getchar();

        if (AcceptanceInput == 'a') AcceptanceInput = 'A';
        if (AcceptanceInput == 'd') AcceptanceInput = 'D';

        if (AcceptanceInput == 'A') {
            printf("Player %c accepted a draw offer. The game ends in a tie!\n", OtherPlayerNumeration);

            Out.Return = 1;
        }

        if (AcceptanceInput == 'D') {
            printf("Player %c declined a draw offer!\n", OtherPlayerNumeration);

            Out = HandleInput(StartingPlayer, Position, Disregards);
        }
    }

    return Out;
}

void InsertOccurrence(Game* Game, _Bool* Pattern, const int SequenceLength) {
    int PatternIndex = 0;
    // Make sure it's not a new pattern
    while (PatternIndex < Game->Path.Count) {
        // Iterating through every pattern, skip the ones that do not match ours

        if (EqualPatterns(Game->List[PatternIndex].Pattern, Pattern, SequenceLength) == 1) {
            // If we hit, just increase the appearances and exit function

            Game->List[PatternIndex].Appearances++;

            return;
        }

        PatternIndex++;
    }

    // Make sure we have space to add the pattern
    Pave(&Game->Path, (void**) &Game->List, sizeof(Occurrence));

    // Add the pattern
    Game->List[Game->Path.Count] = (Occurrence) {
        .Pattern = Pattern,
        .Appearances = 1
    };

    Game->Path.Count++;
}

void AddSpot(Position* Position, const _Bool ADD) {
    Pave(&Position->Path, (void**) &Position->Line, sizeof(_Bool));

    Position->Line[Position->Path.Count] = ADD;
    Position->Path.Count++;
}

void QuickAdd(Game* Game, const Position Position, const int SequenceLength) {
    // Adds Last Occurrence to a list
    if (Position.Path.Count < SequenceLength) return;

    void* Pattern = ReadPattern(Position.Line, Position.Path.Count - SequenceLength, SequenceLength);
    InsertOccurrence(Game, Pattern, SequenceLength);
}

void ModifyList(const Position Position, Game* Game, const int SequenceLength, const size_t EndAt) {
    if (EndAt < Position.Path.Count || EndAt < SequenceLength) return;

    // Set the looping value as a back to front to decrement
    int Back = (int) EndAt - (SequenceLength - 1);

    while (Back >= 0) {
        // Use the function to simplify the obtainance of the pattern
        _Bool* Pattern = ReadPattern(Position.Line, Back + 1, SequenceLength);

        // Use the pattern to only insert it
        InsertOccurrence(Game, Pattern, SequenceLength);
        free(Pattern);
        Pattern = NULL;

        Back--;
    }
}

GameConclude Simulate(Position Position, const _Bool* Pattern) {
    const int SequenceLength = (int) sizeof(*Pattern) / sizeof(_Bool);

    FallBack Error;
    const SizeTracker DemoPath = (SizeTracker) { 0 };
    GameConclude Take = { 0 };
    Take.Error = 1;

    Game Player1;
    Game Player2;

    Player1.List = calloc(0, sizeof(Occurrence));
    Player1.Path = DemoPath;
    Error = CheckNewlyAllocated(Player1.List);
    if (Error.ReturnCode == 1) return Take;

    Player2.List = calloc(0, sizeof(Occurrence));
    Player2.Path = DemoPath;
    Error = CheckNewlyAllocated(Player2.List);
    if (Error.ReturnCode == 1) return Take;

    const Settings Player1Settings = {
        .Repeats = 2,
        .Length = 5
    };

    const Settings Player2Settings = {
        .Repeats = 4,
        .Length = 3
    };

    ModifyList(Position, &Player1, Player1Settings.Length, Position.Path.Count);
    ModifyList(Position, &Player2, Player2Settings.Length, Position.Path.Count);

    int PatternIndex = 0;

    while (PatternIndex < SequenceLength) {
        AddSpot(&Position, Pattern[PatternIndex]);







        PatternIndex++;
    }
}

GameResult MetOccurrence(const Game Game, const int AppearanceRequirement) {
    int OccurrenceIndex = 0;

    while (OccurrenceIndex < Game.Path.Count) {
        if (Game.List[OccurrenceIndex].Appearances >= AppearanceRequirement) {
            return (GameResult) {
                .End = 1,
                .Pattern = Game.List[OccurrenceIndex].Pattern
            };
        }

        OccurrenceIndex++; // Running through all occurrences until one meets requirement
    }

    return (GameResult) { 0 }; // Return the results
}

GameConclude GameEnding(const Game Player1, const Game Player2, const Settings Player1Settings, const Settings Player2Settings, const _Bool WeStart) {
    GameConclude Conclusion = { 0 };

    const GameResult Player1Won = MetOccurrence(Player1, Player1Settings.Repeats);
    const GameResult Player2Won = MetOccurrence(Player2, Player2Settings.Repeats);

    if (Player1Won.End || Player2Won.End) Conclusion.End = 1;
    if (Player1Won.End && Player2Won.End) Conclusion.Drew = 1;
    if (Player2Won.End != WeStart && Player1Won.End != Player2Won.End) Conclusion.WeWon = 1;

    Conclusion.Player1Pattern = Player1Won.Pattern;
    Conclusion.Player2Pattern = Player2Won.Pattern;

    return Conclusion;
}

void OutputResult(const GameConclude Result, const Settings Player1Settings, const Settings Player2Settings, const _Bool StartingPlayer) {
    if (Result.Drew) {
        printf("Game Drew!");

        printf("\n Player1: ");
        PrintPattern(Result.Player1Pattern, Player1Settings.Length);

        printf("\n Player2: ");
        PrintPattern(Result.Player2Pattern, Player2Settings.Length);
    } else {
        const char PlayerNumeration = TwoWayConversion(StartingPlayer, '1', 1, '2', 0);
        printf("We (Player %c) ", PlayerNumeration);

        if (Result.WeWon == 1) printf("Won");
        if (Result.WeWon == 0) printf("Lost");

        printf(" with ");

        const _Bool Outcome = (Result.WeWon == 1 && StartingPlayer == 0) || (Result.WeWon == 0 && StartingPlayer == 1);

        if (Outcome == 1) PrintPattern(Result.Player2Pattern, Player2Settings.Length);
        if (Outcome == 0) PrintPattern(Result.Player1Pattern, Player1Settings.Length);

        printf("!");
    }
}