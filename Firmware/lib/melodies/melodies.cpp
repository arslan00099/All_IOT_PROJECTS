#include <LittleFS.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include "melodies.h"
#include "melodies.pb.h"

void createDefaultMelodies() {
  // Create and initialize the Melodies
  Melodies melodies;
  melodies.melody_count = 1;

  // Create the melody
  Melody melody;
  melody.label = "Melody 4";

  Tone tone1;
  tone1.frequency = 440.0;
  tone1.duration = 500;

  Tone tone2;
  tone2.frequency = 660.0;
  tone2.duration = 400;

  Tone tone3;
  tone3.frequency = 880.0;
  tone3.duration = 300;

  Tone tone4;
  tone4.frequency = 1100.0;
  tone4.duration = 200;

  // Add tones to the melody
  melody.tones.funcs.encode = &Tone_encode;
  pb_ostream_t stream = pb_ostream_from_buffer(melody.tones.buffer, sizeof(melody.tones.buffer));
  pb_encode(&stream, Tone_fields, &tone1);
  pb_encode(&stream, Tone_fields, &tone2);
  pb_encode(&stream, Tone_fields, &tone3);
  pb_encode(&stream, Tone_fields, &tone4);

  // Add melody to the melodies
  melodies.melodies[0].funcs.encode = &Melody_encode;
  pb_ostream_t stream2 = pb_ostream_from_buffer(melodies.melodies[0].buffer, sizeof(melodies.melodies[0].buffer));
  pb_encode(&stream2, Melody_fields, &melody);

  // Save the melody to a file on the LittleFS partition
  File file = LittleFS.open("/melody4.dat", "w");
  if (file) {
    file.write(melodies.melodies[0].buffer, stream2.position);
    file.close();
  }
}

void saveMelodyToFile(const String& melodyName, const melody::Melody& melody) {
  char fileName[MAX_FILE_NAME_LENGTH];
  melodyName.toCharArray(fileName, MAX_FILE_NAME_LENGTH);

  // Open the file for writing
  File file = LittleFS.open(fileName, "w");
  if (!file) {
    Serial.println("Error opening file for writing");
    return;
  }

  // Create a melody message and copy the melody data
  melody::MelodyMessage melodyMessage;
  melodyMessage.set_label(melody.label());

  for (const auto& note : melody.notes()) {
    melody::Note* newNote = melodyMessage.add_notes();
    newNote->set_frequency(note.frequency());
    newNote->set_duration(note.duration());
  }

  // Serialize the melody message
  uint8_t buffer[melody::MelodyMessage_size];
  pb_ostream_t ostream = pb_ostream_from_buffer(buffer, sizeof(buffer));
  bool status = pb_encode(&ostream, melody::MelodyMessage_fields, &melodyMessage);
  if (!status) {
    Serial.println("Error encoding melody message");
    file.close();
    return;
  }

  // Write the serialized data to the file
  file.write(buffer, ostream.bytes_written);
  file.close();
}

bool loadMelodyFromFile(const String& melodyName, melody::Melody& melody) {
  char fileName[MAX_FILE_NAME_LENGTH];
  melodyName.toCharArray(fileName, MAX_FILE_NAME_LENGTH);

  // Open the file for reading
  File file = LittleFS.open(fileName, "r");
  if (!file) {
    Serial.println("Error opening file for reading");
    return false;
  }

  // Read the file contents into a buffer
  uint8_t buffer[melody::MelodyMessage_size];
  size_t bytesRead = file.read(buffer, sizeof(buffer));
  file.close();

  if (bytesRead == 0) {
    Serial.println("Error reading file");
    return false;
  }

  // Decode the serialized data into a melody message
  melody::MelodyMessage melodyMessage;
  pb_istream_t istream = pb_istream_from_buffer(buffer, bytesRead);
  bool status = pb_decode(&istream, melody::MelodyMessage_fields, &melodyMessage);
  if (!status) {
    Serial.println("Error decoding melody message");
    return false;
  }

  // Copy the melody data from the message
  melody.set_label(melodyMessage.label());

  for (const auto& note : melodyMessage.notes()) {
    melody::Note* newNote = melody.add_notes();
    newNote->set_frequency(note.frequency());
    newNote->set_duration(note.duration());
  }

  return true;
}

void listMelodies() {
  // Open the LittleFS partition
  if (!LittleFS.begin()) {
    Serial.println("Failed to mount LittleFS");
    return;
  }

  // Open the root directory
  Dir dir = LittleFS.openDir("/");
  while (dir.next()) {
    // Check if the file has a .tune extension
    if (dir.isFile() && String(dir.fileName()).endsWith(".tune")) {
      // Open the file
      File file = dir.openFile("r");

      // Read the file contents into a buffer
      uint8_t buffer[file.size()];
      size_t bytesRead = file.read(buffer, file.size());

      // Create a melody object
      Melody melody;

      // Attempt to decode the melody object from the buffer
      pb_istream_t stream = pb_istream_from_buffer(buffer, bytesRead);
      bool success = pb_decode(&stream, Melody_fields, &melody);

      // Check if decoding was successful
      if (success) {
        Serial.println(dir.fileName());
      }

      // Close the file
      file.close();
    }
  }

  // Close the LittleFS partition
  LittleFS.end();
}
 
void playMelody(const String& name) {
  // Add .tune extension to the melody file name
  String fileName = name + ".tune";

  // Open the melody file
  File file = LittleFS.open(fileName, "r");
  if (!file) {
    Serial.println("Failed to open melody file: " + fileName);
    return;
  }

  // Read the file contents into a buffer
  uint8_t buffer[file.size()];
  size_t bytesRead = file.read(buffer, file.size());

  // Create a melody object
  Melody melody;

  // Attempt to decode the melody object from the buffer
  pb_istream_t stream = pb_istream_from_buffer(buffer, bytesRead);
  bool success = pb_decode(&stream, Melody_fields, &melody);

  // Check if decoding was successful
  if (!success) {
    Serial.println("Invalid melody file: " + fileName);
    file.close();
    return;
  }

  // Loop through each tone and play it
  for (int i = 0; i < melody.tones_count; i++) {
    Tone tone = melody.tones[i];
    Beep(tone.frequency, tone.duration);
  }

  // Close the file
  file.close();
}

bool deleteTuneFile(const String& fileName) {
  // Append the .tune extension to the filename
  String tuneFileName = fileName + ".tune";

  // Check if the file exists
  if (!LittleFS.exists(tuneFileName)) {
    return false; // File does not exist
  }

  // Delete the file
  if (!LittleFS.remove(tuneFileName)) {
    return false; // Failed to delete the file
  }

  return true; // File deleted successfully
}

melody::Melody createMelody(const String& melodyName) {
  melody::Melody newMelody;
  newMelody.set_label("Melody - " + melodyName);
  return newMelody;
}

void addToneToMelody(melody::Melody& melody, float freq, int duration) {
  melody::Note* newNote = melody.add_notes();
  newNote->set_frequency(freq);
  newNote->set_duration(duration);
}
