#include "libs/run.h"

int main() {
    _Bool* Line = calloc(1, sizeof(_Bool));
    Position Player1 = {
        .Line = Line,
        .Path = {
            .Count = 0,
            .Limit = 1
        }
    };
    Position Player2 = {
        .Line = Line,
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