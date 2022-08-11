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
* [PDCurses](https://github.com/wmcbrine/PDCurses)

## Install OpenWatcomC

### For DOS environments EXCLUDING FREEDOS
If using DosBox or an actual DOS environment (such as MSDOS or Windows 9X, **but excluding FreeDOS**) you can get the DOS installer from the github link and follow their instructions. Make everything is set properly in `AUTOEXEC.BAT`.

### For FreeDOS
If you are not able to use `FDIMPLES` or their bonus / supplemental CD then follow the instructions above for standard DOS environments. Otherwise launch `FDIMPLES` and make sure it is installed from the `DEVEL` package. You will see a script in `C:\DEVEL\WATCOM` called `OWSETENV.BAT` (or something of the sort). Running that will set all necessary environment variables so you can begin. I recommend setting this script to run on boot, or create an alias in `FDAUTO.BAT` in order to more easily run it.

## Install PDCURSES
Follow the instructions in the github link - you will want to build it (obviously) for the DOS environment. The make command will be something like `wmake -f makefile.wcc` from within the DOS environment directory of PDCURSES.

Next I recommend setting an environment variable linking to the PDCURSES directory. The makefiles use `PDCURSES_SRCDIR` so I personally chose to use that name. I made an entry in `AUTOEXEC.BAT` (for MSDOS) or `FDAUTO.BAT` (for FreeDOS). For example, if you installed to `C:\PDCURSES` then your command would be `SET PDCURSES_SRCDIR=C:\PDCURSES`.

## Compile with OpenWatcomC
I'll get a makefile eventually.

I have the environment variable `PDCURSES_SRCDIR` set to where I installed that library. If you used a different name for your variable then do `%YOUR_ENV_VAR_NAME_HERE%`. If you didn't set an environment variable just replace that with your install location. For example if you installed to `C:\PDCURSES` you would use `-i=C:\PDCURSES`. 

`wcl -za99 -wx -za -i=%PDCURSES_SRCDIR% *.c`

This will create `DPP.EXE`. Note: I recommend actually listing the necessary C source files rather than just doing `*.c` however going the wildcard route does technically work.