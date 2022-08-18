#include "puzzles.h"
#include <time.h>
#include <graph.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>

#define DEBUG       0

#define WORD_LEN    5
#define GUESSES     6

#define N_MATCH     0
#define P_MATCH     6
#define F_MATCH     2

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
            title_bar(title);
        }
    }
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

            /*
                Handle duplicate yellow letters
                Example word: SOGGY
                'G' guessed in position 3 or 4 obviously ought to be green
                'G' in any other position ought to be yellow
                Once there have been 2 'G's any other that should be yellow ought to be black

                Examples:
                    GROSS = YBYYB
                    FLOGS = BBYGY
                    GROGS = YBYGY
                    GROGG = YBYGB
                    FLOSS = BBYYB
            */

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
    title_bar(title);

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
    for (g = 0; g < GUESSES; g++) {
        print_keyboard(&kbd);
        enter_guess(&guess);

        ltr_freq_t temp[WORD_LEN];
        init_freqs(&temp);
        memcpy(&temp, &freqs, sizeof freqs);

        compare_guess(&cmp, &guess, word, &temp, &kbd);
        print_guess(&cmp, guess, g);

        if (check_results(&cmp)) {
            break;
        }
    }
    print_keyboard(&kbd);

    if (g < GUESSES - 1) {
        snprintf(title, 35, "You found the word in %d guess%s", g + 1, !g ? "!" : "es!");
    } else {
        snprintf(title, 35, "FAILURE! The word was %s!", word);
    }
    title_bar(title);

    getch();
}

static const char *init(void) {
    srand(time(NULL));

    FILE *ans;
    ans = fopen("answers.txt", "r");
    if (ans != NULL) {
        int line = rand() % count_lines(ans);
        // char *word = get_word(ans, line);
        char *word = "soggy";
        _strupr(word);
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
