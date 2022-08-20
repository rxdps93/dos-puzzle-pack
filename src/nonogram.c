#include "puzzles.h"

static const int init(void) {
    return 0;
}

static const char *description(void) {
    return "Solve for pictures and designs using deductive reasoning!";
}

const struct puzzle_vtable NONOGRAM[] = { { init, description } };
