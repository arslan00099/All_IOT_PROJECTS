#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "tft_display.h"
#include "icons.h"

#define TFT_RST   4
#define TFT_CS    15
#define TFT_DC    2
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void displayInit(void) {
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_BLACK);
  tft.drawRGBBitmap(0, 0, isloAI, 128, 45);
  tft.drawBitmap(100,90, cloudConnection, 15, 15, ST7735_WHITE);
  tft.fillCircle(123, 95, 4, ST7735_RED);
  tft.drawFastHLine(0, 50,  tft.width(), ST7735_WHITE);
  tft.setCursor(0, 54);
  tft.setTextColor(ST7735_YELLOW, ST7735_BLACK);
  tft.print("------");
  tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
  tft.setCursor(94, 54);
  tft.print("WiFi");
  tft.fillCircle(123, 57, 4, ST7735_RED);
  tft.setTextColor(ST7735_YELLOW, ST7735_BLACK);
  tft.setTextSize(3);
  tft.setCursor(0, 120);
  tft.print("----");
}

void tft_display_txt(char* txt, uint16_t x_pos, uint16_t y_pos, uint16_t txt_color, uint16_t txt_backgroundcolor, uint8_t txt_size){
  tft.setTextSize(txt_size);
  tft.setTextColor(txt_color, txt_backgroundcolor);
  tft.setCursor(x_pos, y_pos);
  tft.print(txt);
}

void tft_display_txt(char* txt, uint16_t x_pos, uint16_t y_pos, uint8_t txt_size){
  tft.setTextSize(txt_size);
  tft.setCursor(x_pos, y_pos);
  tft.print(txt);
}

void drawFilledCircle(uint16_t x_pos, uint16_t y_pos, uint16_t radius, uint16_t color){
  tft.fillCircle(x_pos, y_pos, radius, color);
}
