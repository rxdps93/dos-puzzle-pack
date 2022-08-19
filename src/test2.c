#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define WORD_LEN    5

char *struppr(char *s)
{
    char *tmp = s;

    for (; *tmp; ++tmp) {
        *tmp = toupper((unsigned char) *tmp);
    }

    return s;
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

int cmp_func(const void *p1, const void *p2) {
    const char *p1c = (const char *)p1;
    const char **p2c = (const char **)p2;
    printf("%s <-> %s\n", p1c, *p2c);
    return strcmp(p1c, *p2c);
}

int search(char *guess, char **valid_guesses, int ng) {
    char **key;
    key = (char **)bsearch(guess, valid_guesses, ng, sizeof(char *), cmp_func);
    if (key == NULL) {
        return -1;
    }
    return (key - valid_guesses);
}

int main() {

    char *guess = "WELDS";
    char file[18];
    snprintf(file, 18, "dicts/guess_%c.txt", tolower(guess[0]));

    FILE *gfp;
    gfp = fopen(file, "r");
    int lc = count_lines(gfp);
    char **guesses = malloc(lc * sizeof(char *));

    for (int i = 0; i <= lc; i++) {
        guesses[i] = malloc(WORD_LEN + 1);
        fseek(gfp, i * 6, SEEK_SET);
        fgets(guesses[i], 6, gfp);
        struppr(guesses[i]);
        // printf("%s\n", guesses[i]);
    }
    fclose(gfp);

    printf("%d\n", search(guess, guesses, lc));

    for (int i = 0; i < lc; i++) {
        free(guesses[i]);
    }
    free(guesses);

    return 0;
}
