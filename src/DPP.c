#include <stdio.h>
#include "curses.h"
#include "puzzles.h"

int main(void) {

    struct puzzle wordos  = { WORDOS, "WORDOS" };
    struct puzzle nonogram = { NONOGRAM, "NONOGRAM" };
    struct puzzle mathdoku = { MATHDOKU, "MATHDOKU" };

    printf("%s: %s\n", wordos.name, puzzle_description(&wordos));
    printf("%s: %s\n", nonogram.name, puzzle_description(&nonogram));
    printf("%s, %s\n", mathdoku.name, puzzle_description(&mathdoku));

    return 0;
}
