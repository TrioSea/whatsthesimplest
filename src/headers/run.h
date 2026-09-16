#ifndef WITSPG_MAIN_H
#define WITSPG_MAIN_H

#include "logic.h"
#include "conversions.h"

typedef struct {
    _Bool* Pattern;
    _Bool End;
} GameResult;

typedef struct {
    _Bool* Player1Pattern;
    _Bool* Player2Pattern;
    _Bool WeWon;
    _Bool Drew;
    _Bool End;
    _Bool Error;
    int Point;
} GameConclude;

typedef struct {
    _Bool Return;
    char Play;
} IO;

typedef struct {
    int Repeats;
    int Length;
} Settings;

typedef struct {
    SizeTracker Path;
    char* Line;

    int BotLevel;
} Position;

typedef struct {
    _Bool* Pattern;
    size_t Appearances;
} Occurrence;

typedef struct {
    Occurrence* List;
    SizeTracker Path;
} Game;

typedef struct {
    Position Home;

    Game Player1;
    Game Player2;

    Table Table2;
    Table Table4;

    Settings Player1Settings;
    Settings Player2Settings;

    int ErrorCode;

    _Bool Full;
} Set;

typedef struct {
    Table Table2;
    Table Table4;
} MainTables;

typedef struct Branch Branch;

typedef struct Branch {
    Branch* ParentParentCommons;

    Branch* Options;
    long long int FullStack;

    int ParentID;
    int ID;

    int Evaluation;
} Branch;



Set Hold();
MainTables Tables();
void Release(char* Line, Game* Player1, Game* Player2);
void Debunk(MainTables Tables);

void SwapState(_Bool* State);

GameResult ReadPattern(Position Position, size_t PointInLine, int SequenceLength, Table Table4);
void PrintPattern(const _Bool* Pattern, int SequenceLength, Table Table4);
_Bool EqualPatterns(const _Bool* PatternA, const _Bool* PatternB, int SequenceLength);


void PrintLine(Position Position, Table Table4);
IO HandleInput(Set Stance, _Bool Player, char Disregard, char Override, _Bool DrawExhausted, Table Table2, Table Table4);

void InsertOccurrence(Game* Game, _Bool* Pattern, int SequenceLength);
void AddSpot(Position *Position, char ADD);
void QuickAdd(Game* Game, Position Position, int SequenceLength, Table Table4);

GameConclude GameEnding(Game Player1, Game Player2, Settings Player1Settings, Settings Player2Settings, _Bool AssumeStart);
void Add(Branch** Class, int SupposedID, char ADD, Set* Pose, Table Table4);

#define RUN_BOT_OPTIONS 4

int BestImmediateOption(Branch** UC, int ID);
int AverageOption(Branch** UC, int ID);

void Out(Branch*** UC, int SupposedID, _Bool WeStart, int Depth);
int Initiate(Branch** UC, int ID, char ADD, Set Pose, _Bool Create, _Bool WeStart, int Depth, Table Table4);

void Pass(Branch** Operational, int* ON);
void Sweep(Branch** Operational, int* ON);

void ModifyList(Position Position, Game* Game, int SequenceLength, size_t EndAt, Table Table4);
void UpdateGame(Set* Element, char Play, Table Table4);
GameConclude Simulate(Position Copy, const _Bool* Sequence, _Bool WeStart, Table Table4);
char GameBot(Set Stance, _Bool ConsiderDraw, _Bool DrawExhausted, Table Table4);

GameResult MetOccurrence(Game Game, int AppearanceRequirement);
void OutputResult(GameConclude Result, Settings Player1Settings, Settings Player2Settings, _Bool StartingPlayer, _Bool Player, Table Table2, Table Table4);

#endif // WITSPG_MAIN_H
