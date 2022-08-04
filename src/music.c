#include <i86.h>
#include "music.h"

void play_note(note_t note, int bpm) {
    int length = (float)( 60 / (float)bpm) * note.length;
    if (note.pitch != -1) {
        sound(FACTOR / (note.pitch / (1 << note.octave)));
        delay(length);
        nosound();
    } else {
        delay(length);
    }
}
