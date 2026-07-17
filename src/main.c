#include "libs/run.h"

int main() {
    _Bool* Line = calloc(0, sizeof(_Bool));
    _Bool** Reference = &Line;
    Position Player1 = {
        .Line = *Reference,
        .Path = {
            .Count = 0,
            .Limit = 1
        }
    };
    Position Player2 = {
        .Line = *Reference,
        .Path = {
            .Count = 0,
            .Limit = 1
        }
    };

    Line[0] = Convert((char) getchar());

    printf("%d", Line[0]);

    free(Line);

    return 0;
}