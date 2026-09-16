#include "headers/run.h"

Set Hold() {
    Set Element = (Set) {
        .Player1 = (Game) {
            .List = (Occurrence*) calloc(0, sizeof(Occurrence)),
            .Path = (SizeTracker) { 0 }
        },
        .Player2 = (Game) {
            .List = (Occurrence*) calloc(0, sizeof(Occurrence)),
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
        .Full = (_Bool) 1,
        .Home = (Position) {
            .Line = (char*) calloc(0, sizeof(char)),
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

MainTables Tables() {
    MainTables Element = (MainTables) {
        .Table2 = (Table) {
            .Bindings = 2,
            .Bounded = calloc(2, sizeof(Bind)) // repeat bindings
        },
        .Table4 = (Table) {
            .Bindings = 4,
            .Bounded = calloc(4, sizeof(Bind))
        }
    };

    Element.Table2.Bounded[0] = (Bind) {
        .Character = '1',
        .Numeral = 1
    };
    Element.Table2.Bounded[1] = (Bind) {
        .Character = '2',
        .Numeral = 0
    };

    Element.Table4.Bounded[0] = (Bind) {
        .Character = 'X',
        .Numeral = 1
    };
    Element.Table4.Bounded[1] = (Bind) {
        .Character = 'O',
        .Numeral = 2
    };
    Element.Table4.Bounded[2] = (Bind) {
        .Character = 'A',
        .Numeral = 3
    };
    Element.Table4.Bounded[3] = (Bind) {
        .Character = 'E',
        .Numeral = 4
    };

    FallBack Error;

    Error = CheckNewlyAllocated(Element.Table2.Bounded);
    if (Error.ReturnCode == 1) return (MainTables) { 0 };

    Error = CheckNewlyAllocated(Element.Table4.Bounded);
    if (Error.ReturnCode == 1) return (MainTables) { 0 };

    return Element;
}

void Release(char* Line, Game* Player1, Game* Player2) {
    free(Line);
    Line = NULL;

    // vibe corrected set to 0
    int Beta = 0;
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

void Debunk(MainTables Tables) {
    free(Tables.Table2.Bounded);
    Tables.Table2.Bounded = NULL;

    free(Tables.Table4.Bounded);
    Tables.Table4.Bounded = NULL;
}

void SwapState(_Bool* State) {
    if (*State == 0) {
        *State = 1;
        return;
    }
    *State = 0;
}

GameResult ReadPattern(const Position Position, size_t PointInLine, const int SequenceLength, const Table Table4) {
    _Bool* Pattern = calloc(SequenceLength, sizeof(_Bool)); // Clear up a pattern
    const FallBack Error = CheckNewlyAllocated(Pattern);

    const GameResult DefaultReturn = (GameResult) {
        .Pattern = NULL,
        .End = 1
    };

    if (Error.ReturnCode == 1) return DefaultReturn;

    int SequenceIndex = 0; // Incremental Looping Index

    while (SequenceIndex < SequenceLength) {
        if (PointInLine + SequenceIndex < Position.Path.Count) {
            const char Written = Position.Line[PointInLine + SequenceIndex];
            const _Bool Invalid = InvertedAND(InvertedAND(Written != 'E', Written != 'A'), Written != 'D');
            if (Invalid) {
                PointInLine++;
                continue;
            }

            Pattern[SequenceIndex] = (_Bool) Convert(Written, Table4); // Translate pattern from the line directly
            SequenceIndex++;
        } else {
            free(Pattern);
            Pattern = NULL;

            return DefaultReturn;
        }
    }

    return (GameResult) {
        .Pattern = Pattern,
        .End = 0
    };
}

void PrintPattern(const _Bool* Pattern, const int SequenceLength, const Table Table4) {
    int PatternIndex = 0;
    while (PatternIndex < SequenceLength) {
        printf("%c", Convert(Pattern[PatternIndex], Table4));
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



void PrintLine(const Position Position, const Table Table4) {
    int ThroughLine = 0;
    while (ThroughLine < Position.Path.Count) {
        const char PreviousInput = Convert(Position.Line[ThroughLine], Table4);
        printf("%c", PreviousInput);

        ThroughLine++;
    }
}

IO HandleInput(const Set Stance, const _Bool Player, const char Disregard, const char Override, _Bool DrawExhausted, const Table Table2, const Table Table4) {
    // for the 2, it was vibe corrected and same for %s -> %1s for all scanf
    const size_t Check = 2 * sizeof(char);
    char* Filter = malloc(Check);

    char Input = Disregard;

    const char PlayerNumeration = Convert(Stance.Full, Table2);
    const char OtherPlayerNumeration = Convert(Invert(Stance.Full), Table2);

    if (Override != 0) {
        if (Input != 0) DrawExhausted = 0;
        Input = Override;
    } else if (Input == 0) {
        // print out to the player for input

        printf("Player ");
        printf("%c", PlayerNumeration);
        printf("; ");

        // reiterate the line to the player
        PrintLine(Stance.Home, Table4);

        memset(Filter, 0, Check);
        scanf("%1s", Filter);

        Input = Filter[0];
    }

    IO Out = { 0 };
    
    if (Input == 'x') Input = 'X';
    if (Input == 'o') Input = 'O';
    if (Input == 'e') Input = 'E';
    if (Input == 'a') Input = 'A';

    // vibe recorrected
    if (AND(Input == 'A', DrawExhausted)) Input = 0;

    if (InvertedAND(Input != 'X', Input != 'O')) {
        Out.Play = Input; // Simplified to _Bool in UpdateGame()
    }

    if (Input == 'E') {
        printf("\nGame Conclusion: Player ");
        printf("%c", OtherPlayerNumeration);
        printf(" has Won to resignation!");

        Out.Return = 1;
    }

    if (Input == 'A') {
        const _Bool BotOn = 0; // integrate

        if (InvertedInclusiveOR(Player == Stance.Full, BotOn)) {
            printf("What would you like to respond with regarding the possibility of declination? (X O E) ");

            memset(Filter, 0, Check);
            scanf("%1s", Filter);
        } else {
            Filter[0] = GameBot(Stance, 1, 1, Table4);
        }

        const char Disregards = Filter[0];

        if (AND(Player == Stance.Full, BotOn)) {
            Filter[0] = GameBot(Stance, 0, 0, Table4);
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

            DrawExhausted = 1;

            Out = HandleInput(Stance, Player, Disregards, 0, DrawExhausted, Table2, Table4);
        }
    }

    if (AND(AND(AND(Input != 'X', Input != 'O'), Input != 'E'), InclusiveOR(Input != 'A', DrawExhausted))) {
        printf("None of the options are case sensitive. Please pick either X or an O. You can resign the game with an E. If you haven't already, asking for a draw is available.\n");

        free(Filter);

        Out = HandleInput(Stance, Player, 0, 0, DrawExhausted, Table2, Table4);
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

void AddSpot(Position* Position, const char ADD) {
    Pave(&Position->Path, (void**) &Position->Line, sizeof(_Bool));

    Position->Line[Position->Path.Count] = ADD;
    Position->Path.Count++;
}

void QuickAdd(Game* Game, const Position Position, const int SequenceLength, const Table Table4) {
    // Adds Last Occurrence to a list
    if (Position.Path.Count < SequenceLength) return;

    const GameResult Result = ReadPattern(Position, Position.Path.Count - SequenceLength, SequenceLength, Table4);
    if (Invert(Result.End)) InsertOccurrence(Game, Result.Pattern, SequenceLength);
}

void Add(Branch** Class, const int SupposedID, const char ADD, Set* Pose, const Table Table4) {
    if ((*Class)[SupposedID].FullStack == 1) {
        // +1 is vibe corrected
        (*Class)[SupposedID].Options = malloc((RUN_BOT_OPTIONS + 1) * sizeof(Branch));
    }

    (*Class)[SupposedID].Options[Convert(ADD, Table4)] = (Branch) {
        .ID = Convert(ADD, Table4),
        .ParentID = (*Class)[SupposedID].ID,
        .ParentParentCommons = *Class,
        .FullStack = 1,
        .Options = NULL
    };

    (*Class)[SupposedID].FullStack |= 1 << Convert(ADD, Table4);

    UpdateGame(Pose, ADD, Table4);
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

int Initiate(Branch** UC, int ID, char ADD, Set Pose, const _Bool Create, const _Bool WeStart, int Depth, const Table Table4) {
    if (Create) Pose = Hold();

    Add(UC, ID, ADD, &Pose, Table4);
    UC = &(*UC)[ID].Options;
    ID = 1;

    const GameConclude Occurrence = GameEnding(Pose.Player1, Pose.Player2, Pose.Player1Settings, Pose.Player2Settings, WeStart);
    if (Occurrence.End) {
        (*UC)[ID].Evaluation = (int) InvertedExclusiveOR(Occurrence.WeWon, WeStart) << 15;

        Out(&UC, ID, WeStart, Depth);

        if ((*UC)[ID].ID != 0) {
            *UC = (*UC)[ID].ParentParentCommons;
            Depth--;

            const int ADDNumberForm = Convert(ADD, Table4) + 1;
            ADD = Convert((char) ADDNumberForm, Table4);

            Initiate(UC, ID, ADD, Pose, 0, WeStart, Depth, Table4);
        } else {
            Release(Pose.Home.Line, &Pose.Player1, &Pose.Player2);
            return BestImmediateOption(UC, ID);
        }
    } else {
        ADD = Convert(1, Table4);
        Depth++;

        Initiate(UC, ID, ADD, Pose, 0, WeStart, Depth, Table4);
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

void ModifyList(const Position Position, Game* Game, const int SequenceLength, const size_t EndAt, const Table Table4) {
    if (InclusiveOR(EndAt < Position.Path.Count, EndAt < SequenceLength)) return;

    // Set the looping value as a back to front to decrement
    int Back = (int) EndAt - (SequenceLength - 1);

    while (Back >= 0) {
        // Use the function to simplify the obtainance of the pattern
        const GameResult Appear = ReadPattern(Position, Back + 1, SequenceLength, Table4);

        // Use the pattern to only insert itModifyList
        if (Invert(Appear.End)) InsertOccurrence(Game, Appear.Pattern, SequenceLength);

        Back--;
    }
}

void UpdateGame(Set* Element, const char Play, const Table Table4) {
    AddSpot(&(*Element).Home, Play);

    QuickAdd(&(*Element).Player1, (*Element).Home, (*Element).Player1Settings.Length, Table4);
    QuickAdd(&(*Element).Player2, (*Element).Home, (*Element).Player2Settings.Length, Table4);
}

GameConclude Simulate(const Position Copy, const _Bool* Sequence, const _Bool WeStart, const Table Table4) {
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
    ModifyList(Element.Home, &Element.Player1, Element.Player1Settings.Length, Element.Home.Path.Count, Table4);
    ModifyList(Element.Home, &Element.Player2, Element.Player2Settings.Length, Element.Home.Path.Count, Table4);

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

char GameBot(const Set Stance, const _Bool ConsiderDraw, const _Bool DrawExhausted, const Table Table4) {
    if (Stance.Home.BotLevel == 3) {
        const double h = (double) Stance.Home.Path.Count / 2;
        const _Bool WeStart = h == floor(h);

        Branch* Class = malloc(sizeof(Branch));

        Class[0] = (Branch) {
            .ID = 0,
            .ParentID = 0,
            .ParentParentCommons = Class,
            .FullStack = 1,
            .Options = NULL
        };

        const int BestMove = Initiate(&Class, 0, Convert(1, Table4), Stance, 1, WeStart, 0, Table4);

        Branch* Operational = &Class[0];
        int ON = 0;

        Pass(&Operational, &ON);

        return Convert((char) BestMove, Table4);
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

void OutputResult(const GameConclude Result, const Settings Player1Settings, const Settings Player2Settings, const _Bool StartingPlayer, const _Bool Player, const Table Table2, const Table Table4) {
    const char PlayerNumeration = Convert(Player, Table2);
    const char PlayerFinisherNumeration = Convert(StartingPlayer, Table2);

    printf("\nGame Conclusion: ");

    if (Result.Drew) {
        if (StartingPlayer != Player) printf("They");
        if (StartingPlayer == Player) printf("We");

        printf(" Drew! (Player ");
        printf("%c", PlayerFinisherNumeration);
        printf(")");

        printf("\n Player 1: ");
        PrintPattern(Result.Player1Pattern, Player1Settings.Length, Table4);

        printf("\n Player 2: ");
        PrintPattern(Result.Player2Pattern, Player2Settings.Length, Table4);
    } else {
        printf("We (Player ");
        printf("%c", PlayerNumeration);
        printf(") ");

        if (Result.WeWon == 1) printf("Won");
        if (Result.WeWon == 0) printf("Lost");

        printf(" with ");

        const _Bool Outcome = ExclusiveOR(Result.WeWon, Player);

        if (Outcome == 1) PrintPattern(Result.Player2Pattern, Player2Settings.Length, Table4);
        if (Outcome == 0) PrintPattern(Result.Player1Pattern, Player1Settings.Length, Table4);

        printf("!\nThe game was finished by Player ");
        printf("%c", PlayerFinisherNumeration);
        printf("!");
    }
}