#include "puzzles.h"
#include <time.h>
#include <graph.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>

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

    /* Guess Area */
    _settextwindow(3, 2, 7, 25);
    _setbkcolor(1);
    _clearscreen(_GWINDOW);
    _settextcolor(15);
    _settextposition(2, 5);
    _outtext("Enter your guess");

    _settextposition(4, 10);
    for (int letter = 0; letter < WORD_LEN; letter++) {
        putch('_');
    }

    int letter = 0;
    int c;
    int loop = 1;
    _settextposition(4, 10);
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
                _settextposition(4, 10 + letter);
                putch(c);
                guess[letter++] = c;
            }
        }

        if (c == 8) {
            guess[letter--] = '\0';

            if (letter < 0) {
                letter = 0;
            }

            _settextposition(4, 10 + letter);
            putch('_');
            _settextposition(4, 10 + letter);
        }

        if ((c == 13) && (letter == WORD_LEN)) {
            loop = 0;
        }
        
    } while (loop);

    guess[WORD_LEN] = '\0';
}

void title_bar(char *text) {
    _settextwindow(1, 1, 1, 80);
    _setbkcolor(3);
    _clearscreen(_GWINDOW);
    _settextcolor(15);
    // _settextposition(1, 24);
    _settextposition(1, (80 - strlen(text)) / 2);
    _outtext(text);
}

void print_guess(char *guess, int guess_num) {

    int test_colors[5] = { 0, 2, 6, 2, 0 };
    char title[30];
    for (int ltr = 0; ltr < WORD_LEN; ltr++) {
        _settextwindow(3 + (guess_num * 4), 31 + (ltr * 4), 5 + (guess_num * 4), 33 + (ltr * 4));
        _setbkcolor(test_colors[ltr]);
        _clearscreen(_GWINDOW);
        _settextcolor(15);
        _settextposition(2, 2);
        putch(guess[ltr]);

        snprintf(title, 30, "You have %d guesses remaining!", 5 - guess_num);
        title_bar(title);
    }
}

void play(char *word) {
    _settextwindow(1, 1, 25, 80);
    _setbkcolor(7);
    _clearscreen(_GWINDOW);

    title_bar("WORDOS: Solve for the Secret Word!");

    for (int g = 0; g < 6; g++) {
        char guess[WORD_LEN + 1];
        enter_guess(&guess);

        // _settextposition(5, 13);
        // _settextcolor(3);
        // _outtext(guess);

        print_guess(guess, g);
    }

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
