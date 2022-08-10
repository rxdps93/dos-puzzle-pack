# dos-puzzle-pack
Various Puzzle Games for DOS

You must have openwatcomc and pdcurses in order to build it

Will start with

* WORDOS
    * Wordle clone
* NONOGRAM
    * Self explanatory...many know this as PICROSS these days
* MATHDOKU
    * Better known as KenKen

OpenWatcomC 2.0 Compiler: (I'll get a makefile eventually)

`wcl -za99 -wx -za -i=<PDCURSES_SRCDIR> dpp*.c`