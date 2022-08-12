#include <conio.h>
#include <graph.h>

/*
    Color Table, 0-7 is FG + BG, 8-15 is FG only
    iRGB    DEC     COLOR
    ---------------------
    0000    0       BLACK
    0001    1       BLUE
    0010    2       GREEN
    0011    3       CYAN
    0100    4       RED
    0101    5       MAGENTA
    0110    6       "BROWN" (DARK YELLOW)
    0111    7       "WHITE" (LIGHT GRAY)
    1000    8       BRIGHT BLACK (GRAY)
    1001    9       BRIGHT BLUE
    1010    10      BRIGHT GREEN
    1011    11      BRIGHT CYAN
    1100    12      BRIGHT RED
    1101    13      BRIGHT MAGENTA
    1110    14      BRIGHT YELLOW
    1111    15      BRIGHT WHITE

    Add 16 to fg color to make it blink
*/
void draw_message(int y1, int x1, int y2, int x2, int fg, int bg, char *msg) {

    // Message Box
    _settextwindow(y1, x1, y2, x2);
    _setbkcolor(bg);
    _clearscreen(_GWINDOW);

    // Message
    _settextcolor(fg);
    _settextposition(1, 4);
    _outtext(msg);
}

int main() {

    _setvideomode(_TEXTC80);
    _displaycursor(_GCURSOROFF);

    _settextwindow(1, 1, 25, 80);
    _setbkcolor(0);
    _clearscreen(_GCLEARSCREEN);

    for (int bg = 0; bg < 8; bg++) {
        for (int fg = 0; fg < 16; fg++) {
            draw_message(5 + fg, (bg * 10) + 1, 5 + fg, (bg * 10) + 10, fg, bg, "TEST");
        }
        getch();
    }

    _displaycursor(_GCURSORON);
    _setvideomode(_DEFAULTMODE);
    _clearscreen(_GCLEARSCREEN);

    return 0;
}
