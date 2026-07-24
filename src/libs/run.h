#ifndef WITSPG_MAIN_H
#define WITSPG_MAIN_H

#include "strings.h"
#include "conversions.h"

typedef struct {
    _Bool* Pattern;
    size_t Appearances;
} Occurrence;

typedef struct {
    int Repeats;
    int Length;
} Settings;

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
    _Bool* Line;
    SizeTracker Path;
} Position;

typedef struct {
    Occurrence* List;
    SizeTracker Path;
} Game;

_Bool* ReadPattern(const _Bool* Line, size_t PointInLine, int SequenceLength);
void PrintPattern(const _Bool* Pattern, int SequenceLength);
_Bool EqualPatterns(const _Bool* PatternA, const _Bool* PatternB, int SequenceLength);


IO HandleInput(_Bool StartingPlayer, Position Position, char Disregard);

void InsertOccurrence(Game* Game, _Bool* Pattern, int SequenceLength);
void AddSpot(Position *Position, _Bool ADD);
void QuickAdd(Game* Game, Position Position, int SequenceLength);

void ModifyList(Position Position, Game* Game, int SequenceLength, size_t EndAt);
GameConclude Simulate(Position Position, const _Bool* Pattern);

GameResult MetOccurrence(Game Game, int AppearanceRequirement);
GameConclude GameEnding(Game Player1, Game Player2, Settings Player1Settings, Settings Player2Settings, _Bool WeStart);
void OutputResult(GameConclude Result, Settings Player1Settings, Settings Player2Settings, _Bool StartingPlayer);



#endif // WITSPG_MAIN_H
