#ifndef MELODIES_H
#define MELODIES_H

#include <Arduino.h>

// Function to create the default melodies
void createDefaultMelodies();

// Function to save a melody to a .tune file
void saveMelodyToFile(const String& melodyName, const melody::Melody& melody);

// Function to load a melody from a .tune file
bool loadMelodyFromFile(const String& melodyName, melody::Melody& melody);

// Function to list all .tune files on the LittleFS partition
void listTuneFiles();

// Function to play a melody by name
void playMelody(const String& melodyName);

// Function to delete a .tune file by filename
bool deleteTuneFile(const String& fileName);

// Function to create a melody object with the given name
melody::Melody createMelody(const String& melodyName);

// Function to add a tone to a melody
void addToneToMelody(melody::Melody& melody, float freq, int duration);

#endif
