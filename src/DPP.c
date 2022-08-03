#include <stdio.h>
#include <stdlib.h>
#include <i86.h>

unsigned short notes[] = {
        19327 ,        /* C b             */
        18242 ,        /* C               */
        17218 ,        /* C #   ( D b )  */
        16252 ,        /* D               */
        15340 ,        /* D #   ( E b )  */
        14479 ,        /* E      ( F b )  */
        13666 ,        /* F      ( E # )  */
        12899 ,        /* F #   ( G b )  */
        12175 ,        /* G               */
        11492 ,        /* G #   ( A b )  */
        10847 ,        /* A               */
        10238 ,        /* A #   ( B b )  */
        9664 ,         /* B      ( C b )  */
        9121 ,         /* B #             */
        0
};

#define FACTOR  1193180
#define OCTAVE  4

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

    if (selection != 4) {
        printf("We are playing %s\n", opts[selection - 1]);
    } else {
        printf("Bye!\n");
    }

    for(int i = 0; notes[i]; ++i) {
        sound( FACTOR / (notes[i] / (1 << OCTAVE)) );
        delay(125);
        nosound();
    }

    return 0;
}
