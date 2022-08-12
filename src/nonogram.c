#include "puzzles.h"

static const char *init(void) {
    return "You are now playing Nonogram!";
}

static const char *description(void) {
    return "Solve for pictures and designs using deductive reasoning!";
}

const struct puzzle_vtable NONOGRAM[] = { { init, description } };
