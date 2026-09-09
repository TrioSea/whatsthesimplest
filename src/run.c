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
    // for the 2, it was vibe code fixed and same for %s -> %1s for all scanf
    const size_t Check = 2;
    char* Filter = malloc(Check * sizeof(char));

    char Input = Disregard;

    _Bool DrawAvailable = 0;

    const char PlayerNumeration = TwoWayConversion(StartingPlayer, '1', 1, '2', 0);
    const char OtherPlayerNumeration = TwoWayConversion(StartingPlayer, '2', 1, '1', 0);

    if (AND(Input == 0, Override == 0)) {
        // print out to the player for input

        printf("Player ");
        printf("%c", PlayerNumeration);
        printf("; ");

        // reiterate the line to the player
        PrintLine(Position);

        DrawAvailable = 1;

        memset(Filter, 0, Check);
        scanf("%1s", Filter);

        Input = Filter[0];
    }

    if (Override != 0) Input = Override;

    IO Out = { 0 };
    
    if (Input == 'x') Input = 'X';
    if (Input == 'o') Input = 'O';
    if (Input == 'e') Input = 'E';

    if (AND(Input == 'A', Invert(DrawAvailable))) Input = 'a';

    if (InvertedAND(Input != 'X', Input != 'O')) {
        Out.Play = (_Bool) TwoWayConversion(Input, 'X', 1, 'O', 0);
    }

    if (Input == 'E') {
        printf("\nGame Conclusion: Player ");
        printf("%c", OtherPlayerNumeration);
        printf(" has Won to resignation!");

        Out.Return = 1;
    }

    if (Input == 'A') {
        const _Bool BotOn = 0; // integrate

        if (InvertedInclusiveOR(Player == StartingPlayer, BotOn)) {
            printf("What would you like to respond with regarding the possibility of declination? (X O E) ");

            memset(Filter, 0, Check);
            scanf("%1s", Filter);
        } else {
            Filter[0] = GameBot(Position, 1, 1);
        }

        const char Disregards = Filter[0];

        if (AND(Player == StartingPlayer, BotOn)) {
            Filter[0] = GameBot(Position, 0, 0);
        } else {
            printf("Player ");
            printf("%c", OtherPlayerNumeration);
            printf(", would you like to accept a draw? (A D) ");

            memset(Filter, 0, Check);
            scanf("%1s", Filter);
        }

        char AcceptanceInput = Filter[0];

        if (AcceptanceInput == 'a') AcceptanceInput = 'A';
        if (AcceptanceInput == 'd') AcceptanceInput = 'D';

        if (AcceptanceInput == 'A') {
            printf("\nGame Conclusion: Player ");
            printf("%c", OtherPlayerNumeration);
            printf(" accepted a draw offer. The game ends in a tie!");

            Out.Return = 1;
        }

        if (AcceptanceInput == 'D') {
            printf("Player ");
            printf("%c", OtherPlayerNumeration);
            printf(" declined a draw offer!\n");

            free(Filter);

            Out = HandleInput(StartingPlayer, Player, Position, Disregards, 0);
        }
    }

    if (AND(AND(AND(Input != 'X', Input != 'O'), Input != 'E'), Input != 'A')) {
        printf("None of the options are case sensitive. Please pick either X or an O. You can resign the game with an E. If you haven't already, asking for a draw is available.\n");

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

void AddSpot(Position* Position, const int ADD) {
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

void Add(Branch** Class, const int SupposedID, const int ADD, Set* Pose) {
    if ((*Class)[SupposedID].FullStack == 1) {
        (*Class)[SupposedID].Options = malloc(RUN_BOT_OPTIONS * sizeof(Branch));
    }

    (*Class)[SupposedID].Options[ADD] = (Branch) {
        .ID = ADD,
        .ParentID = (*Class)[SupposedID].ID,
        .ParentParentCommons = *Class,
        .FullStack = 1,
        .Options = NULL
    };

    (*Class)[SupposedID].FullStack |= 1 << ADD;

    UpdateGame(Pose, ADD);
}

int BestImmediateOption(Branch** UC, const int ID) {
    int Index = 1;

    int CurrentEvaluation = 0;
    int BestMove = 0;

    while (Index < RUN_BOT_OPTIONS + 1) {
        const int PotentialNewEvaluation = (*UC)[ID].Options[Index].Evaluation;

        if (PotentialNewEvaluation >= CurrentEvaluation) {
            CurrentEvaluation = PotentialNewEvaluation;
            BestMove = Index;
        }

        Index++;
    }

    return BestMove;
}

int AverageOption(Branch** UC, const int ID) {
    int Index = 1;

    int CurrentEvaluation = 0;

    while (Index < RUN_BOT_OPTIONS + 1) {
        CurrentEvaluation += (*UC)[ID].Options[Index].Evaluation;

        Index++;
    }

    return CurrentEvaluation / RUN_BOT_OPTIONS;
}

void Out(Branch*** UC, const int SupposedID, const _Bool WeStart, const int Depth) {
    if ((**UC)[SupposedID].ID < RUN_BOT_OPTIONS + 1) {
        *UC = &(**UC)[SupposedID].ParentParentCommons; // reason we have triple pointer
        int Eval = 0;
        const _Bool StarterMove = (double) Depth / 2 == floor((double) Depth / 2);
        if (InvertedExclusiveOR(StarterMove, WeStart)) {
            Eval = BestImmediateOption(*UC, SupposedID);
        } else {
            Eval = AverageOption(*UC, SupposedID);
        }
        (**UC)[SupposedID].Evaluation = Eval;
        Out(UC, SupposedID, WeStart, Depth);
    }
}

int Initiate(Branch** UC, int ID, int ADD, Set Pose, const _Bool Create, const _Bool WeStart, int Depth) {
    if (Create) Pose = Hold();
    Add(UC, ID, ADD, &Pose);
    UC = &(*UC)[ID].Options;
    ID = 1;

    const GameConclude Occurrence = GameEnding(Pose.Player1, Pose.Player2, Pose.Player1Settings, Pose.Player2Settings, WeStart);
    if (Occurrence.End) {
        (*UC)[ID].Evaluation = (int) InvertedExclusiveOR(Occurrence.WeWon, WeStart) << 16;

        Out(&UC, ID, WeStart, Depth);

        if ((*UC)[ID].ID != 0) {
            *UC = (*UC)[ID].ParentParentCommons;
            Depth--;
            ADD++;
            Initiate(UC, ID, ADD, Pose, 0, WeStart, Depth);
        } else {
            Release(Pose.Home.Line, &Pose.Player1, &Pose.Player2);
            return BestImmediateOption(UC, ID);
        }
    } else {
        ADD = 1;
        Depth++;
        Initiate(UC, ID, ADD, Pose, 0, WeStart, Depth);
    }
}

void Pass(Branch** Operational, int* ON) {
    if (*ON < 4) {
        if ((*Operational)->Options != NULL) {
            *Operational = &(*Operational)->Options[*ON];
            Sweep(Operational, ON);
        } else {
            *Operational = (*Operational)->ParentParentCommons[(*Operational)->ParentID].Options;

            free(*Operational);
            *Operational = NULL;
        }
    } else {
        *ON = (*Operational)->ParentID;

        (*ON)++;
        *Operational = &(*Operational)->ParentParentCommons[*ON];

        Pass(Operational, ON);
    }
}

void Sweep(Branch** Operational, int* ON) {
    *Operational = &(*Operational)->Options[*ON];
    if (AND((*Operational)->Options == NULL, *ON < 4)) { // we can catch all because thats how we build our functions
        *ON = 1;
        Pass(Operational, ON);
    } else {
        // slide over

        *Operational = &(*Operational)->ParentParentCommons[(*Operational)->ParentID];

        (*ON)++;
        *Operational = &(*Operational)->Options[*ON];
        Sweep(Operational, ON);
    }
}

void ModifyList(const Position Position, Game* Game, const int SequenceLength, const size_t EndAt) {
    if (InclusiveOR(EndAt < Position.Path.Count, EndAt < SequenceLength)) return;

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

void UpdateGame(Set* Element, const int Play) {
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

char GameBot(const Position Position, const _Bool ConsiderDraw, const _Bool DrawExhausted) {
    if (Position.BotLevel == 3) {
        const double h = (double) Position.Path.Count / 2;
        const _Bool WeStart = h == floor(h);

        Branch* Class = malloc(sizeof(Branch));

        Class[0] = (Branch) {
            .ID = 0,
            .ParentID = 0,
            .ParentParentCommons = Class,
            .FullStack = 1,
            .Options = NULL
        };

        const int BestMove = Initiate(&Class, 0, 1, (Set) { 0 }, 1, WeStart, 0);

        Branch* Operational = &Class[0];
        int ON = 0;

        Pass(&Operational, &ON);

        const int Bindings = 4;
        Bind* Bounded = calloc(Bindings, sizeof(Bind));

        Bounded[0] = (Bind) {
            .Character = 'X',
            .Numeral = 1
        };

        Bounded[1] = (Bind) {
            .Character = 'O',
            .Numeral = 2
        };

        Bounded[2] = (Bind) {
            .Character = 'A',
            .Numeral = 3
        };

        Bounded[3] = (Bind) {
            .Character = 'E',
            .Numeral = 4
        };

        const char Given = Convert((char) BestMove, Bindings, Bounded);
        free(Bounded);

        return Given;
    }

    printf("Invalid Bot Parameters");
    return 'E';
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

    if (InclusiveOR(Player1Won.End, Player2Won.End)) Conclusion.End = 1;
    if (AND(Player1Won.End, Player2Won.End)) Conclusion.Drew = 1;

    // Idea: (InclusiveOR(ADD(Player2Won.End != AssumeStart, !Starter), AND(Player1Won.End != AssumeStart, Starter)))
    if (AND(Player2Won.End != AssumeStart, Player1Won.End != Player2Won.End)) Conclusion.WeWon = 1;

    Conclusion.Player1Pattern = Player1Won.Pattern;
    Conclusion.Player2Pattern = Player2Won.Pattern;

    return Conclusion;
}

void OutputResult(const GameConclude Result, const Settings Player1Settings, const Settings Player2Settings, const _Bool StartingPlayer, const _Bool Player) {
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

        const _Bool Outcome = ExclusiveOR(Result.WeWon, Player);

        if (Outcome == 1) PrintPattern(Result.Player2Pattern, Player2Settings.Length);
        if (Outcome == 0) PrintPattern(Result.Player1Pattern, Player1Settings.Length);

        printf("!\nThe game was finished by Player ");
        printf("%c", PlayerFinisherNumeration);
        printf("!");
    }
}