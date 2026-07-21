#ifndef WITSPG_MAIN_H
#define WITSPG_MAIN_H

#include "strings.h"
#include "conversions.h"

typedef struct {
    _Bool* Line;
    SizeTracker Path;
} Position;

typedef struct {
    _Bool* Pattern;
    size_t Appearances;
} Occurrence;

typedef struct {
    int Repeats;
    int Length;
} Settings;

typedef struct {
    Occurrence* List;
    SizeTracker Path;
} Game;

_Bool* ReadPattern(const _Bool* Line, size_t PointInLine, int SequenceLength);
void InsertOccurrence(Game* Game, _Bool* Pattern);
void ModifyList(Position Position, Game* Game, int SequenceLength, size_t EndAt);
_Bool MetOccurrence(Game Game, size_t AppearanceRequirement);

#endif // WITSPG_MAIN_H
