#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setupOled()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    { // Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }
}

void welcome(String msg)
{
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.drawRect(5, 5, 123, 55, SSD1306_WHITE);
    display.setCursor(5, 25);
    display.println(msg);
    display.display();
}

void printrfidTag(String id, String type, String name)
{
    display.clearDisplay();
    if (id.length() > 10)
    {
        display.setTextSize(1);
        display.setCursor(35, 5);
        display.println(id);
    }
    else
    {
        display.setTextSize(2);
        display.setCursor(5, 2);
        display.println(id);
    }
    // display.setTextSize(2);
    display.setTextColor(WHITE);
    display.drawRect(0, 0, 127, 63, SSD1306_WHITE);
    // display.setCursor(5, 2);
    // display.println(id);
    // Serial.println((String)id.length());
    display.setTextSize(2);
    display.setCursor(10, 20);
    display.println(type);
    display.setCursor(10, 38);
    display.println(name);
    display.display();
}

void main_MENU(int a)
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.drawRect(0, 0, 128, 15, SSD1306_WHITE);
    display.setCursor(3, 4);
    display.println("REASON FOR TRANSA...");

    display.setTextSize(1);
    display.setCursor(5, 19);
    display.println("1. 464");

    display.setCursor(5, 30);
    display.println("2. CRUSHER");

    display.setCursor(5, 40);
    display.println("3. LAND NOMER 456");

    display.setCursor(5, 50);
    display.println("4. STOCK TRANSFER");
    if (a == 1)
    {
        display.drawRect(0, 16, 128, 13, SSD1306_WHITE);
    }
    else if (a == 2)
    {
        display.drawRect(0, 27, 128, 13, SSD1306_WHITE);
    }
    else if (a == 3)
    {
        display.drawRect(0, 37, 128, 13, SSD1306_WHITE);
    }
    else if (a == 4)
    {
        display.drawRect(0, 47, 128, 13, SSD1306_WHITE);
    }
    display.display();
}

void main_kelometer(String value, String result)
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.drawRect(0, 0, 128, 15, SSD1306_WHITE);
    display.setCursor(3, 4);
    display.println(" Kilometers / Hrs");

    display.setTextSize(2);
    display.setCursor(5, 19);
    display.println(value);
    display.setTextSize(1);
    display.setCursor(5, 45);
    display.println(result);

    display.drawRect(0, 16, 128, 20, SSD1306_WHITE);
    display.display();
}

// #############################################################333

void menuVegModeNight(int moist, String port)
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.drawRect(0, 0, 78, 15, SSD1306_WHITE);
    display.setCursor(16, 4);
    display.println("MOISTURE");
    display.drawRect(77, 0, 51, 15, SSD1306_WHITE);
    display.setCursor(94, 4);
    display.println("PORT");
    display.drawRect(0, 14, 78, 25, SSD1306_WHITE);
    display.setCursor(22, 19);
    display.setTextSize(2);
    display.println(moist + (String) " %");
    display.drawRect(77, 14, 51, 25, SSD1306_WHITE);
    display.setCursor(92, 19);
    display.setTextSize(2);
    display.println(port);
    display.drawRect(0, 38, 128, 26, SSD1306_WHITE);
    display.setCursor(17, 45);
    display.setTextSize(2);
    display.println("VEG MODE");
    display.display();
}

void menuBloomModeS1Night(int moist, String port)
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.drawRect(0, 0, 78, 15, SSD1306_WHITE);
    display.setCursor(16, 4);
    display.println("MOISTURE");
    display.drawRect(77, 0, 51, 15, SSD1306_WHITE);
    display.setCursor(94, 4);
    display.println("PORT");
    display.drawRect(0, 14, 78, 25, SSD1306_WHITE);
    display.setCursor(22, 19);
    display.setTextSize(2);
    display.println(moist + (String) " %");
    display.drawRect(77, 14, 51, 25, SSD1306_WHITE);
    display.setCursor(92, 19);
    display.setTextSize(2);
    display.println(port);
    display.drawRect(0, 38, 128, 26, SSD1306_WHITE);
    display.setCursor(35, 41);
    display.setTextSize(1);
    display.println("BLOOM MODE");
    display.setCursor(43, 53);
    display.println("STAGE 1");
    display.display();
}

void menuBloomModeS2Night(int moist, String port)
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.drawRect(0, 0, 78, 15, SSD1306_WHITE);
    display.setCursor(16, 4);
    display.println("MOISTURE");
    display.drawRect(77, 0, 51, 15, SSD1306_WHITE);
    display.setCursor(94, 4);
    display.println("PORT");
    display.drawRect(0, 14, 78, 25, SSD1306_WHITE);
    display.setCursor(22, 19);
    display.setTextSize(2);
    display.println(moist + (String) " %");
    display.drawRect(77, 14, 51, 25, SSD1306_WHITE);
    display.setCursor(92, 19);
    display.setTextSize(2);
    display.println(port);
    display.drawRect(0, 38, 128, 26, SSD1306_WHITE);
    display.setCursor(35, 41);
    display.setTextSize(1);
    display.println("BLOOM MODE");
    display.setCursor(43, 53);
    display.println("STAGE 2");
    display.display();
}

void menuVegModeDay(int moist, String port)
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.drawRect(0, 0, 78, 15, SSD1306_WHITE);
    display.setCursor(16, 4);
    display.println("MOISTURE");
    display.drawRect(77, 0, 51, 15, SSD1306_WHITE);
    display.setCursor(94, 4);
    display.println("PORT");
    display.drawRect(0, 14, 78, 25, SSD1306_WHITE);
    display.setCursor(22, 19);
    display.setTextSize(2);
    display.println(moist + (String) " %");
    display.drawRect(77, 14, 51, 25, SSD1306_WHITE);
    display.setCursor(92, 19);
    display.setTextSize(2);
    display.println(port);

    display.drawRect(0, 38, 128, 26, SSD1306_BLACK);
    display.setTextColor(BLACK, WHITE);
    display.fillRect(0, 38, 128, 26, SSD1306_WHITE);
    display.setCursor(17, 45);
    display.setTextSize(2);
    display.println("VEG MODE");
    display.display();
}
void menuError(int moist, String port, String error)
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.drawRect(0, 0, 78, 15, SSD1306_WHITE);
    display.setCursor(16, 4);
    display.println("MOISTURE");
    display.drawRect(77, 0, 51, 15, SSD1306_WHITE);
    display.setCursor(94, 4);
    display.println("PORT");
    display.drawRect(0, 14, 78, 25, SSD1306_WHITE);
    display.setCursor(22, 19);
    display.setTextSize(2);
    display.println(moist + (String) " %");
    display.drawRect(77, 14, 51, 25, SSD1306_WHITE);
    display.setCursor(92, 19);
    display.setTextSize(2);
    display.println(port);
    display.drawRect(0, 38, 128, 26, SSD1306_WHITE);
    display.setTextColor(BLACK, WHITE);
    display.fillRect(0, 38, 128, 26, SSD1306_WHITE);
    display.setCursor(3, 43);
    display.setTextSize(1);
    display.println(error);
    // display.setCursor(43, 53);
    // display.println("STAGE 1");
    display.display();
    delay(1000);
}

// ##################################  MENUS WORK ##########################################

void watering_MENU(int a)
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.drawRect(0, 0, 128, 15, SSD1306_WHITE);
    display.setCursor(5, 4);
    display.println("CHOOSE WATERING MODE");

    display.setTextSize(1);
    display.setCursor(5, 19);
    display.println("NORMAL MODE");

    display.setCursor(5, 30);
    display.println("BURST MODE");

    if (a == 1)
    {
        display.drawRect(0, 16, 128, 13, SSD1306_WHITE);
    }
    else if (a == 2)
    {
        display.drawRect(0, 27, 128, 13, SSD1306_WHITE);
    }

    display.display();
}

void noramlModeSelected()
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.drawRect(5, 5, 123, 55, SSD1306_WHITE);
    display.setCursor(30, 30);
    display.println("Normal Mode");
    display.setCursor(40, 40);
    display.println("Selected");
    display.display();
}

void stage1Selected()
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.drawRect(5, 5, 123, 55, SSD1306_WHITE);
    display.setCursor(40, 30);
    display.println("Stage One");
    display.setCursor(40, 40);
    display.println("Selected");
    display.display();
}

void stage2Selected()
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.drawRect(5, 5, 123, 55, SSD1306_WHITE);
    display.setCursor(40, 30);
    display.println("Stage Two");
    display.setCursor(40, 40);
    display.println("Selected");
    display.display();
}

void burstmode_MENU(int a)
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.drawRect(0, 0, 128, 15, SSD1306_WHITE);
    display.setCursor(15, 4);
    display.println("BURST MODE");

    display.setTextSize(1);
    display.setCursor(5, 19);
    display.println("STAGE 1");

    display.setCursor(5, 30);
    display.println("STAGE 2");

    if (a == 1)
    {
        display.drawRect(0, 16, 128, 13, SSD1306_WHITE);
    }
    else if (a == 2)
    {
        display.drawRect(0, 27, 128, 13, SSD1306_WHITE);
    }

    display.display();
}

void pairing_MENU(int a)
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.drawRect(0, 0, 128, 15, SSD1306_WHITE);
    display.setCursor(5, 4);
    display.println("CHOOSE PAIRING MODE");

    display.setTextSize(1);
    display.setCursor(5, 19);
    display.println("Stand Alon Mode");

    display.setCursor(5, 30);
    display.println("Group mode");

    if (a == 1)
    {
        display.drawRect(0, 16, 128, 13, SSD1306_WHITE);
    }
    else if (a == 2)
    {
        display.drawRect(0, 27, 128, 13, SSD1306_WHITE);
    }

    display.display();
}

void standAloneSelected()
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.drawRect(5, 5, 123, 55, SSD1306_WHITE);
    display.setCursor(40, 30);
    display.println("Stand Alone");
    display.setCursor(40, 40);
    display.println("Selected");
    display.display();
}

void groupSelected()
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.drawRect(5, 5, 123, 55, SSD1306_WHITE);
    display.setCursor(40, 30);
    display.println("Group Mode");
    display.setCursor(40, 40);
    display.println("Selected");
    display.display();
}

void light_MENU(int a)
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.drawRect(0, 0, 128, 15, SSD1306_WHITE);
    display.setCursor(15, 4);
    display.println("CHOOSE LIGHT MODE");

    display.setTextSize(1);
    display.setCursor(5, 19);
    display.println("Always On");

    display.setCursor(5, 30);
    display.println("Automatic");

    if (a == 1)
    {
        display.drawRect(0, 16, 128, 13, SSD1306_WHITE);
    }
    else if (a == 2)
    {
        display.drawRect(0, 27, 128, 13, SSD1306_WHITE);
    }

    display.display();
}

void alwaysOn()
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.drawRect(5, 5, 123, 55, SSD1306_WHITE);
    display.setCursor(40, 30);
    display.println("Always On");
    display.setCursor(40, 40);
    display.println("Selected");
    display.display();
}

void automatic()
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.drawRect(5, 5, 123, 55, SSD1306_WHITE);
    display.setCursor(30, 30);
    display.println("Automatic Mode");
    display.setCursor(40, 40);
    display.println("Selected");
    display.display();
}

void dryProbCali_MENU()
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.drawRect(0, 0, 128, 15, SSD1306_WHITE);
    display.setCursor(8, 4);
    display.println("PROB CALIBRATION");

    display.setTextSize(1);
    display.setCursor(3, 20);
    display.println("Make Sure Probe is ");

    display.setCursor(9, 31);
    display.println("Dry then Press ");
    display.setCursor(25, 45);
    display.println("OK/Confirm");
    display.display();
}
void wetProbCali_MENU()
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.drawRect(0, 0, 128, 15, SSD1306_WHITE);
    display.setCursor(8, 4);
    display.println("PROB CALIBRATION");

    display.setTextSize(1);
    display.setCursor(3, 20);
    display.println("Make Sure Probe is ");

    display.setCursor(9, 31);
    display.println("Wet then Press ");
    display.setCursor(25, 45);
    display.println("OK/Confirm");
    display.display();
}
