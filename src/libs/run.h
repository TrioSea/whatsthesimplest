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
    _Bool* Line;
    SizeTracker Path;
} Position;

typedef struct {
    Occurrence* List;
    SizeTracker Path;
} Game;

_Bool* ReadPattern(const _Bool* Line, size_t PointInLine, int SequenceLength);
void InsertOccurrence(Game* Game, _Bool* Pattern);
void AddSpot(Position *Position, _Bool ADD);
void QuickAdd(Game* Game, Position Position, int SequenceLength);

void ModifyList(Position Position, Game* Game, int SequenceLength, size_t EndAt);
_Bool MetOccurrence(Game Game, int AppearanceRequirement);

void FillSpace(Position Home, Game Player1, Game Player2);

#endif // WITSPG_MAIN_H
