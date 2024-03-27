#include <Arduino.h>

// Ski Sign FW 2022-06-03 v3.1
//  June 3rd 2022
//
//
// Dependancies
// Does not work with ESP Boards 3.x "no matching function for call to 'HTTPClient::begin(String&, String&)'"
// Does work with ESP Boards 2.7.4
// Need this to make work on mac https://www.esp8266.com/viewtopic.php?f=32&t=21692&start=12
//
//
// Use this for Kevin's Button
// amzn1.account.AFRC3XZGABXNZANXPJZL7NUWLTAA/7498/3481/4096/1448/5936/2408/BD:64:89:B2:BE:54:4E:51:75:D0:0B:20:91:9A:A5:C7:52:36:AB:DD
//
//
//  ----- ----- ----- ----- ----- Include ----- ----- ----- ----- ----- //
#include <Arduino.h>
//#include <LedFlash.h>
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <BlinkControl.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <DNSServer.h>
#include <DoubleResetDetect.h>
// ----- ----- ----- ----- ----- END Include ----- ----- ----- ----- ----- //
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
// ----- ----- ----- ----- ----- Definitions ----- ----- ----- ----- ----- //
#define LED_PIN 16
#define COLOR_ORDER RGB
#define CHIPSET WS2811
#define NUM_LEDS 25
#define BRIGHTNESS 100
#define FRAMES_PER_SECOND 60
//
bool gReverseDirection = false;
//
CRGB leds[NUM_LEDS];
//
//
//
// WiFiClient client;
//#define AP_SSID "Brice Long Range"
//#define AP_PASSWORD  "redgreenblue"
//
//
//
//
//
// ----- ----- Captivate Portal ----- ----- //
const byte DNS_PORT = 53;     // DNS POrt
IPAddress APIP(172, 0, 0, 1); // Gateway
DNSServer dnsServer;
String oN;
int i = 0;
int statusCode;
const char *ssid = "RIGJAZZ";
const char *passphrase = "Kashmiri786";
String st;
String content;
String esid, epass, uid, btn1, btn2, btn3, btn4, btn5, btn6;
ESP8266WebServer server(80);
int x = 1; // needed for breathe
// ----- ----- END Captivate Portal ----- ----- //
//
//
//
//
//
// ----- ----- Function Decalration ----- ----- //
bool testWifi(void);
void launchWeb(void);
void createWebServer();
void setupAP(void);
String getValue(String data, char separator, int index);
// ----- ----- END Function Decalration ----- ----- //
//
//
//
//
//
// ----- ----- Double Reset ----- ----- //
// maximum number of seconds between resets that
// counts as a double reset
#define DRD_TIMEOUT 2.0
//
// address to the block in the RTC user memory
// change it if it collides with another usage
// of the address block
#define DRD_ADDRESS 0x00
//
DoubleResetDetect drd(DRD_TIMEOUT, DRD_ADDRESS);
// ----- ----- END Double Reset ----- ----- //
//
//
//
//
//
// ----- -----  API Setup ----- ----- //
// const char *host = "alterra.te2.biz";
const int httpsPort = 443; // HTTPS= 443 and HTTP = 80
String fingerprint = "2C:20:3F:B8:A9:E7:0C:A9:78:60:82:52:F8:EE:AB:86:6D:EB:72:B1";
// ----- ----- END Alexa API Setup ----- ----- //
//
//
//
//
//
// ----- ----- ----- ----- ----- END Definitions ----- ----- ----- ----- ----- //
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
// ----- ----- ----- ----- ----- Setup ----- ----- ----- ----- ----- //
void test1() {
  DynamicJsonDocument doc(25000);
Serial.begin(115200);

  //String input =
    //  "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
    String input="{\"lifts\":[{\"name\":\"BroadwayExpress1\",\"status\":\"CLOSED\",\"statusLabel\":\"Open\",\"statusColor\":\"#A02040\",\"hours\":{\"sunday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"monday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"tuesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"wednesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"thursday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"friday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"saturday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"}},\"type\":\"QuadChair\",\"mountainArea\":\"MammothMountain\",\"wearLynxGuid\":\"0b08d266-bd0d-4f4b-962e-50ee1c7d9f05\"},{\"name\":\"CanyonExpress16\",\"status\":\"CLOSED\",\"statusLabel\":\"Closed\",\"statusColor\":\"#A02040\",\"hours\":{\"sunday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"monday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"tuesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"wednesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"thursday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"friday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"saturday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"}},\"type\":\"QuadChair\",\"mountainArea\":\"MammothMountain\",\"wearLynxGuid\":\"dfc34f5a-d3af-4bf3-8726-b5cbef2556c4\"},{\"name\":\"Chair12\",\"status\":\"CLOSED\",\"statusLabel\":\"Closed\",\"statusColor\":\"#A02040\",\"hours\":{\"sunday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"monday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"tuesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"wednesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"thursday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"friday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"saturday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"}},\"type\":\"Double\",\"mountainArea\":\"MammothMountain\",\"wearLynxGuid\":\"7053d3ea-6829-4c2c-9f76-5357712de854\"},{\"name\":\"Chair13\",\"status\":\"CLOSED\",\"statusLabel\":\"Closed\",\"statusColor\":\"#A02040\",\"hours\":{\"sunday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"monday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"tuesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"wednesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"thursday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"friday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"saturday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"}},\"type\":\"Double\",\"mountainArea\":\"MammothMountain\",\"wearLynxGuid\":\"2b21e818-6317-46c2-b744-d5a568c7d4b4\"},{\"name\":\"Chair14\",\"status\":\"CLOSED\",\"statusLabel\":\"Closed\",\"statusColor\":\"#A02040\",\"hours\":{\"sunday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"monday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"tuesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"wednesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"thursday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"friday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"saturday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"}},\"type\":\"Double\",\"mountainArea\":\"MammothMountain\",\"wearLynxGuid\":\"f81819f2-5bf5-4bc4-9e62-78578fc0a7c3\"},{\"name\":\"Chair20\",\"status\":\"CLOSED\",\"statusLabel\":\"Closed\",\"statusColor\":\"#A02040\",\"hours\":{\"sunday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"monday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"tuesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"wednesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"thursday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"friday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"saturday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"}},\"type\":\"TripleChair\",\"mountainArea\":\"MammothMountain\",\"wearLynxGuid\":\"454b8d93-731d-466f-bc70-77118493fabb\"},{\"name\":\"Chair21\",\"status\":\"CLOSED\",\"statusLabel\":\"Closed\",\"statusColor\":\"#A02040\",\"hours\":{\"sunday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"monday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"tuesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"wednesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"thursday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"friday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"saturday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"}},\"type\":\"TripleChair\",\"mountainArea\":\"MammothMountain\",\"wearLynxGuid\":\"b5006a23-a19c-463b-9433-b220837c7c39\"},{\"name\":\"Chair22\",\"status\":\"CLOSED\",\"statusLabel\":\"Closed\",\"statusColor\":\"#A02040\",\"hours\":{\"sunday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"monday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"tuesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"wednesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"thursday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"friday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"saturday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"}},\"type\":\"TripleChair\",\"mountainArea\":\"MammothMountain\",\"wearLynxGuid\":\"d0881271-40d5-4860-91f9-dd372eba932a\"},{\"name\":\"Chair23\",\"status\":\"CLOSED\",\"statusLabel\":\"Closed\",\"statusColor\":\"#A02040\",\"hours\":{\"sunday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"monday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"tuesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"wednesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"thursday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"friday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"saturday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"}},\"type\":\"TripleChair\",\"mountainArea\":\"MammothMountain\",\"wearLynxGuid\":\"0ca61366-553f-4738-8494-928ff7a00651\"},{\"name\":\"Chair25\",\"status\":\"CLOSED\",\"statusLabel\":\"Closed\",\"statusColor\":\"#A02040\",\"hours\":{\"sunday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"monday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"tuesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"wednesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"thursday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"friday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"saturday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"}},\"type\":\"QuadChair\",\"mountainArea\":\"MammothMountain\",\"wearLynxGuid\":\"cc6099c2-9eeb-47f7-8334-11a0d7912f47\"},{\"name\":\"Chair7\",\"status\":\"CLOSED\",\"statusLabel\":\"Closed\",\"statusColor\":\"#A02040\",\"hours\":{\"sunday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"monday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"tuesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"wednesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"thursday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"friday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"saturday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"}},\"type\":\"TripleChair\",\"mountainArea\":\"MammothMountain\",\"wearLynxGuid\":\"595896ce-1835-4331-aeb2-274745f78c16\"},{\"name\":\"Chair8\",\"status\":\"CLOSED\",\"statusLabel\":\"Closed\",\"statusColor\":\"#A02040\",\"hours\":{\"sunday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"monday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"tuesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"wednesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"thursday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"friday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"saturday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"}},\"type\":\"TripleChair\",\"mountainArea\":\"MammothMountain\",\"wearLynxGuid\":\"3565ddd9-3647-49ce-89b9-6c4afefe35ec\"},{\"name\":\"CloudNineExpress9\",\"status\":\"CLOSED\",\"statusLabel\":\"Closed\",\"statusColor\":\"#A02040\",\"hours\":{\"sunday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"monday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"tuesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"wednesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"thursday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"friday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"saturday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"}},\"type\":\"6Chair\",\"mountainArea\":\"MammothMountain\",\"wearLynxGuid\":\"b2ea93db-abd2-4a6a-abc8-c222a914e20e\"},{\"name\":\"DiscoveryExpress11\",\"status\":\"CLOSED\",\"statusLabel\":\"Closed\",\"statusColor\":\"#A02040\",\"hours\":{\"sunday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"monday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"tuesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"wednesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"thursday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"friday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"saturday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"}},\"type\":\"QuadChair\",\"mountainArea\":\"MammothMountain\",\"wearLynxGuid\":\"c5a2067b-d02f-4d7b-9724-52e0764f9be8\"},{\"name\":\"EagleExpress15\",\"status\":\"CLOSED\",\"statusLabel\":\"Closed\",\"statusColor\":\"#A02040\",\"hours\":{\"sunday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"monday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"tuesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"wednesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"thursday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"friday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"saturday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"}},\"type\":\"6Chair\",\"mountainArea\":\"MammothMountain\",\"wearLynxGuid\":\"71eaa938-2b83-4dda-bdb6-1bf5da5086d8\"},{\"name\":\"FaceLiftExpress3\",\"status\":\"CLOSED\",\"statusLabel\":\"Closed\",\"statusColor\":\"#A02040\",\"hours\":{\"sunday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"monday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"tuesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"wednesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"thursday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"friday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"saturday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"}},\"type\":\"QuadChair\",\"mountainArea\":\"MammothMountain\",\"wearLynxGuid\":\"805ea3f0-52e2-41de-82a9-039a1ef03fd2\"},{\"name\":\"GoldRushExpress10\",\"status\":\"CLOSED\",\"statusLabel\":\"Closed\",\"statusColor\":\"#A02040\",\"hours\":{\"sunday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"monday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"tuesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"wednesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"thursday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"friday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"saturday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"}},\"type\":\"QuadChair\",\"mountainArea\":\"MammothMountain\",\"wearLynxGuid\":\"462aef23-796a-4abb-b417-8d9e3c280719\"},{\"name\":\"High5Express\",\"status\":\"CLOSED\",\"statusLabel\":\"Closed\",\"statusColor\":\"#A02040\",\"hours\":{\"sunday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"monday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"tuesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"wednesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"thursday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"friday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"saturday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"}},\"type\":\"QuadChair\",\"mountainArea\":\"MammothMountain\",\"wearLynxGuid\":\"4f05492a-87aa-4e4c-ad09-784b3d76b75c\"},{\"name\":\"PanoramaLower\",\"status\":\"CLOSED\",\"statusLabel\":\"Closed\",\"statusColor\":\"#A02040\",\"hours\":{\"sunday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"monday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"tuesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"wednesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"thursday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"friday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"saturday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"}},\"type\":\"Gondola\",\"mountainArea\":\"MammothMountain\",\"wearLynxGuid\":\"5ad64d33-3cb2-4845-b4de-519e23a32259\"},{\"name\":\"PanoramaUpper\",\"status\":\"CLOSED\",\"statusLabel\":\"Closed\",\"statusColor\":\"#A02040\",\"hours\":{\"sunday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"monday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"tuesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"wednesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"thursday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"friday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"saturday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"}},\"type\":\"Gondola\",\"mountainArea\":\"MammothMountain\",\"wearLynxGuid\":\"9db2d02c-d068-4486-8fcb-5f5fee18156e\"},{\"name\":\"RollerCoasterExpress4\",\"status\":\"CLOSED\",\"statusLabel\":\"Closed\",\"statusColor\":\"#A02040\",\"hours\":{\"sunday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"monday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"tuesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"wednesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"thursday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"friday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"saturday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"}},\"type\":\"QuadChair\",\"mountainArea\":\"MammothMountain\",\"wearLynxGuid\":\"48159b85-adbd-42dd-9198-f6badef71431\"},{\"name\":\"SchoolyardExpress17\",\"status\":\"CLOSED\",\"statusLabel\":\"Closed\",\"statusColor\":\"#A02040\",\"hours\":{\"sunday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"monday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"tuesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"wednesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"thursday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"friday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"saturday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"}},\"type\":\"QuadChair\",\"mountainArea\":\"MammothMountain\",\"wearLynxGuid\":\"d83d6fb9-df9c-4372-8d6b-4548f0b87689\"},{\"name\":\"StumpAlleyExpress2\",\"status\":\"CLOSED\",\"statusLabel\":\"Closed\",\"statusColor\":\"#A02040\",\"hours\":{\"sunday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"monday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"tuesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"wednesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"thursday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"friday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"saturday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"}},\"type\":\"QuadChair\",\"mountainArea\":\"MammothMountain\",\"wearLynxGuid\":\"054bac04-8a77-40f6-8653-cded97368740\"},{\"name\":\"UnboundExpress6\",\"status\":\"CLOSED\",\"statusLabel\":\"Closed\",\"statusColor\":\"#A02040\",\"hours\":{\"sunday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"monday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"tuesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"wednesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"thursday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"friday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"saturday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"}},\"type\":\"QuadChair\",\"mountainArea\":\"MammothMountain\",\"wearLynxGuid\":\"516bbffe-6d21-418d-a054-35ff8b418873\"},{\"name\":\"VillageGondola\",\"status\":\"CLOSED\",\"statusLabel\":\"Open\",\"statusColor\":\"#A02040\",\"hours\":{\"sunday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"monday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"tuesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"wednesday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"thursday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"friday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"},\"saturday\":{\"open\":\"7:00AM\",\"close\":\"5:00PM\"}},\"type\":\"Gondola\",\"mountainArea\":\"MammothMountain\",\"wearLynxGuid\":\"3fb403a9-6e09-4c1b-8048-0ddf702f31ee\"}]}";
  deserializeJson(doc, input);
  JsonObject obj = doc.as<JsonObject>();

  // You can use a String to get an element of a JsonObject
  // No duplication is done.

  for(int chair=0; chair<=24; chair++){
  if (doc["lifts"][chair]["statusLabel"] == "Open"){
      Serial.println(" Open");
  
    }
  

   else if ((doc["lifts"][chair]["statusLabel"] == "Expected") || (doc["lifts"][chair]["statusLabel"] == "Hold")|| (doc["lifts"][chair]["statusLabel"] == "Weather Hold") || (doc["lifts"][chair]["statusLabel"] == "Anticipated Weather Impact")){
      Serial.println(" Hold");

    }
//
//
//    
    else if (doc["lifts"][chair]["statusLabel"] == "Closed"){
      Serial.println(" Closed");

    }
  
  
  }

}
void setup()
{
  //
  //
  //
  //
  //
  // ----- ----- Serial Setup ----- ----- //
  Serial.begin(115200); // Initialising if(DEBUG)Serial Monitor
  Serial.println();
  delay(2000);
  Serial.println("Welcome to channel 115200, I'm your host Debugger!");
  // ----- ----- END Serial Setup ----- ----- //
  test1();
  //
  while (1)
  {
    /* code */
  }
  
  //
  //
  //
  //
  // ----- ----- Double Reset Hanlding ----- ----- //
  if (drd.detect())
  // if(digitalRead(rightButton) == LOW && digitalRead(middleButton) == LOW && digitalRead(leftButton) == LOW)
  {
    Serial.println("** Reset Boot **");
    Serial.println("Erasing stored WiFi credentials.");
    EEPROM.begin(512); // Initialasing EEPROM
    for (int i = 0; i < 300; ++i)
    {
      EEPROM.write(i, 0);
    }
    EEPROM.commit();
    delay(1000);
    // ESP.reset(); //for esp8266
  }
  else
  {
    Serial.println("** Normal boot **");
  }
  // ----- ----- END Double Reset Hanlding ----- ----- //
  //
  //
  //
  //
  //
  // ----- ----- Fast LED Setup ----- ----- //
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  // ----- ----- END Fast LED Setup ----- ----- //
  //
  //
  //
  //
  //
  // ----- ----- EEPROM & WIFI Setup ----- ----- //
  Serial.println("Disconnecting current wifi connection");
  WiFi.disconnect();
  EEPROM.begin(512); // Initialasing EEPROM
  delay(10);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println();
  Serial.println();
  Serial.println("Startup");
  // Read eeprom for ssid and pass
  Serial.println("Reading EEPROM ssid");
  for (int i = 0; i < 32; ++i)
  {
    esid += char(EEPROM.read(i));
  }
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(esid);
  Serial.println("Reading EEPROM pass");
  for (int i = 32; i < 96; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  Serial.print("PASS: ");
  Serial.println(epass);
  String temp;
  for (int i = 100; i < 300; ++i)
  {
    temp += char(EEPROM.read(i));
  }
  Serial.print("API setup: ");
  Serial.println(temp);
  uid = getValue(temp, '/', 0);
  btn1 = getValue(temp, '/', 1);
  btn2 = getValue(temp, '/', 2);
  btn3 = getValue(temp, '/', 3);
  btn4 = getValue(temp, '/', 4);
  btn5 = getValue(temp, '/', 5);
  btn6 = getValue(temp, '/', 6);
  // Handle Empty Fingerprint Field
  if (getValue(temp, '/', 7) != "")
  {
    fingerprint = getValue(temp, '/', 7);
    Serial.println("Fingerprint Changed");
  }
  // Try Wifi
  WiFi.begin(esid.c_str(), epass.c_str());
  if (testWifi())
  {
    Serial.println("Succesfully Connected!!!");
    // return;
  }
  else
  {
    Serial.println("Failed, Turning the HotSpot On");
    launchWeb();
    setupAP(); // Setup HotSpot
  }
  Serial.println();
  Serial.println("Waiting.");
  // Handle Waiting for Clients
  while ((WiFi.status() != WL_CONNECTED))
  {
    Serial.print(".");
    delay(100);
    server.handleClient();
    dnsServer.processNextRequest();
  }
  launchWeb();
  WiFi.softAPdisconnect(true);

  // ----- ----- END EEPROM & WIFI Setup ----- ----- //
  //
  //
  //
  //
  //
  // ----- ----- OTA Setup ----- ----- //
  ArduinoOTA.setHostname("skisign");
  ArduinoOTA.setPassword("skisign");
  ArduinoOTA.onStart([]()
                     { Serial.println("Start"); });
  ArduinoOTA.onEnd([]()
                   { Serial.println("\nEnd"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });
  ArduinoOTA.onError([](ota_error_t error)
                     {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed"); });
  ArduinoOTA.begin();
  Serial.println("OTA ready");
  // ----- ----- END OTA Setup ----- ----- //
  //
  //
  //
  //
  //




  




} // End Setup
//
//
//
//
//
// ----- ----- ----- ----- ----- END Setup ----- ----- ----- ----- ----- //
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
// ----- ----- ----- ----- ----- Funcions ----- ----- ----- ----- ----- //
//
//
//
//
//
// ----- ----- Test Wifi Function ----- ----- //
bool testWifi(void)
{
  int c = 0;
  Serial.println("Waiting for Wifi to connect");
  while (c < 20)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      return true;
    }
    delay(500);
    Serial.print("*");
    c++;
  }
  Serial.println("");
  Serial.println("Connect timed out, opening AP");
  return false;
}
// ----- ----- END Test Wifi Function ----- ----- //
//
//
//
//
//
// ----- ----- Launch Web Function ----- ----- //
void launchWeb()
{
  Serial.println("");
  if (WiFi.status() == WL_CONNECTED)
    Serial.println("WiFi connected");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
  createWebServer();
  server.begin();
  Serial.println("Server started");
}
// ----- ----- END Launch Web Function ----- ----- //
//
//
//
//
//
// ----- ----- Setup AP Function ----- ----- //
void setupAP(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      //      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");
  st = "<ol>";
  for (int i = 0; i < n; ++i)
  {
    // Print SSID and RSSI for each network found
    st += "<li>";
    st += WiFi.SSID(i);
    st += " (";
    st += WiFi.RSSI(i);
    st += ")";
    //    st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
    st += "</li>";
  }
  st += "</ol>";
  delay(100);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("Setup Brice Ski Sign");
  Serial.println("Initializing_softap_for_wifi credentials_modification");
  launchWeb();
  Serial.println("over");
}
// ----- ----- END Setup AP Function ----- ----- //
//
//
//
//
//
// ----- ----- Create Web Server Function ----- ----- //
void createWebServer()
{
  dnsServer.start(DNS_PORT, "*", APIP); // DNS spoofing (Only for HTTP)
  server.on("/", []() {
    IPAddress ip = WiFi.softAPIP();
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    content = "<!DOCTYPE HTML>\r\n<html";
    content +=  "<head><style>input[type=text] {  width: 130px;  box-sizing: border-box;  border: 2px solid #ccc;  border-radius: 4px;  font-size: 16px; background-color: white;  background-position: 10px 10px;   background-repeat: no-repeat;  padding: 12px 20px 12px 40px;  transition: width 0.4s ease-in-out;}";
    content += "h1 {text-align: center;}p {text-align: center;}div {text-align: center;}";
    content += "input[type=text]:focus {  width: 100%;}";
    content += "input[type=submit]:hover {  background-color: #45a049;}</style></head>";
    content += "<h1>Brice Ski Sign is Connected</h1>";
    content += "<div>";
    content += ipStr;
    //content += "<div><form action=\"/scan\" method=\"POST\"><input type=\"submit\" value=\"Scan\"></form>";
    content += "<form action=\"/reset\" method=\"POST\"><input type=\"submit\" value=\"Reset Ski Sign\"></form>";
    //  content += "<p>";
    //content += st+"</p>";
    //content += "<form method='get' action='setting'><label>SSID: </label><input  name='ssid' length=32> <p></P>";
    //content += " <label>Password: </label><input  name='pass' length=64><p></P>";
    //content += "<label>Setup: </label> <input type= \"text\" name='Uid' length=200><p></P>";
    //content += "<input type='submit'></form></div>";
    content += "</html>";
    oN = content;
    server.send(200, "text/html", content);
  });
  server.on("/scan", []() {
    //setupAP();
    IPAddress ip = WiFi.softAPIP();
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    st = "<ol>";
    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      st += "<li>";
      st += WiFi.SSID(i);
      st += " (";
      st += WiFi.RSSI(i);
      st += ")";
      //    st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
      st += "</li>";
    }
    st += "</ol>";
    content = "<!DOCTYPE HTML>\r\n<html>";
    content += "<p>";
    content += st;
    content += "</p>";
    server.send(200, "text/html", content);
  });
  server.on("/reset", []() {
    content = "<!DOCTYPE HTML>\r\n<html>";
    content += "<p>";
    content += "Resetting the device. Now connect again";
    content += "</p>";
    server.send(200, "text/html", content);
    for (int i = 0; i < 300; ++i) {
      EEPROM.write(i, 0);
    }
    EEPROM.commit();
    ESP.reset(); //for esp8266
    //ESP.restart(); //for esp32
  });
  server.on("/setting", []() {
    String qsid = server.arg("ssid");
    String qpass = server.arg("pass");
    delay(2000);
    Serial.println();
    Serial.println("------------------Previous Credentials-------------");
    Serial.print("SSID: ");
    Serial.println(esid);
    Serial.print("PASS: ");
    Serial.println(epass);
    Serial.println("clearing eeprom");
    for (int i = 0; i < 300; ++i) {
      EEPROM.write(i, 0);
    }
    Serial.println(qsid);
    Serial.println("");
    Serial.println(qpass);
    Serial.println("");
    Serial.println("writing eeprom ssid:");
    for (int i = 0; i < qsid.length(); ++i)
    {
      EEPROM.write(i, qsid[i]);
    }
    Serial.println("writing eeprom pass:");
    for (int i = 0; i < qpass.length(); ++i)
    {
      EEPROM.write(32 + i, qpass[i]);
    }
    //Writing api stuff
    String APIStuff = server.arg("Uid"); //+"/"+server.arg("BTN1")+"/"+server.arg("BTN2")+"/"+server.arg("BTN3")+"/"+server.arg("BTN4")+"/"+server.arg("BTN5")+"/"+server.arg("BTN6")+"/"+server.arg("fng");
    for (int i = 0; i < (APIStuff).length(); ++i)
    {
      EEPROM.write(100 + i, APIStuff[i]);
    }
    EEPROM.commit();
    content = "{\"Success\":\" Data  saved to eeprom... reset to boot into new settings\"}";
    statusCode = 200;
    //      else {
    //        content = "{\"Error\":\"404 not found\"}";
    //        statusCode = 404;
    //        Serial.println("Sending 404");
    //      }
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(statusCode, "application/json", content);
    ESP.reset(); //for esp8266
    //ESP.restart(); //for esp32
  });
  server.onNotFound([]() {
    IPAddress ip = WiFi.softAPIP();
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    content = "<!DOCTYPE HTML>\r\n<html";
    content +=  "<head><style>input[type=text] {  width: 200px;  box-sizing: border-box;  border: 2px solid #ccc;  border-radius: 4px;  font-size: 12px; background-color: white;  background-position: 10px 10px;   background-repeat: no-repeat;  padding: 8px 8px 8px 8px;  transition: width 0.4s ease-in-out;}";
    content += "h1 {text-align: center;}p {text-align: center;}div {text-align: center;}";
    content += "input[type=text]:focus {  width: 220px;}";
    content += "input[type=submit] {    background-color: #4CAF50;  border: none;  color: white;  padding: 16px 32px;  text-decoration: none;  margin: 4px 2px;  cursor: pointer;}</style></head>";
    content += "<h1>SETUP BRICE SKI SIGN</h1>";
    content += "<div>";
    st = "";
    int n = WiFi.scanNetworks();
    st += "<p>";
    st += "Found 2.4GHz WiFi Networks:";
    st += "<p>";
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      st += i+1;
      st += ".  ";
      st += WiFi.SSID(i);
    //  st += " (";
    //  st += WiFi.RSSI(i);
    //  st += ")";
    //  st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
      st += "</br>";
    }
    st += "</p>";
    st += "Enter Your Wifi Network Name (SSID) and Password (PASS)";
    st += "</br>";
    st += "Only Enter BLOB if Supplied";


    //st += "</ol>";
    content += "<p>";
    content += st;
    content += "</p>";
    //content += "<div><form action=\"/scan\" method=\"POST\"><input type=\"submit\" value=\"Scan\"></form>";
    //content += "<form action=\"/reset\" method=\"POST\"><input type=\"submit\" value=\"Reset All info\"></form>";
    //content += ipStr;
    //  content += "<p>";
    //content += st+"</p>";
    content += "<form method='get' action='setting'>";
    content += "<label>SSID </label><input  type= \"text\" name='ssid' id='ssid' length=32> <p></P>";
    content += " <label>PASS </label><input  type= \"text\" name='pass' length=64><p></P>";
    content += "<label>BLOB </label> <input type= \"text\" name='Uid' length=200><p></P>";
    content += "<input type='submit'></form></div>";
    content += "</html>";
    oN = content;
    server.send(200, "text/html", content);
  });
}
void loop()
{
  //
  //
  //
  //

  server.handleClient();
  dnsServer.processNextRequest();
  ArduinoOTA.handle();
  //
  Serial.println("START LOOP");
  //
  WiFiClientSecure client;
  client.setInsecure();
  client.setFingerprint("2C:20:3F:B8:A9:E7:0C:A9:78:60:82:52:F8:EE:AB:86:6D:EB:72:B1");
  HTTPClient http;
  http.useHTTP10(true);
   String path = "https://alterra.te2.biz/v1/conditions/ALT_MAM/lifts";
  client.connect(path, 443);

  http.begin(client, path);

    http.addHeader("X-Customer-ID", "alterra");
  http.addHeader("Authorization", "Basic ZnVlbGVkX0FQSTp6YWg5b3RhbjVVZnVQb2hmZWVRdV5hZXF1b282YXQ2cXVpZV9qYWlobzRpZUc1am9vO0ZhZXhpZThqaWVzN1No");
  //
  //  Serial.println("Break 1: 60 sec delay");
  //  delay(60000);
  //
  int httpCode = http.GET();
  Serial.print("Response Code: ");
  Serial.println(httpCode);
  Serial.println("Break 2");
  String payload1;
  payload1.reserve(24576);

  int count=0;
  while (client.available()) {
    char ch = (char)(client.read());
payload1.concat(ch);
    //Serial.print(ch);
    count++;
  ///Serial.println(count);
  }
  Serial.println("count: "+(String)count);


Serial.println("Outside HTTP 200 IF");
//
//
//
http.end();
Serial.println("After HTTP END");
Serial.println("END LOOP");
} // END Loop
//
//
//
//
//
//
// ----- ----- ----- ----- ----- END Loop ----- ----- ----- ----- ----- //
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
// ----- ----- ----- ----- ----- System Functions ----- ----- ----- ----- ----- //
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
// ----- ----- ----- ----- ----- END System Functions ----- ----- ----- ----- ----- //
