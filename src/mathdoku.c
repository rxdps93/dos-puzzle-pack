#include "puzzles.h"

static const int init(void) {
    return 0;
}

static const char *description(void) {
    return "Ensure every row and column contain each number only once using arithmetic problems as clues!";
}

const struct puzzle_vtable MATHDOKU[] = { { init, description } };
