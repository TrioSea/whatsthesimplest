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
    unsigned char Repeats;
    unsigned char Length;
} Settings;

typedef struct {
    Occurrence* List;
    SizeTracker Path;
} Game;

Game Pose();

_Bool* ReadPattern(const _Bool* Line, size_t PointInLine, unsigned char SequenceLength);
void AllocateSpace(Game* Game);
void InsertOccurrence(Game* Game, _Bool* Pattern);
void ModifyList(Position Position, Game* Game, unsigned char SequenceLength, size_t EndAt);
_Bool MetOccurrence(Game Game, size_t AppearanceRequirement);

#endif // WITSPG_MAIN_H
