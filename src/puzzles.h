#ifndef PUZZLES_H_
#define PUZZLES_H_

typedef struct puzzle {
    const struct puzzle_vtable *vtable_;
    const char *name;
} puzzle_t;

struct puzzle_vtable {
    const int (*init)(void);
    const char *(*description)(void);
};

static inline const int init_puzzle(puzzle_t *puzzle) {
    return puzzle->vtable_->init();
}

static inline const char *puzzle_description(puzzle_t *puzzle) {
    return puzzle->vtable_->description();
}

extern const struct puzzle_vtable 
    WORDOS[],
    NONOGRAM[],
    MATHDOKU[];

#endif
