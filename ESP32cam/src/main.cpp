// Change according to your model
// The models available are
//   - CAMERA_MODEL_WROVER_KIT
//   - CAMERA_MODEL_ESP_EYE
//   - CAMERA_MODEL_M5STACK_PSRAM
//   - CAMERA_MODEL_M5STACK_WIDE
//   - CAMERA_MODEL_AI_THINKER
#include <Arduino.h>

#define CAMERA_MODEL_AI_THINKER
#define Flash 4

#include <FS.h>
#include <SPIFFS.h>
#include "EloquentVision.h"
boolean flashFlag=false;
unsigned long preFlashMillis = 0;

#define FRAME_SIZE FRAMESIZE_QVGA
#define SOURCE_WIDTH 320
#define SOURCE_HEIGHT 240
#define CHANNELS 1
#define DEST_WIDTH 32
#define DEST_HEIGHT 24
#define BLOCK_VARIATION_THRESHOLD 0.3
#define MOTION_THRESHOLD 0.2

using namespace Eloquent::Vision;
using namespace Eloquent::Vision::IO;
using namespace Eloquent::Vision::ImageProcessing;
using namespace Eloquent::Vision::ImageProcessing::Downscale;
using namespace Eloquent::Vision::ImageProcessing::DownscaleStrategies;

// an easy interface to capture images from the camera
ESP32Camera camera;
// the buffer to store the downscaled version of the image
uint8_t resized[DEST_HEIGHT][DEST_WIDTH];

Cross<SOURCE_WIDTH, SOURCE_HEIGHT, DEST_WIDTH, DEST_HEIGHT> crossStrategy;

Downscaler<SOURCE_WIDTH, SOURCE_HEIGHT, CHANNELS, DEST_WIDTH, DEST_HEIGHT> downscaler(&crossStrategy);
MotionDetection<DEST_WIDTH, DEST_HEIGHT> motion;
JpegWriter<SOURCE_WIDTH, SOURCE_HEIGHT> jpegWriter;

bool debounceMotion(bool touch = false);
void printFilesize(const char *filename);


void setup()
{
  Serial.begin(115200);
  pinMode(Flash,OUTPUT);
  SPIFFS.begin(true);
  camera.begin(FRAME_SIZE, PIXFORMAT_GRAYSCALE);
  motion.setBlockVariationThreshold(BLOCK_VARIATION_THRESHOLD);
}

void loop()
{
if(flashFlag){
  if(millis()-preFlashMillis > 1000){
    digitalWrite(Flash,LOW);
    flashFlag=false;
  }
}

  uint32_t start = millis();
  Serial.println("TAKING PHOTO ...");
  camera_fb_t *frame = camera.capture();

 
  delay(500);
  Serial.println("RESIZING PHOTO ...");
  downscaler.downscale(frame->buf, resized);
  motion.update(resized);
  motion.detect();

 
  Serial.print(1000.0f / (millis() - start));
  Serial.println(" fps");


  if (motion.ratio() > MOTION_THRESHOLD)
  {
    Serial.println("Motion detected");
      flashFlag =true;
      preFlashMillis=0;
      digitalWrite(Flash, HIGH);

      if (debounceMotion())
      {
    // take a new pic in the hope it is less affected by the motion noise
    // (you may comment this out if you want)

    unsigned long preMillis = 0;
    if (millis() - preMillis > 500)
    {
      frame = camera.capture();

      // write as jpeg
      File imageFile = SPIFFS.open("/capture.jpg", "wb");
      // you can tweak this value as per your needs
      uint8_t quality = 30;

      Serial.println("The image will be saved as /capture.jpg");
      jpegWriter.writeGrayscale(imageFile, frame->buf, quality);
      imageFile.close();
      printFilesize("/capture.jpg");

      debounceMotion(true);
    }
      }
  }
}

/**
 * Debounce repeated motion detections
 * @return
 */
bool debounceMotion(bool touch)
{
  static uint32_t lastMotion = 0;

  // update last tick
  if (lastMotion == 0 || touch)
    lastMotion = millis();

  // debounce
  if (millis() - lastMotion > 5000)
  {
    lastMotion = millis();

    return true;
  }

  return false;
}

/**
 * Print file size (for debug)
 * @param filename
 */
void printFilesize(const char *filename)
{
  File file = SPIFFS.open(filename, "r");

  Serial.print(filename);
  Serial.print(" size is ");
  Serial.print(file.size() / 1000);
  Serial.println(" kb");

  file.close();
}