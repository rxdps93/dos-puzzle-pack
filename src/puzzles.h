#ifndef PUZZLES_H_
#define PUZZLES_H_

struct puzzle {
    const struct puzzle_vtable *vtable_;
    const char *name;
};

struct puzzle_vtable {
    const char *(*description)(void);
};

static inline const char *puzzle_description(struct puzzle *puzzle) {
    return puzzle->vtable_->description();
}

extern const struct puzzle_vtable 
    WORDOS[],
    NONOGRAM[],
    MATHDOKU[];

#endif
