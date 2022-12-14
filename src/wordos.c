#include "puzzles.h"
#include <time.h>
#include <graph.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>

#define DEBUG       0

#define WORD_LEN    5
#define GUESSES     6

#define N_MATCH     0
#define P_MATCH     6
#define F_MATCH     2

#define TITLE_BAD   4
#define TITLE_GOOD  2
#define TITLE_NTRL  3

const char keys[26] = {
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M' 
};

typedef struct kbd {
    char keys[26];
    int status[26];
} kbd_t;

typedef struct ltr_freq {
    char letter;
    int freq;
} ltr_freq_t;

void init_kbd(kbd_t *kbd) {
    for (int k = 0; k < 26; k++) {
        kbd->keys[k] = keys[k];
        kbd->status[k] = 1;
    }
}

void set_kbd(kbd_t *kbd, char c, int status) {
    for (int k = 0; k < 26; k++) {
        if (kbd->keys[k] == c) {
            if (kbd->status[k] != F_MATCH) {
                kbd->status[k] = status;
            }
        }
    }
}

void init_freqs(ltr_freq_t *freqs) {
    for (int i = 0; i < WORD_LEN; i++) {
        freqs[i].letter = '\0';
        freqs[i].freq = 0;
    }
}

int freq_contains(ltr_freq_t *freqs, char letter) {
    for (int i = 0; i < WORD_LEN; i++) {
        if (freqs[i].letter == letter) {
            return i;
        }
    }

    return -1;
}

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
    word[WORD_LEN] = '\0';
    return word;
}

void log_message(char *msg, int count) {
    if (DEBUG) {
        if (count == 0) {
            _settextwindow(9, 2, 23, 25);
            _setbkcolor(4);
            _clearscreen(_GWINDOW);
            _settextcolor(15);
            _settextposition(2, 5);
            _outtext("Debug Messaging:");
        }

        _settextposition(4 + count, 2);
        _outtext(msg);
    }
}

void title_bar(char *text, int type) {
    _settextwindow(1, 1, 1, 80);
    _setbkcolor(type);
    _clearscreen(_GWINDOW);
    _settextcolor(15);
    _settextposition(1, (80 - strlen(text)) / 2);
    _outtext(text);
}

int check_guess(char *guess) {

    char file[18];
    snprintf(file, 18, "dicts/guess_%c.txt", tolower(guess[0]));

    FILE *gfp;
    gfp = fopen(file, "r");
    int lc = count_lines(gfp);
    char **guesses = malloc(lc * sizeof(char *));

    int valid = 0;
    for (int i = 0; i <= lc; i++) {
        guesses[i] = get_word(gfp, i);
        _strupr(guesses[i]);

        if (strcmp(guess, guesses[i]) == 0) {
            valid = 1;
        }

        free(guesses[i]);

        if (valid) {
            break;
        }
    }
    fclose(gfp);
    free(guesses);

    if (!valid) {
        struct rccoord old = _gettextposition();
        _settextposition(6, 4);
        _outtext("Not a valid guess!");
        _settextposition(old.row, old.col);
        return 1;
    } else {
        return 0;
    }
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
    int chk_txt = 0;
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

            if (chk_txt) {
                struct rccoord old = _gettextposition();
                _settextposition(6, 4);
                _outtext("                  ");
                _settextposition(old.row, old.col);
                chk_txt = 0;
            }
        }

        if ((c == 13) && (letter == WORD_LEN)) {
            guess[WORD_LEN] = '\0';
            loop = check_guess(guess);
            chk_txt = loop;
        }
        
    } while (loop);

    guess[WORD_LEN] = '\0';
}

void print_keyboard(kbd_t *kbd) {

    int offset_y = 0;
    int offset_x;
    for (int k = 0; k < 26; k++) {
        offset_x = (k % 6) * 4;
        _settextwindow(3 + (offset_y * 4), 55 + offset_x, 5 + (offset_y * 4), 57 + offset_x);
        _setbkcolor(kbd->status[k]);
        _clearscreen(_GWINDOW);
        _settextcolor(15);
        _settextposition(2, 2);
        putch(kbd->keys[k]);

        if ((57 + (offset_x + 4)) > 79) {
            offset_y++;
        }
    }
}

void print_guess(int *test_colors, char *guess, int guess_num) {

    char title[30];
    for (int ltr = 0; ltr < WORD_LEN; ltr++) {
        _settextwindow(3 + (guess_num * 4), 31 + (ltr * 4), 5 + (guess_num * 4), 33 + (ltr * 4));
        _setbkcolor(test_colors[ltr]);
        _clearscreen(_GWINDOW);
        _settextcolor(15);
        _settextposition(2, 2);
        putch(guess[ltr]);

        if (!DEBUG) {
            snprintf(title, 30, "You have %d guesses remaining!", 5 - guess_num);
            title_bar(title, TITLE_NTRL);
        }
    }
}

void compare_guess(int *result, char *guess, char *word, ltr_freq_t *freqs, kbd_t *kbd) {

    int lc;
    int debug = 0;
    char msg[25];
    int contains = 0;
    for (int gl = 0; gl < WORD_LEN; gl++) {

        snprintf(msg, 25, "compare %c with %c at %d", guess[gl], word[gl], gl);
        log_message(msg, debug++);
        /* Check for green */
        if (guess[gl] == word[gl]) {
            result[gl] = F_MATCH;

            contains = freq_contains(freqs, guess[gl]);
            freqs[contains].freq--;

            /* Fix erroneous yellows e.g. SOGGY -> YYYYY = YBBBG instead of BBBBG */
            if (freqs[contains].freq < 0) {
                for (int rw = gl - 1; gl >= 0; rw--) {
                    if ((guess[rw] == word[gl]) && (result[rw] == P_MATCH)) {
                        result[rw] = N_MATCH;
                        break;
                    }
                }
            }

            set_kbd(kbd, guess[gl], F_MATCH);

            snprintf(msg, 25, ">found a match!");
            log_message(msg, debug++);
        } else {
            /* Check for yellow */
            lc = 0;
            for (int wl = 0; wl < WORD_LEN; wl++) {
                if (guess[gl] == word[wl]) {
                    contains = freq_contains(freqs, guess[gl]);

                    if (freqs[contains].freq > 0) {
                        freqs[contains].freq--;
                        result[gl] = P_MATCH;
                        set_kbd(kbd, guess[gl], P_MATCH);
                        lc++;
                        snprintf(msg, 25, ">partial match lc=%d", lc);
                        log_message(msg, debug++);
                        break;
                    }  else {
                        result[gl] = N_MATCH;
                        set_kbd(kbd, guess[gl], N_MATCH);
                        snprintf(msg, 25, ">freq limit hit");
                        log_message(msg, debug++);
                        break;
                    }
                }
            }

            /* Mark as black */
            if (lc == 0) {
                snprintf(msg, 25, ">no bueno");
                log_message(msg, debug++);
                result[gl] = N_MATCH;
                set_kbd(kbd, guess[gl], N_MATCH);
            }
        }
    }
}

int check_results(int *results) {
    int matches = 0;
    for (int i = 0; i < WORD_LEN; i++) {
        matches += (results[i] == F_MATCH);
    }

    return (matches == WORD_LEN);
}

void play(char *word) {
    _settextwindow(1, 1, 25, 80);
    _setbkcolor(7);
    _clearscreen(_GWINDOW);

    char title[35];
    if (DEBUG) {
        snprintf(title, 29, "The new secret word is %s!", word);
    } else {
        snprintf(title, 35, "WORDOS: Solve for the Secret Word!");
    }
    title_bar(title, TITLE_NTRL);

    ltr_freq_t freqs[WORD_LEN];
    init_freqs(&freqs);
    int f_count = 0;
    int contains;
    for (int l = 0; l < WORD_LEN; l++) {
        contains = freq_contains(&freqs, word[l]);
        if (contains == -1) {
            freqs[f_count].letter = word[l];
            freqs[f_count].freq++;
            f_count++;
        } else {
            freqs[contains].freq++;
        }
    }

    char guess[WORD_LEN + 1];
    int cmp[WORD_LEN];
    kbd_t kbd;
    init_kbd(&kbd);
    int g;
    int result = 0;
    for (g = 0; g < GUESSES; g++) {
        print_keyboard(&kbd);
        enter_guess(&guess);

        ltr_freq_t temp[WORD_LEN];
        init_freqs(&temp);
        memcpy(&temp, &freqs, sizeof freqs);

        compare_guess(&cmp, &guess, word, &temp, &kbd);
        print_guess(&cmp, guess, g);

        result = check_results(&cmp);
        if (result) {
            break;
        }
    }
    print_keyboard(&kbd);

    if (result) {
        snprintf(title, 35, "You found the word in %d guess%s", g + 1, !g ? "!" : "es!");
        title_bar(title, TITLE_GOOD);
    } else {
        snprintf(title, 35, "FAILURE! The word was %s!", word);
        title_bar(title, TITLE_BAD);
    }

    getch();
}

int menu() {
    /* Shadow */
    _settextwindow(11, 21, 21, 61);
    _setbkcolor(0);
    _clearscreen(_GWINDOW);

    /* Menu */
    _settextwindow(10, 20, 20, 60);
    _setbkcolor(1);
    _clearscreen(_GWINDOW);

    /* Title */
    _settextcolor(15);
    _settextposition(1, 1);
    _outtext("Would you like to play again?");

    /* Options */
    _settextposition(3, 1);
    _outtext("[Y]es  [R]eturn to Menu  [Q]uit to DOS");

    /* Read input */
    int c1 = 0;
    while (1) {

        if (kbhit()) {
            c1 = getch();

            if ((c1 == 89) || (c1 == 121)) { // Y or y
                return 1;
            }

            if ((c1 == 82) || (c1 == 114)) { // R or r
                return -1;
            }

            if ((c1 == 81) || (c1 == 113)) { // Q or q
                return 0;
            }
        }
    }
}

static const int init(void) {
    srand(time(NULL));

    FILE *ans;
    ans = fopen("dicts/answers.txt", "r");
    int cont;
    if (ans != NULL) {
        int line = rand() % count_lines(ans);

        do {
            char *word = get_word(ans, line);
            // char *word = "soggy";
            _strupr(word);

            play(word);
            free(word);

            cont = menu();

        } while (cont == 1);
        
        fclose(ans);
    }

    return cont;
}

static const char *description(void) {
    return "Solve for the secret 5-letter word in 6 guesses or less with each guess narrowing down the possibilities!";
}

const struct puzzle_vtable WORDOS[] = { { init, description } };
