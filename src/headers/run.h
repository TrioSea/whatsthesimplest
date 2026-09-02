#ifndef WITSPG_MAIN_H
#define WITSPG_MAIN_H

#include "strings.h"
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
    _Bool Play;
} IO;

typedef struct {
    int Repeats;
    int Length;
} Settings;

typedef struct {
    _Bool* Line;
    SizeTracker Path;
    signed char BotLevel;
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
    Game Player1;
    Game Player2;
    Settings Player1Settings;
    Settings Player2Settings;
    _Bool Full;
    Position Home;
    int ErrorCode;
} Set;

Set Hold();
void Release(_Bool* Line, Game* Player1, Game* Player2);

void SwapState(_Bool* State);

_Bool* ReadPattern(const _Bool* Line, size_t PointInLine, int SequenceLength);
void PrintPattern(const _Bool* Pattern, int SequenceLength);
_Bool EqualPatterns(const _Bool* PatternA, const _Bool* PatternB, int SequenceLength);


void PrintLine(Position Position);
IO HandleInput(_Bool StartingPlayer, _Bool Player, Position Position, char Disregard, char Override);

void InsertOccurrence(Game* Game, _Bool* Pattern, int SequenceLength);
void AddSpot(Position *Position, _Bool ADD);
void QuickAdd(Game* Game, Position Position, int SequenceLength);

void ModifyList(Position Position, Game* Game, int SequenceLength, size_t EndAt);
void UpdateGame(Set* Element, _Bool Play);
GameConclude Simulate(Position Copy, const _Bool* Sequence, _Bool WeStart);
char GameBot(Position Position, _Bool Playing, _Bool DrawExhausted);

GameResult MetOccurrence(Game Game, int AppearanceRequirement);
GameConclude GameEnding(Game Player1, Game Player2, Settings Player1Settings, Settings Player2Settings, _Bool AssumeStart);
void OutputResult(Position Position, GameConclude Result, Settings Player1Settings, Settings Player2Settings, _Bool StartingPlayer, _Bool Player);

#endif // WITSPG_MAIN_H
