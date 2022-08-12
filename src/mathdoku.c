#include "puzzles.h"

static const char *init(void) {
    return "You are now playing Mathdoku!";
}

static const char *description(void) {
    return "Ensure every row and column contain each number only once using arithmetic problems as clues!";
}

const struct puzzle_vtable MATHDOKU[] = { { init, description } };
