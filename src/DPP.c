#include <conio.h>
#include <graph.h>
#include "puzzles.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/* Background Colors */
#define SCREEN_BK   1
#define MENU_BK     3

/* Foreground/Text Colors */
#define MENU_FG     0

void column_header() {
    _settextwindow(7, 1, 7, 80);
    _setbkcolor(2);
    _clearscreen(_GWINDOW);
    _settextcolor(15);
    char buf[2];
    for (int i = 0; i < 8; i++) {
        for (int j = 1; j < 11; j++) {
            if (i == 7 && j == 10) {
                break;
            }
            _outtext(itoa(j < 10 ? j : 0, buf, 10));
        }
    }
}

void row_header() {
    _settextwindow(1, 80, 25, 80);
    _setbkcolor(4);
    _clearscreen(_GWINDOW);
    _settextcolor(14);
    char buf[2];
    for (int i = 0; i < 24; i++) {
        _settextposition(i + 1, 1);
        _outtext(itoa(i % 10, buf, 10));
    }
}

int menu(int count, ...) {

    va_list valist;

    va_start(valist, count);

    puzzle_t *temp;
    char **names;
    names = malloc(count * sizeof(char *));

    for (int i = 0; i < count; i++) {
        temp = va_arg(valist, puzzle_t *);
        names[i] = malloc(strlen(temp->name) + 1);
        strcpy(names[i], temp->name);
    }
    va_end(valist);

    // Message Box
    _settextwindow(8, 32, 16, 49);
    _setbkcolor(MENU_BK);
    _clearscreen(_GWINDOW);

    // Static Text
    _settextcolor(MENU_FG);
    _settextposition(1, 2);
    _outtext("CHOOSE AN OPTION");
    _settextposition(9, 8);
    _outtext("QUIT");

    // Games
    for (int i = 0; i < count; i++) {
        _settextposition((2 * i) + 3, ((18 - strlen(names[i])) / 2) + 1);
        _outtext(names[i]);
    }

    getch();

    for (int i = 0; i < count; i++) {
        free(names[i]);
    }
    free(names);

    return 0;
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

    column_header();
    row_header();

    menu(3, &wordos, &nonogram, &mathdoku);

    _displaycursor(_GCURSORON);
    _setvideomode(_DEFAULTMODE);
    _clearscreen(_GCLEARSCREEN);

    // printf("%s: %s\n", wordos.name, puzzle_description(&wordos));
    // printf("%s: %s\n", nonogram.name, puzzle_description(&nonogram));
    // printf("%s: %s\n", mathdoku.name, puzzle_description(&mathdoku));

    // printf("%s\n", init_puzzle(&wordos));
    // printf("%s\n", init_puzzle(&nonogram));
    // printf("%s\n", init_puzzle(&mathdoku));

    return 0;
}
