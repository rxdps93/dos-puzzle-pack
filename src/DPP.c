#include <dos.h>
#include <conio.h>
#include <graph.h>
#include "puzzles.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define GAMES       3

/* Background Colors */
#define SCREEN_BK   1
#define MENU_BK     3
#define SHADOW      0

/* Foreground/Text Colors */
#define MENU_FG     0
#define MENU_HL     31
#define TITLE       15

void column_header() {
    _settextwindow(7, 1, 7, 80);
    _setbkcolor(2);
    _clearscreen(_GWINDOW);
    _settextcolor(15);
    for (int i = 0; i < 8; i++) {
        for (int j = 1; j < 11; j++) {
            if (i == 7 && j == 10) {
                break;
            }
            putch(j < 10 ? j : 0);
        }
    }
}

void row_header() {
    _settextwindow(1, 31, 25, 31);
    _setbkcolor(4);
    _clearscreen(_GWINDOW);
    _settextcolor(14);
    for (int i = 0; i < 24; i++) {
        _settextposition(i + 1, 1);
        putch(i % 10);
    }
}

void show_title() {
    _settextcolor(TITLE);
    _settextposition(2, 5);
    _outtext(" ____   ___  ____    ____                _        ____            _   ");
    _settextposition(3, 5);
    _outtext("|  _ \\ / _ \\/ ___|  |  _ \\ _   _ _______| | ___  |  _ \\ __ _  ___| | __");
    _settextposition(4, 5);
    _outtext("| | | | | | \\___ \\  | |_) | | | |_  /_  / |/ _ \\ | |_) / _` |/ __| |/ /");
    _settextposition(5, 5);
    _outtext("| |_| | |_| |___) | |  __/| |_| |/ / / /| |  __/ |  __/ (_| | (__|   < ");
    _settextposition(6, 5);
    _outtext("|____/ \\___/|____/  |_|    \\__,_/___/___|_|\\___| |_|   \\__,_|\\___|_|\\_\\");
}

void play_note(unsigned short freq, unsigned int duration) {
    sound(freq);
    delay(duration);
    nosound();
}

void highlight_option(int cur, int prev, char **opts) {
    _settextposition((2 * cur) + 3, ((18 - strlen(opts[cur])) / 2) + 1);
    _settextcolor(MENU_HL);
    _outtext(opts[cur]);

    if (prev != -1) {
        _settextposition((2 * prev) + 3, ((18 - strlen(opts[prev])) / 2) + 1);
        _settextcolor(MENU_FG);
        _outtext(opts[prev]);

        play_note(440, 100);
    }
}

int menu(int count, ...) {

    va_list valist;

    va_start(valist, count);

    puzzle_t *temp;
    char **names;
    names = malloc((count + 1) * sizeof(char *));

    for (int i = 0; i < count; i++) {
        temp = va_arg(valist, puzzle_t *);
        names[i] = malloc(strlen(temp->name) + 1);
        strcpy(names[i], temp->name);
    }
    va_end(valist);
    names[count] = "QUIT";

    // Title
    show_title();

    // Drop Shadow
    _settextwindow(12, 33, 20, 50);
    _setbkcolor(SHADOW);
    _clearscreen(_GWINDOW);

    // Message Box
    _settextwindow(11, 32, 19, 49);
    _setbkcolor(MENU_BK);
    _clearscreen(_GWINDOW);

    // Header Text
    _settextcolor(MENU_FG);
    _settextposition(1, 2);
    _outtext("CHOOSE AN OPTION");

    // Options
    for (int i = 0; i < count + 1; i++) {
        _settextposition((2 * i) + 3, ((18 - strlen(names[i])) / 2) + 1);
        _outtext(names[i]);
    }

    int c1 = 0;
    int highlighted = 0;
    highlight_option(highlighted, -1, names);
    while (c1 != 13 && c1 != 27) {

        if (kbhit()) {
            c1 = getch();

            if ((c1 == 72) && (highlighted > 0)) {
                highlight_option(highlighted - 1, highlighted, names);
                highlighted--;
            } else if ((c1 == 80) && (highlighted < count)) {
                highlight_option(highlighted + 1, highlighted, names);
                highlighted++;
            }

            if (c1 == 27) {
                highlighted = count;
            }
        }
    }
    
    play_note(highlighted == count ? 220 : 880, 100);

    for (int i = 0; i < count; i++) {
        free(names[i]);
    }
    free(names);

    return highlighted;
}

int main(void) {

    _setvideomode(_TEXTC80);
    _displaycursor(_GCURSOROFF);

    _settextwindow(1, 1, 25, 80);
    _setbkcolor(SCREEN_BK);
    _clearscreen(_GCLEARSCREEN);

    puzzle_t wordos  = { WORDOS, "WORDOS" };
    puzzle_t nonogram = { NONOGRAM, "NONOGRAM" };
    puzzle_t mathdoku = { MATHDOKU, "MATHDOKU" };

    puzzle_t **puzzles = malloc(GAMES * sizeof(puzzle_t *));
    puzzles[0] = &wordos;
    puzzles[1] = &nonogram;
    puzzles[2] = &mathdoku;

    // column_header();
    // row_header();

    int option = menu(GAMES, &wordos, &nonogram, &mathdoku);

    if (option < GAMES) {
        printf("%s\n", init_puzzle(puzzles[option]));
        // init_puzzle(puzzles[option]);
    }

    _displaycursor(_GCURSORON);
    _setvideomode(_DEFAULTMODE);
    _clearscreen(_GCLEARSCREEN);

    free(puzzles);
    return 0;
}
