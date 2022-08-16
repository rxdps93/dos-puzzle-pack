#include "puzzles.h"
#include <time.h>
#include <graph.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#define WORD_LEN    5

int count_lines(FILE *fp) {
    int c;
    int count = 0;
    for (c = getc(fp); c != EOF; c = getc(fp)) {
        if (c == '\n') {
            count++;
        }
    }
    return count;
}

char *get_word(FILE *fp, int line) {
    fseek(fp, line * 6, SEEK_SET);
    char *word = malloc(6 * sizeof(char));
    fgets(word, 6, fp);
    return word;
}

void enter_guess(char *guess) {

    _settextposition(5, 13);
    // _outtext("_ _ _ _ _");

    for (int letter = 0; letter < WORD_LEN; letter++) {
        putch('_');
    }

    int letter = 0;
    int c;
    int loop = 1;
    _settextposition(5, 13);
    do {
        c = getch();
        if (c == 0) {
            getch();
        }

        if (letter < WORD_LEN) {
            if ((c >= 'a') && (c <= 'z')) {
                c -= 32;
            }

            if ((c >= 'A') && (c <= 'Z')) {
                _settextposition(5, 13 + letter);
                putch(c);
                guess[letter++] = c;
            }
        }

        if (c == 8) {
            guess[letter--] = '\0';

            if (letter < 0) {
                letter = 0;
            }

            _settextposition(5, 13 + letter);
            putch('_');
            _settextposition(5, 13 + letter);
        }

        if ((c == 13) && (letter == WORD_LEN)) {
            loop = 0;
        }
        
    } while (loop);

    guess[WORD_LEN] = '\0';
}

void play(char *word) {
    _settextwindow(1, 1, 25, 80);
    _setbkcolor(7);
    _clearscreen(_GWINDOW);

    /* Title Bar */
    _settextwindow(1, 1, 1, 80);
    _setbkcolor(3);
    _clearscreen(_GWINDOW);
    _settextcolor(15);
    _settextposition(1, 24);
    _outtext("WORDOS: Solve for the Secret Word!");

    /* Test Area */
    _settextwindow(4, 25, 10, 56);
    _setbkcolor(1);
    _clearscreen(_GWINDOW);
    _settextcolor(15);
    _settextposition(2, 5);
    _outtext("Please enter your guess:");

    char guess[WORD_LEN + 1];
    enter_guess(&guess);

    _settextposition(6, 13);
    _settextcolor(3);
    _outtext(guess);

    getch();
}

static const char *init(void) {
    srand(time(NULL));

    FILE *ans;
    ans = fopen("answers.txt", "r");
    if (ans != NULL) {
        int line = rand() % count_lines(ans);
        char *word = get_word(ans, line);
        play(word);
        free(word);
        fclose(ans);
    }

    return "You are now playing WORDOS!";
}

static const char *description(void) {
    return "Solve for the secret 5-letter word in 6 guesses or less with each guess narrowing down the possibilities!";
}

const struct puzzle_vtable WORDOS[] = { { init, description } };
