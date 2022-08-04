#ifndef MUSIC_H
#define MUSIC_H

#define FACTOR      1193180

// Pitches
typedef enum pitch {
    PITCH_C_F = 19327,
    PITCH_C   = 18242,
    PITCH_C_S = 17218,
    PITCH_D   = 16252,
    PITCH_D_S = 15340,
    PITCH_E   = 14479,
    PITCH_F   = 13667,
    PITCH_F_S = 12899,
    PITCH_G   = 12175,
    PITCH_G_S = 11492,
    PITCH_A   = 10847,
    PITCH_A_S = 10238,
    PITCH_B   = 9664,
    PITCH_B_S = 9121,
    INTERRUPT = 0,
    REST = -1
} pitch_t;

typedef enum octave {
    OCTAVE_BASS    = 1,
    OCTAVE_LOW     = 2,
    OCTAVE_MID     = 3,
    OCTAVE_HIGH    = 4
} octave_t;

typedef enum length {
    WHOLE       = 4000,
    HALF        = 2000,
    QUARTER     = 1000,
    EIGHTH       = 500,
    SIXTEENTH   = 250
} length_t;

typedef struct note {
    pitch_t pitch;
    octave_t octave;
    length_t length;
} note_t;

void play_note(note_t note, int bpm);

#endif
