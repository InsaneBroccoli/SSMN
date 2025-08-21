#include "buzzer.h"
#include <Arduino.h>

#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494

#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988

#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_E6  1319 

// tempo (BPM)
static const int TEMPO = 130;   // 130
static inline int beat_ms() { return 60000 / TEMPO; }



void mario_racing() {
    int melody[] = { NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5 };
    int noteDurations[] = { 4, 4, 4, 1 };

    int notes = sizeof(melody) / sizeof(melody[0]);
    for (int i = 0; i < notes; i++) {
        int duration = 1000 / noteDurations[i];
        tone(BUZZER1, melody[i], duration);
        delay(duration * 3);
        noTone(BUZZER1);
    }
}


void level_clear() {
    int melody[] = { NOTE_C5, NOTE_E5, NOTE_G5, NOTE_C6, NOTE_G5, NOTE_C6 };
    int durations[] = { 8, 8, 8, 8, 8, 2 }; // ends with long C6

    int notes = sizeof(melody) / sizeof(melody[0]);
    for (int i = 0; i < notes; i++) {
        int duration = 1000 / durations[i];
        tone(BUZZER1, melody[i], duration);
        delay(duration * 1.4);
        noTone(BUZZER1);
    }
}

void crazyfrog() {
  int melody[] = {
    // Line 1
    NOTE_E5, NOTE_G5, NOTE_E5, NOTE_E5, NOTE_A5, NOTE_E5, NOTE_D5,
    // Line 2
    NOTE_E5, NOTE_B5, NOTE_E5, NOTE_E5, NOTE_C6, NOTE_B5, NOTE_G5,
    // Line 3
    NOTE_E5, NOTE_B5, NOTE_E6, NOTE_E5, NOTE_D5, NOTE_B4, NOTE_FS5, NOTE_E5
  };

  int durations[] = {
    // Line 1
    4, 7, 7, 14, 10, 10, 10,
    // Line 2
    4, 7, 7, 14, 10, 10, 10,
    // Line 3
    10, 10, 10, 12, 12, 10, 10, 2 
  };

  int n = sizeof(melody) / sizeof(melody[0]);
  int beat = beat_ms();

  for (int i = 0; i < n; i++) {
    int duration = (4 * beat) / durations[i];   // 4 = quarter note
    tone(BUZZER1, melody[i], duration);
    delay(duration * 1.3);   // space between notes
    noTone(BUZZER1);
  }
}
