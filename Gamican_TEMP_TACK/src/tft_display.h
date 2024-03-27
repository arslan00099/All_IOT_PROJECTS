#ifndef TFT_DISPLAY_H
#define TFT_DISPLAY_H

void displayInit(void);
void tft_display_txt(char* txt, uint16_t x_pos, uint16_t y_pos, uint16_t txt_color, uint16_t txt_backgroundcolor, uint8_t txt_size);
void tft_display_txt(char* txt, uint16_t x_pos, uint16_t y_pos, uint8_t txt_size);
void drawFilledCircle(uint16_t x_pos, uint16_t y_pos, uint16_t raduis, uint16_t color);

#endif
