#include <Arduino.h>

/*
 * Rui Santos
 * Complete Project Details https://randomnerdtutorials.com
 */
#include <SPI.h>
#include <SD.h>

#include "Nextion.h"

// Declare your Nextion objects - Example (page id = 0, component id = 1, component name = "b0")
NexText usertb = NexText(0, 2, "usrtb");
NexText vehtb = NexText(0, 3, "vehtb");
NexText timeinfo = NexText(0, 1, "info");
NexText p1c1tb = NexText(1, 1, "c1tb");
NexText p1tb1 = NexText(1, 3, "p1tb1");
NexText p1tb2 = NexText(1, 5, "p1tb2");
NexText p1tb3 = NexText(1, 7, "p1tb3");
NexText p1tb4 = NexText(1, 8, "p1tb4");
NexText p1tb5 = NexText(1, 11, "p1tb5");
NexText p1tb6 = NexText(1, 13, "p1tb6");

NexText carinfo = NexText(2, 1, "t0");
NexText p2tb1 = NexText(2, 2, "p2tb1");

NexRadio p1r1 = NexRadio(1, 2, "radio1");
NexRadio p1r2 = NexRadio(1, 4, "radio2");

NexButton bOn = NexButton(0, 2, "bOn");
NexButton bOff = NexButton(0, 3, "bOff");
NexSlider h0 = NexSlider(0, 5, "h0");
NexText tSlider = NexText(0, 6, "tSlider");
NexText tTempC = NexText(1, 5, "tTempC");
NexText tTempF = NexText(1, 4, "tTempF");
NexProgressBar jHumidity = NexProgressBar(1, 8, "jHumidity");
NexText tHumidity = NexText(1, 9, "tHumidity");
NexButton bUpdate = NexButton(1, 10, "bUpdate");

// Register a button object to the touch event list.
NexTouch *nex_listen_list[] = {
    &bOn,
    &p1r1,
    &p1r2,
    &bOff,
    &h0,
    &bUpdate,
    NULL};

/*
 * Button bOn component pop callback function.
 * When the ON button is released, the LED turns on and the state text changes.
 */
void showUserTag(String msg)
{

  usertb.setText(msg.c_str());
  delay(1000);
}
void showVehicleTag(String msg)
{

  vehtb.setText(msg.c_str());
  delay(1000);
}
void usertbPopCallback(void *ptr)
{
  // usertb.setText((String)random(0,100));
}
void vehPopCallback(void *ptr)
{
  vehtb.setText("State: on");
}

/*
 * Slider h0 component pop callback function.
 * When the slider is released, the LED brightness changes and the slider text changes.
 */

/*
 * Button bUpdate component pop callback function.
 * When the UPDATE button is released, the temperature and humidity readings are updated.
 */
void reasonTrans1(void *ptr)
{
  Serial.println("Transaction Reason 1 Selected");
}
void reasonTrans2(void *ptr)
{
  Serial.println("Transaction Reason 2 Selected");
}
void setupNExtion()
{

  // You might need to change NexConfig.h file in your ITEADLIB_Arduino_Nextion folder
  // Set the baudrate which is for debug and communicate with Nextion screen
  nexInit();

  // Register the pop event callback function of the components

  p1r1.attachPush(reasonTrans1, &p1r1);
  p1r2.attachPush(reasonTrans2, &p1r2);
}

void loopNextion(void)
{
  /*
   * When a pop or push event occured every time,
   * the corresponding component[right page id and component id] in touch event list will be asked.
   */
  nexLoop(nex_listen_list);
}