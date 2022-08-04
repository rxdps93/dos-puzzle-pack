#include <stdio.h>
#include <stdlib.h>
#include <i86.h>
#include "music.h"

int main(int argc, char **argv) {

    int selection = -1;
    char c;
    char *opts[4] = { "WORDOS", "NONOGRAM", "MATHDOKU", "QUIT" };

    if (argc == 2) {
        selection = atoi(argv[1]);

        if (selection < 1 || selection > 3) {
            printf("Game choice must be 1, 2, or 3.\n");
            return 1;
        }
    } else if (argc > 2) {
        printf("Invalid argument(s)\n");
        return 1;
    }

    if (selection == -1) {
        for (int i = 0; i < 4; i++) {
            printf("%d) %s\n", i + 1, opts[i]);
        }

        do {
            printf("What would you like to do?\n> ");
            c = getc(stdin);
        } while (atoi(&c) < 1 || atoi(&c) > 4);

        selection = atoi(&c);
    }

    // if (selection != 4) {
    //     printf("We are playing %s\n", opts[selection - 1]);
    // } else {
    //     printf("Bye!\n");
    // }

    note_t scale[] = {
        { PITCH_C_F, OCTAVE_HIGH, WHOLE },
        { PITCH_C, OCTAVE_HIGH, HALF },
        { PITCH_C_S, OCTAVE_HIGH, HALF },
        { PITCH_D, OCTAVE_HIGH, QUARTER },
        { PITCH_D_S, OCTAVE_HIGH, QUARTER },
        { PITCH_E, OCTAVE_HIGH, QUARTER },
        { PITCH_F, OCTAVE_HIGH, QUARTER },
        { PITCH_F_S, OCTAVE_HIGH, EIGHTH },
        { PITCH_G, OCTAVE_HIGH, EIGHTH },
        { PITCH_G_S, OCTAVE_HIGH, EIGHTH },
        { PITCH_A, OCTAVE_HIGH, EIGHTH },
        { PITCH_A_S, OCTAVE_HIGH, SIXTEENTH },
        { PITCH_B, OCTAVE_HIGH, SIXTEENTH },
        { PITCH_B_S, OCTAVE_HIGH, SIXTEENTH }
    };

    note_t ss2[] = {
        { PITCH_A, OCTAVE_HIGH, SIXTEENTH },
        { REST, OCTAVE_HIGH, SIXTEENTH},
        { PITCH_E, OCTAVE_HIGH, SIXTEENTH },
        { PITCH_D, OCTAVE_HIGH, SIXTEENTH },
        { PITCH_C_S, OCTAVE_HIGH, EIGHTH },
        { REST, OCTAVE_HIGH, SIXTEENTH},
        { PITCH_C_S, OCTAVE_HIGH, SIXTEENTH },
        { PITCH_D, OCTAVE_HIGH, SIXTEENTH },
        { PITCH_E, OCTAVE_HIGH, SIXTEENTH },
        { PITCH_F_S, OCTAVE_HIGH, SIXTEENTH },
        { PITCH_G_S, OCTAVE_HIGH, SIXTEENTH },
        { PITCH_E, OCTAVE_HIGH, EIGHTH },
        { REST, OCTAVE_HIGH, EIGHTH },

        { PITCH_F_S, OCTAVE_HIGH, SIXTEENTH },
        { REST, OCTAVE_HIGH, SIXTEENTH },
        { PITCH_E, OCTAVE_HIGH, SIXTEENTH },
        { PITCH_D, OCTAVE_HIGH, SIXTEENTH },
        { PITCH_C_S, OCTAVE_HIGH, SIXTEENTH },
        { REST, OCTAVE_HIGH, SIXTEENTH },
        { PITCH_C_S, OCTAVE_HIGH, SIXTEENTH },
        { REST, OCTAVE_HIGH, SIXTEENTH },
        { PITCH_B, OCTAVE_MID, SIXTEENTH },
        { REST, OCTAVE_HIGH, SIXTEENTH },
        { PITCH_C_S, OCTAVE_HIGH, SIXTEENTH },
        { REST, OCTAVE_HIGH, SIXTEENTH },
        { PITCH_D, OCTAVE_HIGH, EIGHTH },
        { REST, OCTAVE_HIGH, EIGHTH}
    };

    for(int i = 0; i < (sizeof scale / sizeof scale[0]); i++) {
        play_note(scale[i], 200);
    }

    for(int i = 0; i < (sizeof ss2 / sizeof ss2[0]); i++) {
        play_note(ss2[i], 100);
    }

    return 0;
}
