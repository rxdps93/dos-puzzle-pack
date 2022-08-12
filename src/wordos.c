#include "puzzles.h"

static const char *init(void) {
    return "You are now playing WORDOS!";
}

static const char *description(void) {
    return "Solve for the secret 5-letter word in 6 guesses or less with each guess narrowing down the possibilities!";
}

const struct puzzle_vtable WORDOS[] = { { init, description } };
