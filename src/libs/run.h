#ifndef WITSPG_MAIN_H
#define WITSPG_MAIN_H

#include <stdio.h>
#include <stdlib.h>

#include "conversions.h"

typedef struct {
    size_t Count;
    size_t Limit;
} SizeTracker;

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

_Bool* ReadPattern(const _Bool* Line, unsigned char PointInLine, unsigned char LengthOfPattern);
_Bool NewPattern(Game Game, const _Bool* Pattern);
void InsertOccurrence(Game* Game, _Bool* Pattern);
_Bool MetOccurrence(Game Game, size_t AppearanceRequirement);

#endif //WITSPG_MAIN_H
