# dos-puzzle-pack
Various Puzzle Games for DOS

Will start with

* WORDOS
    * Wordle clone
* NONOGRAM
    * Self explanatory...many know this as PICROSS these days
* MATHDOKU
    * Better known as KenKen

Needed to build:
* [OpenWatcomC 2.0](https://github.com/open-watcom/open-watcom-v2)

## Install OpenWatcomC

### For DOS environments EXCLUDING FREEDOS
If using DosBox or an actual DOS environment (such as MSDOS or Windows 9X, **but excluding FreeDOS**) you can get the DOS installer from the github link and follow their instructions. Make everything is set properly in `AUTOEXEC.BAT`.

### For FreeDOS
If you are not able to use `FDIMPLES` or their bonus / supplemental CD then follow the instructions above for standard DOS environments. Otherwise launch `FDIMPLES` and make sure it is installed from the `DEVEL` package. You will see a script in `C:\DEVEL\WATCOM` called `OWSETENV.BAT` (or something of the sort). Running that will set all necessary environment variables so you can begin. I recommend setting this script to run on boot, or create an alias in `FDAUTO.BAT` in order to more easily run it.

## Compile with OpenWatcomC
I'll get a makefile eventually.

`wcl -za99 -wx -za *.c`

This will create `DPP.EXE`. Note: I recommend actually listing the necessary C source files rather than just doing `*.c` however going the wildcard route does technically work.

Also worth noting is that the `-za99` flag enables the currently experimental and unfinished `C99` support within the compiler. It does enable many of C99's features however it does lack many as well - most notably variable length arrays. I'm using it for now because it is what I am used to however at some point I may just switch to full C89/C90.