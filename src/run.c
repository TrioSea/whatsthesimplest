#include "headers/run.h"

Set Hold() {
    Set Element = (Set) {
        .Player1 = (Game) {
            .List = calloc(0, sizeof(Occurrence)),
            .Path = (SizeTracker) { 0 }
        },
        .Player2 = (Game) {
            .List = calloc(0, sizeof(Occurrence)),
            .Path = (SizeTracker) { 0 }
        },
        .Player1Settings = (Settings) {
            .Repeats = 2,
            .Length = 5
        },
        .Player2Settings = (Settings) {
            .Repeats = 4,
            .Length = 3
        },
        .Full = 1,
        .Home = (Position) {
            .Line = calloc(0, sizeof(_Bool)),
            .Path = (SizeTracker) { 0 },
            .BotLevel = 0
        },
    };

    FallBack Error;

    Error = CheckNewlyAllocated(Element.Home.Line);
    if (Error.ReturnCode == 1) Element.ErrorCode = Error.Code;

    Error = CheckNewlyAllocated(Element.Player1.List);
    if (Error.ReturnCode == 1) Element.ErrorCode = Error.Code;

    Error = CheckNewlyAllocated(Element.Player2.List);
    if (Error.ReturnCode == 1) Element.ErrorCode = Error.Code;

    return Element;
}

void Release(_Bool* Line, Game* Player1, Game* Player2) {
    free(Line);
    Line = NULL;

    int Beta;
    int* PlayIndex = &Beta;

    while (*PlayIndex < Player1->Path.Count) {
        _Bool* Pattern = Player1->List[*PlayIndex].Pattern;

        free(Pattern);
        Pattern = NULL;

        (*PlayIndex)++;
    }

    *PlayIndex = 0;

    free(Player1->List);
    Player1->List = NULL;

    while (*PlayIndex < Player2->Path.Count) {
        _Bool* Pattern = Player2->List[*PlayIndex].Pattern;

        free(Pattern);
        Pattern = NULL;

        (*PlayIndex)++;
    }

    *PlayIndex = 0;

    free(Player2->List);
    Player2->List = NULL;
}

void SwapState(_Bool* State) {
    if (*State == 0) {
        *State = 1;
        return;
    }
    *State = 0;
}

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



void PrintLine(const Position Position) {
    int ThroughLine = 0;
    while (ThroughLine < Position.Path.Count) {
        const char PreviousInput = TwoWayConversion(Position.Line[ThroughLine], 'X', 1, 'O', 0);
        printf("%c", PreviousInput);

        ThroughLine++;
    }
}

IO HandleInput(const _Bool StartingPlayer, const _Bool Player, const Position Position, const char Disregard, const char Override) {
    const size_t Check = 1;
    char* Filter = malloc(Check);

    char Input = Disregard;

    if (Input == 0 && Override == 0) {
        // print out to the player for input
        const char PlayerNumeration = TwoWayConversion(StartingPlayer, '1', 1, '2', 0);
        printf("Player %c; ", PlayerNumeration);

        // reiterate the line to the player
        PrintLine(Position);

        memset(Filter, 0, Check);
        scanf("%s", Filter);

        Input = Filter[0];
    }

    if (Override != 0) Input = Override;

    IO Out = { 0 };
    
    if (Input == 'x') Input = 'X';
    if (Input == 'o') Input = 'O';
    if (Input == 'e') Input = 'E';
    if (Input == 'a') Input = 'A';

    if (Input == 'X' || Input == 'O') {
        Out.Play = (_Bool) TwoWayConversion(Input, 'X', 1, 'O', 0);
    }

    const char OtherPlayerNumeration = TwoWayConversion(StartingPlayer, '2', 1, '1', 0);

    if (Input == 'E') {
        printf("Game Conclusion: Player %c has Won to resignation!\n", OtherPlayerNumeration);

        Out.Return = 1;
    }

    if (Input == 'A') {
        if (Player == StartingPlayer) {
            printf("What would you like to respond with regarding the possibility of declination? (X O A E) ");

            memset(Filter, 0, Check);
            scanf("%s", Filter);
        } else {
            Filter[0] = GameBot(Position, 1, 1);
        }

        const char Disregards = Filter[0];

        if (Player == StartingPlayer) {
            Filter[0] = GameBot(Position, 0, 0);
        } else {
            printf("Player %c, would you like to accept a draw? (A D) ", OtherPlayerNumeration);

            memset(Filter, 0, Check);
            scanf("%s", Filter);
        }

        char AcceptanceInput = Filter[0];

        if (AcceptanceInput == 'a') AcceptanceInput = 'A';
        if (AcceptanceInput == 'd') AcceptanceInput = 'D';

        if (AcceptanceInput == 'A') {
            printf("\nGame Conclusion: Player %c accepted a draw offer. The game ends in a tie!", OtherPlayerNumeration);

            Out.Return = 1;
        }

        if (AcceptanceInput == 'D') {
            printf("Player %c declined a draw offer!\n", OtherPlayerNumeration);

            free(Filter);

            Out = HandleInput(StartingPlayer, Player, Position, Disregards, 0);
        }
    }

    if (Input != 'X' && Input != 'O' && Input != 'E' && Input != 'A') {
        printf("No option is case sensitive. Please pick either X or an O. You can resign the game with an E. Ask for a draw with an A.\n");

        free(Filter);

        Out = HandleInput(StartingPlayer, Player, Position, 0, 0);
    }

    Filter = NULL;

    return Out;
}

static int ScrollList(const Game Game, const _Bool* Pattern, const int SequenceLength) {
    int PatternIndex = 0;

    while (PatternIndex < Game.Path.Count) {
        // Iterating through every pattern, skip the ones that do not match ours

        if (EqualPatterns(Game.List[PatternIndex].Pattern, Pattern, SequenceLength) == 1) return PatternIndex;

        PatternIndex++;
    }

    return -1;
}

void InsertOccurrence(Game* Game, _Bool* Pattern, const int SequenceLength) {
    const int PatternIndex = ScrollList(*Game, Pattern, SequenceLength);

    // Make sure it's not a new pattern
    if (PatternIndex >= 0) {
        // If we hit, just increase the appearances and exit function
        Game->List[PatternIndex].Appearances++;

        return;
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

        Back--;
    }
}

void UpdateGame(Set* Element, const _Bool Play) {
    AddSpot(&(*Element).Home, Play);

    QuickAdd(&(*Element).Player1, (*Element).Home, (*Element).Player1Settings.Length);
    QuickAdd(&(*Element).Player2, (*Element).Home, (*Element).Player2Settings.Length);
}

GameConclude Simulate(const Position Copy, const _Bool* Sequence, const _Bool WeStart) {
    const int SequenceLength = (int) sizeof(*Sequence) / sizeof(_Bool);

    // Get the simulation
    Set Element = Hold();

    // Copy "Copy" into Element in a way to create the line identically in two separate spaces in memory
    free(Element.Home.Line);

    Element.Home.Path = Copy.Path;
    Element.Home.Line = calloc(Element.Home.Path.Limit, sizeof(_Bool));

    Dump(Element.Home.Path, sizeof(_Bool), (void**) &Element.Home.Line, (void**) &Copy.Line);
    memcpy(Element.Home.Line, Copy.Line, Element.Home.Path.Count * sizeof(_Bool));

    // *Sub-Comment*: Making the game a bit harder on compute by passing less through "Simulate" to use.
    ModifyList(Element.Home, &Element.Player1, Element.Player1Settings.Length, Element.Home.Path.Count);
    ModifyList(Element.Home, &Element.Player2, Element.Player2Settings.Length, Element.Home.Path.Count);

    // Asserts sequence
    int PatternIndex = 0;

    while (PatternIndex < SequenceLength) {
        AddSpot(&Element.Home, Sequence[PatternIndex]);

        PatternIndex++;
    }

    // Calls off the function (finish later)
    Release(Element.Home.Line, &Element.Player1, &Element.Player2);

    GameConclude Conclusion = { 0 };

    Conclusion = GameEnding(Element.Player1, Element.Player2, Element.Player1Settings, Element.Player2Settings, WeStart);

    return Conclusion; // Template return
}

char GameBot(Position Position, const _Bool ConsiderDraw, const _Bool DrawExhausted) {
    if (Position.BotLevel == -3) {
        return 'E';
    }
    if (Position.BotLevel == -2) {
        if (DrawExhausted) {

        }
    }
    if (Position.BotLevel == -1) {

    }
    if (Position.BotLevel == 0) {
        if (!ConsiderDraw) return 'X';
        return 'A';
    }
    if (Position.BotLevel == 1) {
        if (!ConsiderDraw) {
            if (Position.Line[Position.Path.Count - 2] != 0) return 'O';
            return 'X';
        }
        return 'D';
    }
    if (Position.BotLevel == 2) {

    }
    if (Position.BotLevel == 3) {

    }

    printf("Invalid Bot Parameters");
    return 0;
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

GameConclude GameEnding(const Game Player1, const Game Player2, const Settings Player1Settings, const Settings Player2Settings, const _Bool AssumeStart) {
    GameConclude Conclusion = { 0 };

    const GameResult Player1Won = MetOccurrence(Player1, Player1Settings.Repeats);
    const GameResult Player2Won = MetOccurrence(Player2, Player2Settings.Repeats);

    if (Player1Won.End || Player2Won.End) Conclusion.End = 1;
    if (Player1Won.End && Player2Won.End) Conclusion.Drew = 1;

    // Idea: (Player2Won.End != AssumeStart && !Starter || Player1Won.End != AssumeStart && Starter)
    if (Player2Won.End != AssumeStart && Player1Won.End != Player2Won.End) Conclusion.WeWon = 1;

    Conclusion.Player1Pattern = Player1Won.Pattern;
    Conclusion.Player2Pattern = Player2Won.Pattern;

    return Conclusion;
}

void OutputResult(const Position Position, const GameConclude Result, const Settings Player1Settings, const Settings Player2Settings, const _Bool StartingPlayer, const _Bool Player) {
    const char PlayerNumeration = TwoWayConversion(Player, '1', 1, '2', 0);
    const char PlayerFinisherNumeration = TwoWayConversion(StartingPlayer, '1', 1, '2', 0);

    printf("\nGame Conclusion: ");

    if (Result.Drew) {
        if (StartingPlayer != Player) printf("They");
        if (StartingPlayer == Player) printf("We");

        printf(" Drew! (Player ");
        printf("%c", PlayerFinisherNumeration);
        printf(")");

        printf("\n Player 1: ");
        PrintPattern(Result.Player1Pattern, Player1Settings.Length);

        printf("\n Player 2: ");
        PrintPattern(Result.Player2Pattern, Player2Settings.Length);
    } else {
        printf("We (Player ");
        printf("%c", PlayerNumeration);
        printf(") ");

        if (Result.WeWon == 1) printf("Won");
        if (Result.WeWon == 0) printf("Lost");

        printf(" with ");

        const _Bool Outcome = (Result.WeWon == 1 && Player == 0) || (Result.WeWon == 0 && Player == 1);

        if (Outcome == 1) PrintPattern(Result.Player2Pattern, Player2Settings.Length);
        if (Outcome == 0) PrintPattern(Result.Player1Pattern, Player1Settings.Length);

        printf("!\nThe game was finished by Player ");
        printf("%c", PlayerFinisherNumeration);
        printf("!");
    }
}