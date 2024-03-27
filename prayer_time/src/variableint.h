#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include "SPIFFS.h"
#include "Custom_RTC_DS3231.h"
#include "EEPROM.h"
         // ..........   Master For 6 Panel ........... //
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include "sys/time.h"

const int slave1Pin = 32;
const int slave2Pin = 26;
// const int slave3Pin = 25;

//char buff[500];

#define packetMaxLen 50
#define EEPROM_SIZE 125

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristicRX = NULL;

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID_RX "beb5483e-36e1-4688-b7f5-ea07361b26a1"
#define PASSKEY 000000
//SemaphoreHandle_t xGuiSemaphore;

#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <Fonts/FreeSerifBoldItalic9pt7b.h>
//#include <Fonts/font6x10.h>


File myFile;

//const char daysOfTheWeek[] PROGMEM = "SUNMONTUEWEDTHUFRISAT";
//const char monthOfTheYear[] PROGMEM = "AAAAAA[JAN[[[FEB[[MARCH[APRIL[[MAY[[[JUNE[[JULY[AUGUST[SEP[[[OCT[[[NOV[[[DEC[[";
const char startendnamaz[] PROGMEM = "FAJAR[TUL[AFISHRAQCHAST[ZAWAL[ZUHER[[ASAR[GURUB[MAGRIBAWABIN[ISHA[TAHJUD";
//const char jamat_text[] PROGMEM = "FAJZUHASRMAGISAJUM";
const char HmonthOfTheYear[] PROGMEM = "AAAAAAMUHRAMSAFAR[RAB[AWRAB[AKJAM[AWJAM[AKRAJAB[SHABANRAMZANSHAWALZUL[QDZUL[HJ";

const char daysOfTheWeek1[7][4] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
const char jamat_text1[6][7]= {"FAJAR","ZUHAR","ASAR","MAGRIB","ISHA","JUMA"};
const char monthOfTheYear[12][7]= {"JAN","FEB","MARCH","APRIL","MAY","JUNE","JULY","AUGUST","SEP","OCT","NOV","DEC"};

/*
const byte SevenSeg[10] =
{ 
     B10101111, B00001010, B11000111, B01001111,   // 0123        
     B01101010, B01101101, B11101101, B00001011,   // 4567   
     B11101111, B01101111, 
};

const byte SevenSeg2[10] =
{ 
     B10101111, B00001010, B11000111, B11001011,   // 0123        
     B01101010, B11101001, B11101101, B00001011,   // 4567   
     B11101111, B11101011, 
};


char Matdisp1[3];
char Matdisp2[3];
char Matdisp3[6];
char Matdisp4[6]; */

uint16_t row_index=0;
byte cathod2disp,row2disp,Tempe1,Tempe2,ack=0,ackt=0,acka=0,ack1=0,cday,q,cMonth,Hack=0,HDone = 0,k=1,dimtime = 0, decdig = 0;
//byte digit2disp1=SevenSeg[0],digit2disp2=SevenSeg[1],digit2disp3=SevenSeg[2],digit2disp4=SevenSeg[3],digit2disp5=SevenSeg[4],digit2disp6=SevenSeg[5],digit2disp7=SevenSeg[6],digit2disp8=SevenSeg[7],digit2disp9=SevenSeg[8],digit2disp10=SevenSeg[9];
//byte digit2disp11=SevenSeg[0],digit2disp12=SevenSeg[1],digit2disp13=SevenSeg[2],digit2disp14=SevenSeg[3],digit2disp15=SevenSeg[4],digit2disp16=SevenSeg[5],digit2disp17=SevenSeg[6],digit2disp18=SevenSeg[7],digit2disp19=SevenSeg[8],digit2disp20=SevenSeg[9],digit2disp21=SevenSeg[0];

//byte Alpha1,Alpha2,Alpha3,Alpha4,Alpha5,Alpha6,Alpha7,Alpha8,Alpha9,Alpha10,Alpha11,Alpha12,Alpha13,Alpha14,Alpha15,Alpha16,Alpha17,Alpha18;
byte Hour=10,Min=25,Sec=36,Day=02,Month=07,HDay=28,HMonth=12,hcounter=0; //Day1,Day2,Month1,Month2,Year1,Year2,Year3,Year4,Hour1,Hour2,Min1,Min2,qq,HDay1,HDay2,HMonth1,HMonth2,HYear1,HYear2,HYear3,HYear4;

uint16_t  Year=2023,HYear=1443,Temperature=0,compare_time,sstart,eend;
uint16_t  Sahar,FajrS,FajrE,TuluS,TuluE,Ishraq,ChastS,ChastE,ZawalS,ZawalE,ZuhrS,ZuhrE,AsrS,AsrE,GurubE,MaghribS,Iftar,AwwabinS,MaghribE,Isha,Tahajjud,tommorrow;
uint16_t  Fajart=0,Zuhert=0,Asart=0,Magribt=0,Ishat=0,Jumat=0,Fajara=0,Zuhera=0,Asara=0,Magriba=0,Ishaa=0,Jumaa=0,jamat_time=0,azan_time=0;

const int buzzer = 4;

uint16_t LineLength = 115;
//  001*5:33,5:38,6:54,6:55,7:15,7:16,10:01,12:20,12:22,12:27,12:28,4:22,4:23,5:55,5:58,5:59,6:00,6:19,7:16,7:17,2:40#  == 114 ( * - # )
uint16_t LineNumber = 0;
char textdata,f_command,temp1;

byte inputString[100];         // a String to hold incoming data
bool stringComplete = false; 
char counterserial = 0;

char HourArr[5];
char MinArr[5];
int tempo=0;
char jamat_timeArr[7];
char azan_timeArr[7];
byte b;
byte currentJamatIdentifier = 255;


//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


// This is how many color levels the display shows - the more the slower the update
//#define PxMATRIX_COLOR_DEPTH 8
// Defines the buffer width / the maximum width of the matrix
#define PxMATRIX_MAX_WIDTH  128
// Defines the buffer height / the maximum height of the matrix
#define PxMATRIX_MAX_HEIGHT  32  //64
// Defines how long we display things by default
//#define PxMATRIX_DEFAULT_SHOWTIME 30
// Defines the speed of the SPI bus (reducing this may help if you experience noisy images)
#define PxMATRIX_SPI_FREQUENCY 20000000
// Creates a second buffer for backround drawing (doubles the required RAM)
//#define PxMATRIX_double_buffer true

#include <PxMatrix.h>

#define P_LAT 17
#define P_A 19
#define P_B 23
#define P_C 18
#define P_D 5
#define P_E 15
#define P_OE 16
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

#define matrix_width  128
#define matrix_height  32

// This defines the 'on' time of the display is us. The larger this number,
// the brighter the display. If too large the ESP will crash
uint8_t display_draw_time=55; //30-70 is usually fine  // was 60

PxMATRIX display(128,32,P_LAT, P_OE,P_A,P_B,P_C,P_D); //WxH = 128,64

// Some standard colors
uint16_t myRED = display.color565(255, 0, 0);
uint16_t myGREEN = display.color565(0, 255, 0);
uint16_t myBLUE = display.color565(0, 0, 255);
uint16_t myWHITE = display.color565(255, 255, 255);
uint16_t myYELLOW = display.color565(255, 255, 0);
uint16_t myCYAN = display.color565(0, 255, 255);
uint16_t myMAGENTA = display.color565(255, 0, 255);
uint16_t myBLACK = display.color565(0, 0, 0);
    

#ifdef ESP32
void display_updater(){
  // Increment the counter and set the time of ISR
//  portENTER_CRITICAL_ISR(&timerMux);
  display.display(display_draw_time);
//  portEXIT_CRITICAL_ISR(&timerMux);
}
#endif


void display_update_enable(bool is_enable)
{
  if (is_enable)
  {
    timer = timerBegin(0, 80, true);  // was 80
    timerAttachInterrupt(timer, &display_updater, true);
    timerAlarmWrite(timer, 4000, true);
    timerAlarmEnable(timer);
  }
  else
  {
    timerDetachInterrupt(timer);
    timerAlarmDisable(timer);
  }
}

/*
unsigned long last_draw=0;
void scroll_text(uint8_t ypos, unsigned long scroll_delay, String text, uint8_t colorR, uint8_t colorG, uint8_t colorB)
{
    uint16_t text_length = text.length();
    display.setTextWrap(false);  // we don't wrap text so it scrolls nicely
    display.setTextSize(1);
    display.setRotation(0);
    display.setTextColor(display.color565(colorR,colorG,colorB));

    // Asuming 5 pixel average character width
    for (int xpos=matrix_width; xpos>-(matrix_width+text_length*5); xpos--)
    {
      display.setTextColor(display.color565(colorR,colorG,colorB));
      display.clearDisplay();
      display.setCursor(xpos,ypos);
      display.println(text);
      delay(scroll_delay);
      yield();

      // This might smooth the transition a bit if we go slow
      // display.setTextColor(display.color565(colorR/4,colorG/4,colorB/4));
      // display.setCursor(xpos-1,ypos);
      // display.println(text);

      delay(scroll_delay/5);
      yield();

    }
} */

uint8_t icon_index=0;

void initRCT()
{
  
  if (! rtc.begin()) {
   //    Serial.println("Couldn't find RTC");
    // while (1);
  }
  else
  {
  //  Serial.println("RTC Initialised");
  }

  if (rtc.lostPower()) {
    // Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  else
  {
 //   Serial.println("RTC Cell is OK did not loose Power..");
  }
}


void read_serial_data()
{
  counterserial = 0;
  temp1=0;
        while(inputString[counterserial] != '#')
        {
                f_command = inputString[counterserial];
                if(f_command == 'N' && temp1==0)
                {
                        temp1=21;
                }
                else if(temp1==21)
                {
                        temp1=22;
                        Fajart = (f_command - 48) * 1000;
                }
                else if(temp1==22)
                {
                        temp1=23;
                        Fajart  =  Fajart + ((f_command - 48) * 100);
                }
                else if(temp1==23)
                {
                        temp1=24;
                        Fajart =  Fajart + ((f_command - 48) * 10);
                }
                else if(temp1==24)
                {
                        temp1=25;
                        Fajart =  Fajart + (f_command - 48);
                }
                else if(temp1==25)
                {
                        temp1=26;
                        Zuhert = (f_command - 48) * 1000;
                }
                else if(temp1==26)
                {
                        temp1=27;
                        Zuhert  =  Zuhert + ((f_command - 48) * 100);
                }
                else if(temp1==27)
                {
                        temp1=28;
                        Zuhert =  Zuhert + ((f_command - 48) * 10);
                }
                else if(temp1==28)
                {
                        temp1=29;
                        Zuhert =  Zuhert + (f_command - 48) +1200;
                }
                else if(temp1==29)
                {
                        temp1=30;
                        Asart = (f_command - 48) * 1000;
                }
                else if(temp1==30)
                {
                        temp1=31;
                        Asart  =  Asart + ((f_command - 48) * 100);
                }
                else if(temp1==31)
                {
                        temp1=32;
                        Asart =  Asart + ((f_command - 48) * 10);
                }
                else if(temp1==32)
                {
                        temp1=33;
                        Asart =  Asart + (f_command - 48) +1200;
                }
                else if(temp1==33)
                {
                        temp1=34;
                        Magribt = (f_command - 48) * 1000;
                }
                else if(temp1==34)
                {
                        temp1=35;
                        Magribt  =  Magribt + ((f_command - 48) * 100);
                }
                else if(temp1==35)
                {
                        temp1=36;
                        Magribt =  Magribt + ((f_command - 48) * 10);
                }
                else if(temp1==36)
                {
                        temp1=37;
                        Magribt =  Magribt + (f_command - 48) +1200;
                }
                else if(temp1==37)
                {
                        temp1=38;
                        Ishat = (f_command - 48) * 1000;
                }
                else if(temp1==38)
                {
                        temp1=39;
                        Ishat  =  Ishat + ((f_command - 48) * 100);
                }
                else if(temp1==39)
                {
                        temp1=40;
                        Ishat =  Ishat + ((f_command - 48) * 10);
                }
                else if(temp1==40)
                {
                        temp1=41;
                        Ishat =  Ishat + (f_command - 48) +1200;
                }
                else if(temp1==41)
                {
                        temp1=42;
                        Jumat = (f_command - 48) * 1000;
                }
                else if(temp1==42)
                {
                        temp1=43;
                        Jumat  =  Jumat + ((f_command - 48) * 100);
                }
                else if(temp1==43)
                {
                        temp1=44;
                        Jumat =  Jumat + ((f_command - 48) * 10);
                }
                else if(temp1==44)
                {
                        temp1=0;
                        ackt = 1;
                        Jumat =  Jumat + (f_command - 48) +1200;
                }
    
                
                if(f_command == 'A' && temp1==0)
                {
                        temp1=51;
                }
                else if(temp1==51)
                {
                        temp1=52;
                        Fajara = (f_command - 48) * 1000;
                }
                else if(temp1==52)
                {
                        temp1=53;
                        Fajara  =  Fajara + ((f_command - 48) * 100);
                }
                else if(temp1==53)
                {
                        temp1=54;
                        Fajara =  Fajara + ((f_command - 48) * 10);
                }
                else if(temp1==54)
                {
                        temp1=55;
                        Fajara =  Fajara + (f_command - 48);
                }
                else if(temp1==55)
                {
                        temp1=56;
                        Zuhera = (f_command - 48) * 1000;
                }
                else if(temp1==56)
                {
                        temp1=57;
                        Zuhera  =  Zuhera + ((f_command - 48) * 100);
                }
                else if(temp1==57)
                {
                        temp1=58;
                        Zuhera =  Zuhera + ((f_command - 48) * 10);
                }
                else if(temp1==58)
                {
                        temp1=59;
                        Zuhera =  Zuhera + (f_command - 48);
                }
                else if(temp1==59)
                {
                        temp1=60;
                        Asara = (f_command - 48) * 1000;
                }
                else if(temp1==60)
                {
                        temp1=61;
                        Asara  =  Asara + ((f_command - 48) * 100);
                }
                else if(temp1==61)
                {
                        temp1=62;
                        Asara =  Asara + ((f_command - 48) * 10);
                }
                else if(temp1==62)
                {
                        temp1=63;
                        Asara =  Asara + (f_command - 48);
                }
                else if(temp1==63)
                {
                        temp1=64;
                        Magriba = (f_command - 48) * 1000;
                }
                else if(temp1==64)
                {
                        temp1=65;
                        Magriba  =  Magriba + ((f_command - 48) * 100);
                }
                else if(temp1==65)
                {
                        temp1=66;
                        Magriba =  Magriba + ((f_command - 48) * 10);
                }
                else if(temp1==66)
                {
                        temp1=67;
                        Magriba =  Magriba + (f_command - 48);
                }
                else if(temp1==67)
                {
                        temp1=68;
                        Ishaa = (f_command - 48) * 1000;
                }
                else if(temp1==68)
                {
                        temp1=69;
                        Ishaa  =  Ishaa + ((f_command - 48) * 100);
                }
                else if(temp1==69)
                {
                        temp1=70;
                        Ishaa =  Ishaa + ((f_command - 48) * 10);
                }
                else if(temp1==70)
                {
                        temp1=71;
                        Ishaa =  Ishaa + (f_command - 48);
                }
                else if(temp1==71)
                {
                        temp1=72;
                        Jumaa = (f_command - 48) * 1000;
                }
                else if(temp1==72)
                {
                        temp1=73;
                        Jumaa  =  Jumaa + ((f_command - 48) * 100);
                }
                else if(temp1==73)
                {
                        temp1=74;
                        Jumaa =  Jumaa + ((f_command - 48) * 10);
                }
                else if(temp1==74)
                {
                        temp1=0;
                        acka = 1;
                        Jumaa =  Jumaa + (f_command - 48);
                }
                
                else if(f_command == 'T' && temp1==0)
                {
                         temp1=1;
                }
                else if(temp1==1)
                {
                        temp1=2;
                        Hour = (f_command - 48) * 10;
                }
                else if(temp1==2)
                {
                        temp1=3;
                        Hour =  Hour + (f_command - 48);
                }
                else if(temp1==3)
                {
                        temp1=4;
                        Min = (f_command - 48) * 10;
                }
                else if(temp1==4)
                {
                        temp1=5;
                        Min =  Min + (f_command - 48);
                }
                else if(temp1==5)
                {
                        temp1=6;
                        Sec = (f_command - 48) * 10;
                }
                else if(temp1==6)
                {
                        temp1=7;
                        Sec =  Sec + (f_command - 48);
                }
                else if(temp1==7)
                {
                        temp1=8;
                        Day = (f_command - 48) * 10;
                }
                else if(temp1==8)
                {
                        temp1=9;
                        Day =  Day + (f_command - 48);
                }
                else if(temp1==9)
                {
                        temp1=10;
                        Month = (f_command - 48) * 10;
                }
                else if(temp1==10)
                {
                        temp1=11;
                        Month =  Month + (f_command - 48);
                }
                else if(temp1==11)
                {
                        temp1=12;
                        Year = (f_command - 48) * 1000;
                }
                else if(temp1==12)
                {
                        temp1=13;
                        Year =  Year + ((f_command - 48) * 100);
                }
                else if(temp1==13)
                {
                        temp1=14;
                        Year =  Year + ((f_command - 48) * 10);
                }
                else if(temp1==14)
                {
                        temp1=0;
                        ack = 1;
                        Year =  Year + (f_command - 48);
                } 


                else if(f_command == 'H' && temp1==0)
                {
                        temp1=111;
                        
                }
                else if(temp1==111)
                {
                        temp1=112;
                        HDay = (f_command - 48) * 10;
                        
                }
                else if(temp1==112)
                {
                        temp1=113;
                        HDay =  HDay + (f_command - 48);
                        
                }
                else if(temp1==113)
                {
                        temp1=114;
                        HMonth = (f_command - 48) * 10;
                        
                }
                else if(temp1==114)
                {
                        temp1=115;
                        HMonth =  HMonth + (f_command - 48);
                        
                }
                else if(temp1==115)
                {
                        temp1=116;
                        HYear = (f_command - 48) * 1000;
                        
                }
                else if(temp1==116)
                {
                        temp1=117;
                        HYear =  HYear + ((f_command - 48) * 100);
                        
                }
                else if(temp1==117)
                {
                        temp1=118;
                        HYear =  HYear + ((f_command - 48) * 10);
                }
                 else if(temp1==118)
                {
                        temp1=0;
                        Hack = 1;
                        HYear =  HYear + (f_command - 48);
                }
                              
                else if(f_command=='C' && temp1==0)
                {
                        ack1 = 1;
                }
        counterserial++;
        }
   counterserial = 0;          
}




//=====================================BLE Secure Server Classes========================================

boolean newValueReceived = 0;
byte localindexstart = 0;
byte localindexend = 0, isStringStarted = 0;


class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        if(isStringStarted == 0)
        {
          localindexend = 0;
      //    Serial.println("*******Start*******");
     //     Serial.print("New value: ");
          for (int i = 0; i < value.length(); i++)
          {
     //       Serial.print(value[i]);
            inputString[i] = value[i];
            localindexend++;
            if(value[i] == '*')
            {
              localindexstart = i+1;
              isStringStarted = 1;
            }
            if(value[i] == '#')
            {
              localindexend = i;
              newValueReceived = 1;
              isStringStarted = 0;
           //   Serial.println();
           //   Serial.println("*****End*");
            }
            if(localindexend == packetMaxLen)
            {
              localindexend = 0;
              isStringStarted = 0;
              break;
            }
          }
          
        }
        else
        {
          for (int i = 0; i < value.length(); i++)
          {
       //     Serial.print(value[i]);
            inputString[localindexend] = value[i];
            localindexend++;
           
            if(value[i] == '#')
            {
              newValueReceived = 1;
              isStringStarted = 0;
         //     Serial.println();
       //       Serial.println("*****End****");
            }
            if(localindexend == packetMaxLen)
            {
              localindexend = 0;
              isStringStarted = 0;
              break;
            }
          }
          
        }       
      }
      
    }
};

class ServerCallback: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
  //    Serial.println(" - ServerCallback - onConnect");

      int connectedCount = pServer->getConnectedCount();
  //     Serial.printf("connectedCount: %d\n", connectedCount);
  
        if (connectedCount >0) { 
          
          //pCharacteristic->setValue((uint8_t*)&connectedCount, 4);
        }
    };

    void onDisconnect(BLEServer* pServer) {
//      Serial.println(" - ServerCallback - onDisconnect");

      int connectedCount = pServer->getConnectedCount();
 //      Serial.printf("connectedCount: %d\n", connectedCount);
  
        if (connectedCount >0) { 
          
       //   pCharacteristic->setValue((uint8_t*)&connectedCount, 4);
        }
    }

  /*  void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        Serial.println("*********");
        Serial.print("New value: ");
        for (int i = 0; i < value.length(); i++)
          Serial.print(value[i]);

        Serial.println();
        Serial.println("*********");
      }
    }*/
};

class SecurityCallback : public BLESecurityCallbacks {

  uint32_t onPassKeyRequest(){
    return 000000;
  }

  void onPassKeyNotify(uint32_t pass_key){}

  bool onConfirmPIN(uint32_t pass_key){
    vTaskDelay(5000);
    return true;
  }

  bool onSecurityRequest(){
    return true;
  }

  void onAuthenticationComplete(esp_ble_auth_cmpl_t cmpl){
    if(cmpl.success){
 //     Serial.println("   - SecurityCallback - Authentication Success");       
    }else{
 //     Serial.println("   - SecurityCallback - Authentication Failure*");
      pServer->removePeerDevice(pServer->getConnId(), true);
    }
    BLEDevice::startAdvertising();
  }
};

//=====================================BLE Secure Server========================================

void bleSecurity(){
  esp_ble_auth_req_t auth_req = ESP_LE_AUTH_REQ_SC_MITM_BOND;
  esp_ble_io_cap_t iocap = ESP_IO_CAP_OUT;          
  uint8_t key_size = 16;     
  uint8_t init_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
  uint8_t rsp_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
  uint32_t passkey = PASSKEY;
  uint8_t auth_option = ESP_BLE_ONLY_ACCEPT_SPECIFIED_AUTH_DISABLE;
  esp_ble_gap_set_security_param(ESP_BLE_SM_SET_STATIC_PASSKEY, &passkey, sizeof(uint32_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_AUTHEN_REQ_MODE, &auth_req, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_IOCAP_MODE, &iocap, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_MAX_KEY_SIZE, &key_size, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_ONLY_ACCEPT_SPECIFIED_SEC_AUTH, &auth_option, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_SET_INIT_KEY, &init_key, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_SET_RSP_KEY, &rsp_key, sizeof(uint8_t));
}

void bleInit(){
  BLEDevice::init("Smart Watch");
  BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT);
  BLEDevice::setSecurityCallbacks(new SecurityCallback());

  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallback());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristicRX = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_RX,
                      BLECharacteristic::PROPERTY_WRITE
                    );



//  pCharacteristicRX->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);

  pCharacteristicRX->setCallbacks(new MyCallbacks());
//  pCharacteristicTXRX->setCallbacks(new MyCallbacks2());
  
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x0);
  BLEDevice::startAdvertising();

  bleSecurity();
}

static void ble_task(void){
      int connectedCount = pServer->getConnectedCount();
   //    Serial.printf("connectedCount: %d\n", connectedCount);
       if(connectedCount == 0)
       {
  //      display_update_enable(true);
       }
       else
       {
  //      display_update_enable(false);
       }
  
    

  /*      std::string value = pCharacteristicRX->getValue();

          if (value.length() > 0) {
            Serial.println("*********");
            Serial.print("Characterstic value: ");
            for (int i = 0; i < value.length(); i++)
              Serial.print(value[i]);
    
            Serial.println();
            Serial.println("****"); 
         
      } */
   //   pCharacteristic->notify(); 
   
  // snprintf(buff, 50, packetTX);
 // snprintf(buff, 250, packetTX);  
 
//   pCharacteristicTX->setValue(&packetTX[0],packetTXsize);    

}

///////////////////////////////////////////////////////////





void SendToSlave1()
{
  digitalWrite(slave1Pin, LOW);
  delay(700);
  get_jamat_time();
  get_azan_time();
  Serial.print("*"); 
  for(int kk=0;kk<2;kk++)
  {
    Serial.print("*");
    Serial.print("J");    
    sprintf(jamat_timeArr, "%04d",TuluS);  
    Serial.print(jamat_timeArr); 
    sprintf(jamat_timeArr, "%04d",ZawalE);  
    Serial.print(jamat_timeArr); 
    sprintf(jamat_timeArr, "%04d",GurubE);  
    Serial.print(jamat_timeArr);    
    sprintf(jamat_timeArr, "%04d",tommorrow);  
    Serial.print(jamat_timeArr);

   
    Serial.print("N");  // Fajart=0,Zuhert=0,Asart=0,Magribt=0,Ishat=0,Jumat=0,Fajara=0,Zuhera=0,Asara=0,Magriba=0,Ishaa=0,Jumaa=0
    sprintf(jamat_timeArr, "%04d",Fajart);  
    Serial.print(jamat_timeArr); 
    sprintf(jamat_timeArr, "%04d",(Zuhert-1200));  
    Serial.print(jamat_timeArr); 
    sprintf(jamat_timeArr, "%04d",(Asart-1200));  
    Serial.print(jamat_timeArr); 
    sprintf(jamat_timeArr, "%04d",(Magribt-1200));  
    Serial.print(jamat_timeArr); 
    sprintf(jamat_timeArr, "%04d",(Ishat-1200));  
    Serial.print(jamat_timeArr); 
    sprintf(jamat_timeArr, "%04d",(Jumat-1200));  
    Serial.print(jamat_timeArr); 
    Serial.print("#");
    delay(200);  
  }
  digitalWrite(slave1Pin, HIGH);
}

/*
void SendToSlave3()
{
  digitalWrite(slave3Pin, LOW);
  delay(700);
  get_jamat_time();
//  get_azan_time();
  Serial.print("@"); 
  for(int kk=0;kk<3;kk++)
  {
    Serial.print("@");
    Serial.print("J");  
    sprintf(jamat_timeArr, "%02d",Jumat);  
    Serial.print(jamat_timeArr);   
    sprintf(jamat_timeArr, "%04d",TuluS);  
    Serial.print(jamat_timeArr); 
    sprintf(jamat_timeArr, "%04d",ZawalE);  
    Serial.print(jamat_timeArr); 
    sprintf(jamat_timeArr, "%04d",GurubE);  
    Serial.print(jamat_timeArr);    
    sprintf(jamat_timeArr, "%04d",tommorrow);  
    Serial.print(jamat_timeArr);
    Serial.print("$");
    delay(200);  
  }
  digitalWrite(slave3Pin, HIGH);
}        */

void SendToSlave2()
{
   digitalWrite(slave2Pin, LOW);
   delay(700);
   int p = 24;
   HDay = EEPROM.read(p);
   p++;
   HMonth = EEPROM.read(p);
   p++;

   HYear = EEPROM.read(p) * 100;
   p++;
   HYear = HYear + EEPROM.read(p);
   
   DateTime now = rtc.now(); 
    Hour = now.hour();
    Min = now.minute();
    Sec = now.second();
    Day = now.day();
    Month = now.month();
    Year = now.year();
    
    
  Serial.print("!"); 
  for(int kk=0;kk<3;kk++)
  {
    Serial.print("!");
    Serial.print("T");  
    sprintf(jamat_timeArr, "%02d",Hour);  
    Serial.print(jamat_timeArr);   
    sprintf(jamat_timeArr, "%02d",Min);  
    Serial.print(jamat_timeArr); 
    sprintf(jamat_timeArr, "%02d",Sec);  
    Serial.print(jamat_timeArr); 
    sprintf(jamat_timeArr, "%02d",Day);  
    Serial.print(jamat_timeArr);    
    sprintf(jamat_timeArr, "%02d",Month);  
    Serial.print(jamat_timeArr);
    sprintf(jamat_timeArr, "%04d",Year);  
    Serial.print(jamat_timeArr);

    Serial.print("H");
    sprintf(jamat_timeArr, "%02d",HDay);  
    Serial.print(jamat_timeArr);    
    sprintf(jamat_timeArr, "%02d",HMonth);  
    Serial.print(jamat_timeArr);
    sprintf(jamat_timeArr, "%04d",HYear);  
    Serial.print(jamat_timeArr);
    Serial.print("%");
    delay(200);  
  }
  digitalWrite(slave2Pin, HIGH);
}


void serialEvent() 
{
  byte counting = 0;
  char inChar;
  while (Serial.available()) 
  {
    inChar = Serial.read();
    inputString[counting] = inChar;
    counting++;
    if (inChar == '#') 
    {
      newValueReceived = true;
    }
    delay(5);
  }
}

void loop() {
  delay(600);
  if(Serial.available()) 
  {
      display_update_enable(false);
      serialEvent();
 
    
    if(newValueReceived == 1)
    {
      newValueReceived = 0;
      read_serial_data();
  //    Serial.println(inputString);
      if(ack == 1)
      {
        rtc.adjust(DateTime(Year, Month, Day, Hour, Min, Sec));// to set the time manualy
        ack = 0;
        currentJamatIdentifier = 255;
        SendToSlave2();
      }
    }
    
   
    
    if(ackt == 1)
    {
      store_jamat_time();
     // store_times();
      ackt = 0; 
      SendToSlave1();
      currentJamatIdentifier = 255; 
    //  SendToSlave3();
    }
  
    if(acka == 1)
    {
      store_azan_time();
      acka = 0; 
    //  SendToSlave1();
    }    
  
    if(Hack == 1)
    {
      int p,u;
      
      Hack = 0; 
      p = 24; 
  
      EEPROM.write(p, HDay);
      EEPROM.commit();
      p++;
  
      EEPROM.write(p, HMonth);
      EEPROM.commit();
      p++;
  
      u = HYear/100; 
      EEPROM.write(p, u);
      EEPROM.commit();
      p++;
      u = HYear - (u*100);
      EEPROM.write(p, u);
      EEPROM.commit();

      SendToSlave2();
    }
    display_update_enable(true);
  }
  
  if( cday != Day )
  {
    display_update_enable(false);
    find_linenumber();
    get_tdata();
    cday = Day;
    currentJamatIdentifier = 255;
   // SendToSlave3();
    display_update_enable(true);
 //   cMonth = Month;
  }

  
   DateTime now = rtc.now();
    Hour = now.hour();
    Min = now.minute();
    Day = now.day();
    Month = now.month();


    if(hcounter < 11)
    {
 /*   Matdisp4[0] = pgm_read_byte_near(monthOfTheYear +(Month * 6)); //(monthOfTheYear[Month * 3]) ;
    Matdisp4[1] = pgm_read_byte_near(monthOfTheYear +(Month * 6)+ 1); //(monthOfTheYear[ (Month * 3)+1 ]) ;    
    Matdisp4[2] = pgm_read_byte_near(monthOfTheYear +(Month * 6)+ 2); //(monthOfTheYear[ (Month * 3)+2 ]) ;
    Matdisp4[3] = pgm_read_byte_near(monthOfTheYear +(Month * 6)+ 3); 
    Matdisp4[4] = pgm_read_byte_near(monthOfTheYear +(Month * 6)+ 4);
    Matdisp4[5] = pgm_read_byte_near(monthOfTheYear +(Month * 6)+ 5); */
    
    }
    else if(hcounter > 10)
    {
 /*   Matdisp4[0] = pgm_read_byte_near(HmonthOfTheYear +(HMonth * 6)); //(monthOfTheYear[Month * 3]) ;
    Matdisp4[1] = pgm_read_byte_near(HmonthOfTheYear +(HMonth * 6)+ 1); //(monthOfTheYear[ (Month * 3)+1 ]) ;
    Matdisp4[2] = pgm_read_byte_near(HmonthOfTheYear +(HMonth * 6)+ 2); //(monthOfTheYear[ (Month * 3)+2 ]) ;
    Matdisp4[3] = pgm_read_byte_near(HmonthOfTheYear +(HMonth * 6)+ 3);
    Matdisp4[4] = pgm_read_byte_near(HmonthOfTheYear +(HMonth * 6)+ 4);
    Matdisp4[5] = pgm_read_byte_near(HmonthOfTheYear +(HMonth * 6)+ 5); */
    
    }

    if(hcounter == 21)
    {
      hcounter = 0;
    }
    hcounter++;
    
    Year = now.year();
    
    compare_time = 0;
    compare_time = ((Hour/10)%10) *1000;
    compare_time = compare_time + (((Hour)%10) *100) ;
    compare_time = compare_time + (((Min/10)%10) *10);
    compare_time = compare_time + ((Min)%10) ;

    if(Hour > 12)
    {
      Hour = Hour - 12;
    }
    else if(Hour == 0)
    {
      Hour = 12;
    }

    
   

     if(((Magriba+1200) == compare_time) && (HDone == 0))
     {
        HDone = 1;
        HDay++;
        if(HDay == 31)
        {
          HDay = 1;
          HMonth++;
          if(HMonth == 13)
          {
            HMonth = 1;
            HYear++;
          }
          
        }
        Hack = 1;
     }
     else if( Magriba != (compare_time - 1200) )
     {
        HDone = 0;
     }

     if(Hack == 1)
    {
      int p,u;
      
      Hack = 0; 
      p = 24; 
  
      EEPROM.write(p, HDay);
      EEPROM.commit();
      p++;
  
      EEPROM.write(p, HMonth);
      EEPROM.commit();
      p++;
  
      u = HYear/100; 
      EEPROM.write(p, u);
      EEPROM.commit();
      p++;
      u = HYear - (u*100);
      EEPROM.write(p, u);
      EEPROM.commit();
    }


    if( (Sec < 3) && (compare_time > (Fajara-10)) )
    {
      if( (Fajara == compare_time) || ((Asara+1200) == compare_time) || ((Magriba+1200) == compare_time) || ((Ishaa+1200) == compare_time) || (Fajart == compare_time) || (Asart == compare_time) || (Magribt == compare_time) || (Ishat == compare_time))
      {
          digitalWrite(buzzer, HIGH);
      }
      else if((now.dayOfTheWeek() != 5) && ( (Zuhera == compare_time) || (Zuhert == compare_time) || ((Zuhera+1200) == compare_time) ) )
      {
          digitalWrite(buzzer, HIGH);
      }
      else if((now.dayOfTheWeek() == 5) && ( (Jumaa == compare_time) || (Jumat == compare_time) || ((Jumaa+1200) == compare_time) ) )
      {
         digitalWrite(buzzer, HIGH);
      }

    }
    else
    {
      digitalWrite(buzzer, LOW);
    } 
      
 /*********************************   AZAN   *****************************************************/  
 if( (Sec < 3) && (compare_time > (Fajara-10)) )   // pinMode(leda,  OUTPUT); For Azan time indication 
    {
      if( (Fajara == compare_time) || ((Asara+1200) == compare_time) || ((Magriba+1200) == compare_time) || ( (Ishaa+1200) == compare_time) ) 
      {
        //  digitalWrite(leda, HIGH);
      }
      else if((now.dayOfTheWeek() != 5) && ( (Zuhera == compare_time) || ((Zuhera+1200) == compare_time) ) )
      {
       //   digitalWrite(leda, HIGH);
      }
      else if((now.dayOfTheWeek() == 5) && ( (Jumaa == compare_time) || ((Jumaa+1200) == compare_time) ) )
      {
       //  digitalWrite(leda, HIGH);
      }
      else
      {
       // digitalWrite(leda, LOW);
      }
/*****************************  NAMAJ TIME  ************************************************************/
      if( (Fajart == compare_time) || (Asart == compare_time) || (Magribt == compare_time) || (Ishat == compare_time) )
      {
       //   digitalWrite(ledn, HIGH);
      }
      else if( (now.dayOfTheWeek() != 5) && ( (Zuhert == compare_time) ) )
      {
      //    digitalWrite(ledn, HIGH);
      }
      else if( (now.dayOfTheWeek() == 5) && ( (Jumat == compare_time) ) )
      {
      //   digitalWrite(ledn, HIGH);
      }
      else
      {
     // digitalWrite(ledn, LOW);
      }
    }   
       

  /************************ Time Slot for Brightness : ***********************************************/
  
    if( (Ishat+30 <= compare_time) || (compare_time < Fajara) )    
      {
        dimtime = 1;
        display.setBrightness(90);
      }
      else
      {
        dimtime = 0;
        display.setBrightness(255);
      }
/************************************************************************************/ 

    if((Ishat+15 <=compare_time) || (compare_time <= Fajart+15))
    {
      jamat_time = Fajart;
      azan_time = Fajara;
      b = 0;
    }
    else if((Fajart+15 <=compare_time) && (compare_time<= Zuhert+15) && (now.dayOfTheWeek() != 5))
    {
      if(Zuhert < 1300)
      {
        jamat_time = Zuhert; 
      }
      else
      {
        jamat_time = Zuhert - 1200;
      }
      azan_time = Zuhera;
      b = 1;
    }
    else if((Fajart+15 <=compare_time) && (compare_time<= Jumat+15) && (now.dayOfTheWeek() == 5))
    {
      if(Jumat < 1300)
      {
        jamat_time = Jumat; 
      }
      else
      {
        jamat_time =Jumat - 1200;
      }
      azan_time = Jumaa;
      b = 5;
    }
    else if((Zuhert+15 <=compare_time) && (compare_time<= Asart+15))
    {
      jamat_time = Asart - 1200;
      azan_time = Asara;
      b = 2;
    }
    else if((Asart+15 <=compare_time) && (compare_time<= Magribt+15))
    {
      jamat_time = Magribt - 1200;
      azan_time = Magriba;
      b = 3;
    }
    else if((Magribt+15 <=compare_time) && (compare_time<= Ishat+15))
    {
      jamat_time = Ishat - 1200;
      azan_time = Ishaa;
      b = 4;
    }

    if(currentJamatIdentifier != b)
    {
      currentJamatIdentifier = b;
      tommorrow = jamat_time;
      if((b == 1) && (now.dayOfTheWeek() == 4))
      {
        tommorrow = Jumat;
      }

      if(b == 5)
      {
        tommorrow = Zuhert;
      }

      if(b==3)
      {
        
      }
      
      
      display_update_enable(false);
      SendToSlave1();   // .......     For 6P Update
      display_update_enable(true);
    }


    tempo = rtc.getTemperature();
    sprintf(jamat_timeArr, "%02d:%02d",(jamat_time/100),(jamat_time - ((jamat_time/100)*100))); 
    sprintf(HourArr, "%02d",Hour);
    sprintf(MinArr, "%02d",Min);
    sprintf(azan_timeArr, "%02d:%02d",(azan_time/100),(azan_time - ((azan_time/100)*100)));
    
    display.setTextSize(2);
    display.setTextColor(myGREEN);
    display.setCursor(3,2);  
    display.clearDisplay();
    display.print(HourArr);
    

    if(Sec != now.second())
    {
      Sec = now.second();
      display.print(":");
   //   delay(100);     
    }
    else
    {
      display.print(" "); 
    }  
    display.print(MinArr);

//    display.setTextSize(2);
//    display.setTextColor(myGREEN); 
//    sprintf(jamat_timeArr, "%02d",(jamat_time/100));
    display.setCursor(67,2);
    display.print(jamat_timeArr);   
//    display.print(":"); 
//    sprintf(jamat_timeArr, "%02d",(jamat_time/100));

    display.setCursor(67,23);
    display.setTextSize(1);
    display.setTextColor(myBLUE);
    display.print(jamat_text1[b]);

    display.setCursor(3,23);
    display.setTextSize(1);
//    display.setTextColor(myBLUE);
    display.print(daysOfTheWeek1[now.dayOfTheWeek()]);
    

    display.setCursor(98,23);
//    display.setTextSize(1);
    display.setTextColor(myRED);
    display.print(azan_timeArr);

    display.setCursor(50,23);
//    display.setTextSize(1);
//    display.setTextColor(myRED);
    display.print(tempo);

    
  //  display_update_enable(true);

    //-------------------------------------------------------
    
    if((FajrS <=compare_time) && (compare_time<= FajrE))
    {
      sstart = FajrS;
      eend = FajrE; 
      b = 0;
    }
    else if((TuluS <=compare_time) && (compare_time<= TuluE))
    {
      sstart = TuluS;
      eend = TuluE;
      b = 1;
   //   digitalWrite(ledm, HIGH);
    }
    else if((Ishraq <=compare_time) && (compare_time<= ChastS))
    {
      sstart = Ishraq;
      eend = ChastS;
     // eend = ChastS;
      b = 2;
   //   digitalWrite(ledm, LOW);
    }
    else if((ChastS <=compare_time) && (ChastE >=compare_time))
    {
      sstart = ChastS;
      eend = ChastE;
      b = 3;
    }
    else if((ZawalS <=compare_time) && (compare_time<= ZawalE))
    {
      sstart = ZawalS;
      eend = ZawalE;
      b = 4;
   //   digitalWrite(ledm, HIGH);
    }
    else if((ZuhrS <=compare_time) && (compare_time<= ZuhrE))
    {
      sstart = ZuhrS;
      eend = ZuhrE - 1200;
      b = 5;
    //  digitalWrite(ledm, LOW);
    }
    else if((AsrS <=compare_time) && (compare_time<= AsrE))
    {
      sstart = AsrS - 1200;
      eend = AsrE - 1200;
      b = 6;
    }
    else if((AsrE <=compare_time) && (compare_time<= GurubE))
    {
      sstart = AsrE - 1200;
      eend = GurubE - 1200;
      b = 7;
   //   digitalWrite(ledm, HIGH);
    }
    else if((MaghribS <=compare_time) && (compare_time<= MaghribE)) 
    { 
      if((AwwabinS <=compare_time) && (compare_time<= MaghribE))
      {
          if(hcounter < 11)
          {
            sstart = AwwabinS - 1200;
            eend = MaghribE - 1200;
            b = 9;
          }
          else if(hcounter > 10)
          {
             sstart = MaghribS - 1200;
             eend = MaghribE - 1200;
             b = 8;
          }
      }
      else
      {
      sstart = MaghribS - 1200;
      eend = MaghribE - 1200;
      b = 8 ;
      }
   //   digitalWrite(ledm, LOW);
    }
    
    else if((Isha <=compare_time ) || (compare_time<= Sahar)) 
    { 
      if((Tahajjud <=compare_time) && (compare_time<= Sahar))
      {
          if(hcounter < 11)
          {
            sstart = Tahajjud;
            eend = Sahar;
            b = 11;
          }
          else if(hcounter > 10)
          {
             sstart = Isha - 1200;
             eend = Sahar;
             b = 10;
          }
      }
      else
      {
        sstart = Isha - 1200;
        eend = Sahar;
        b = 10;
      }
    }
    else
    {
      b = 100;
    }


    if(b != 100)
    {
 /*     Matdisp3[0] = pgm_read_byte_near(startendnamaz +(b*6)+0 ); //startendnamaz[(b*6)+0];
      Matdisp3[1] = pgm_read_byte_near(startendnamaz +(b*6)+1 ); //startendnamaz[(b*6)+1];

      Matdisp3[2] = pgm_read_byte_near(startendnamaz +(b*6)+2 ); //startendnamaz[(b*6)+2];
      Matdisp3[3] = pgm_read_byte_near(startendnamaz +(b*6)+3 ); //startendnamaz[(b*6)+3];

      Matdisp3[4] = pgm_read_byte_near(startendnamaz +(b*6)+4 ); //startendnamaz[(b*6)+4];
      Matdisp3[5] = pgm_read_byte_near(startendnamaz +(b*6)+5 ); //startendnamaz[(b*6)+5]; */
    }
   // delay(500); 
}


void find_linenumber()
{
  uint8_t i=0;
  uint8_t daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
  if (Year%4  == 0)
  {
    if (Year%100 != 0)
    {
      daysInMonth[1] = 29;
    }
    else
    {
      if (Year%400 == 0)
      {
        daysInMonth[1] = 29;
      }
    }
  }
  
  LineNumber = 0;
  Month = Month - 1;
  for(i = 0;i < Month;i++)
  {
    LineNumber = LineNumber + daysInMonth[i];
  }
  
  LineNumber = LineNumber + Day - 2;
}


void get_tdata()
{
  byte p=0;
  
  myFile = SPIFFS.open("/database.txt", "r");
  if(myFile)
  {
    myFile.seek((LineNumber * LineLength) + 67 + LineNumber);  
    
    // 001*5:33,5:38,6:54,6:55,7:15,7:16,10:01,12:20,12:22,12:27,12:28,4:22,4:23,5:55,5:58,5:59,6:00,6:19,7:16,7:17,2:40#  == 68 ( Total Digits In between * to # )
        
        textdata = myFile.read();
        while(textdata != '*')
        {
    //      tday[p] = textdata - 48;
          p++;
          textdata = myFile.read();
        }

   p=50;
  while(true)
  {
       textdata = myFile.read();
       if(textdata == '#')
       {
        break;
       }
       else if( (textdata != '*') && (textdata != ',') && (textdata != ':'))
       {
    //    Serial.write(textdata);          
        EEPROM.write(p, (textdata-48)); 
        EEPROM.commit();
   //     Timetable[p] = SevenSeg[textdata-48];
        p++;
       }
   }
    
   myFile.close();
   p = 50;
   Sahar = 0;
   Sahar = EEPROM.read(p) * 100;
   p++;
   Sahar = Sahar + (EEPROM.read(p) * 10); 
   p++;
   Sahar = Sahar + EEPROM.read(p);
   p++;
   
   FajrS = 0;
   FajrS = EEPROM.read(p) * 100;
   p++;
   FajrS = FajrS + (EEPROM.read(p) * 10); 
   p++;
   FajrS = FajrS + EEPROM.read(p);
   p++;

   FajrE = 0;
   FajrE = EEPROM.read(p) * 100;
   p++;
   FajrE = FajrE + (EEPROM.read(p) * 10); 
   p++;
   FajrE = FajrE + EEPROM.read(p);
   p++;

   TuluS = 0;
   TuluS = EEPROM.read(p) * 100;
   p++;
   TuluS = TuluS + (EEPROM.read(p) * 10); 
   p++;
   TuluS = TuluS + EEPROM.read(p);
   p++;

   TuluE = 0;
   TuluE = EEPROM.read(p) * 100;
   p++;
   TuluE = TuluE + (EEPROM.read(p) * 10); 
   p++;
   TuluE = TuluE + EEPROM.read(p);
   p++;

   Ishraq = 0;
   Ishraq = EEPROM.read(p) * 100;
   p++;
   Ishraq = Ishraq + (EEPROM.read(p) * 10); 
   p++;
   Ishraq = Ishraq + EEPROM.read(p);
   p++;

   ChastS = 0;
   ChastS = EEPROM.read(p) * 1000;
   p++;
   ChastS = ChastS + (EEPROM.read(p) * 100);
   p++;
   ChastS = ChastS + (EEPROM.read(p) * 10); 
   p++;
   ChastS = ChastS + EEPROM.read(p);
   p++;

   ChastE = 0;
   ChastE = EEPROM.read(p) * 1000;
   p++;
   ChastE = ChastE + (EEPROM.read(p) * 100);
   p++;
   ChastE = ChastE + (EEPROM.read(p) * 10); 
   p++;
   ChastE = ChastE + EEPROM.read(p);
   p++;

   ZawalS = 0;
   ZawalS = EEPROM.read(p) * 1000;
   p++;
   ZawalS = ZawalS + (EEPROM.read(p) * 100);
   p++;
   ZawalS = ZawalS + (EEPROM.read(p) * 10); 
   p++;
   ZawalS = ZawalS + EEPROM.read(p);
   p++;

   ZawalE = 0;
   ZawalE = EEPROM.read(p) * 1000;
   p++;
   ZawalE = ZawalE + (EEPROM.read(p) * 100);
   p++;
   ZawalE = ZawalE + (EEPROM.read(p) * 10); 
   p++;
   ZawalE = ZawalE + EEPROM.read(p);
   p++;

   ZuhrS = 0;
   ZuhrS = EEPROM.read(p) * 1000;
   p++;
   ZuhrS = ZuhrS + (EEPROM.read(p) * 100);
   p++;
   ZuhrS = ZuhrS + (EEPROM.read(p) * 10); 
   p++;
   ZuhrS = ZuhrS + EEPROM.read(p);
   p++;

   ZuhrE = 0;
   ZuhrE = EEPROM.read(p) * 100;
   p++;
   ZuhrE = ZuhrE + (EEPROM.read(p) * 10); 
   p++;
   ZuhrE = ZuhrE + EEPROM.read(p);
   p++;
   ZuhrE = ZuhrE + 1200;

   AsrS = 0;
   AsrS = EEPROM.read(p) * 100;
   p++;
   AsrS = AsrS + (EEPROM.read(p) * 10); 
   p++;
   AsrS = AsrS + EEPROM.read(p);
   p++;
   AsrS = AsrS + 1200;

   AsrE = 0;
   AsrE = EEPROM.read(p) * 100;
   p++;
   AsrE = AsrE + (EEPROM.read(p) * 10); 
   p++;
   AsrE = AsrE + EEPROM.read(p);
   p++;
   AsrE = AsrE + 1200;

   GurubE = 0;
   GurubE = EEPROM.read(p) * 100;
   p++;
   GurubE = GurubE + (EEPROM.read(p) * 10); 
   p++;
   GurubE = GurubE + EEPROM.read(p);
   p++;
   GurubE = GurubE + 1200;

   MaghribS = 0;
   MaghribS = EEPROM.read(p) * 100;
   p++;
   MaghribS = MaghribS + (EEPROM.read(p) * 10); 
   p++;
   MaghribS = MaghribS + EEPROM.read(p);
   p++;
   MaghribS = MaghribS + 1200;

   Iftar = 0;
   Iftar = EEPROM.read(p) * 100;
   p++;
   Iftar = Iftar + (EEPROM.read(p) * 10); 
   p++;
   Iftar = Iftar + EEPROM.read(p);
   p++;
   Iftar = Iftar +1200;
   
   AwwabinS = 0;
   AwwabinS = EEPROM.read(p) * 100;
   p++;
   AwwabinS = AwwabinS + (EEPROM.read(p) * 10); 
   p++;
   AwwabinS = AwwabinS + EEPROM.read(p);
   p++;
   AwwabinS = AwwabinS +1200;

   MaghribE = 0;
   MaghribE = EEPROM.read(p) * 100;
   p++;
   MaghribE = MaghribE + (EEPROM.read(p) * 10); 
   p++;
   MaghribE = MaghribE + EEPROM.read(p);
   p++;
   MaghribE = MaghribE + 1200;

   Isha = 0;
   Isha = EEPROM.read(p) * 100;
   p++;
   Isha = Isha + (EEPROM.read(p) * 10); 
   p++;
   Isha = Isha + EEPROM.read(p);
   p++;
   Isha = Isha + 1200;

   Tahajjud = 0;
   Tahajjud = EEPROM.read(p) * 100;
   p++;
   Tahajjud = Tahajjud + (EEPROM.read(p) * 10); 
   p++;
   Tahajjud = Tahajjud + EEPROM.read(p);
   p++;

   
  }
  else 
  {
 //   Serial.println("error opening database.txt");
  }
}


void store_jamat_time()
{
  byte p,u;

  p = 0;
   
  u = Fajart/100; 
  EEPROM.write(p, u);
  EEPROM.commit();
  p++;
  u = Fajart - (u*100);
  EEPROM.write(p, u);
  EEPROM.commit();
  p++;

  if(Zuhert > 1700)
  {
    Zuhert = Zuhert - 1200;
  }
  
  u = Zuhert/100; 
  EEPROM.write(p, u);
  EEPROM.commit();
  p++;
  u = Zuhert - (u*100);
  EEPROM.write(p, u);
  EEPROM.commit();
  p++;

  u = Asart/100; 
  EEPROM.write(p, u);
  EEPROM.commit();
  p++;
  u = Asart - (u*100);
  EEPROM.write(p, u);
  EEPROM.commit();
  p++;

  u = Magribt/100; 
  EEPROM.write(p, u);
  EEPROM.commit();
  p++;
  u = Magribt - (u*100);
  EEPROM.write(p, u);
  EEPROM.commit();
  p++;

  u = Ishat/100; 
  EEPROM.write(p, u);
  EEPROM.commit();
  p++;
  u = Ishat - (u*100);
  EEPROM.write(p, u);
  EEPROM.commit();
  p++;

  if(Jumat > 1700)
  {
    Jumat = Jumat - 1200;
  }

  u = Jumat/100; 
  EEPROM.write(p, u);
  EEPROM.commit();
  p++;
  u = Jumat - (u*100);
  EEPROM.write(p, u);
  EEPROM.commit();
  p++;
}


void store_azan_time()
{
  byte p,u;

  p = 12;
   
  u = Fajara/100; 
  EEPROM.write(p, u);
  EEPROM.commit();
  p++;
  u = Fajara - (u*100);
  EEPROM.write(p, u);
  EEPROM.commit();
  p++;
  
  u = Zuhera/100; 
  EEPROM.write(p, u);
  EEPROM.commit();
  p++;
  u = Zuhera - (u*100);
  EEPROM.write(p, u);
  EEPROM.commit();
  p++;

  u = Asara/100; 
  EEPROM.write(p, u);
  EEPROM.commit();
  p++;
  u = Asara - (u*100);
  EEPROM.write(p, u);
  EEPROM.commit();
  p++;

  u = Magriba/100; 
  EEPROM.write(p, u);
  EEPROM.commit();
  p++;
  u = Magriba - (u*100);
  EEPROM.write(p, u);
  EEPROM.commit();
  p++;

  u = Ishaa/100; 
  EEPROM.write(p, u);
  EEPROM.commit();
  p++;
  u = Ishaa - (u*100);
  EEPROM.write(p, u);
  EEPROM.commit();
  p++;

  u = Jumaa/100; 
  EEPROM.write(p, u);
  EEPROM.commit();
  p++;
  u = Jumaa - (u*100);
  EEPROM.write(p, u);
  EEPROM.commit();
  p++;
}

void get_jamat_time()
{
  int p;
  p = 0;
  
   Fajart = 0;
   Fajart = EEPROM.read(p) * 100;
   p++;
   Fajart = Fajart + EEPROM.read(p);
   p++;

   Zuhert = 0;
   Zuhert = (EEPROM.read(p) * 100);
   p++;
   Zuhert = Zuhert + EEPROM.read(p);
   p++;

   Asart = 0;
   Asart = EEPROM.read(p) * 100;
   p++;
   Asart = Asart + EEPROM.read(p);
   p++;

   Magribt = 0;
   Magribt = EEPROM.read(p) * 100;
   p++;
   Magribt = Magribt + EEPROM.read(p);
   p++;
   
   Ishat = 0;
   Ishat = EEPROM.read(p) * 100;
   p++;
   Ishat = Ishat + EEPROM.read(p);
   p++;

   Jumat = 0;
   Jumat = EEPROM.read(p) * 100;
   p++;
   Jumat = Jumat + EEPROM.read(p);
   p++;
}

void get_azan_time()
{
  int p;
  p = 12;
  
   Fajara = 0;
   Fajara = EEPROM.read(p) * 100;
   p++;
   Fajara = Fajara + EEPROM.read(p);
   p++;

   Zuhera = 0;
   Zuhera = (EEPROM.read(p) * 100);
   p++;
   Zuhera = Zuhera + EEPROM.read(p);
   p++;

   Asara = 0;
   Asara = EEPROM.read(p) * 100;
   p++;
   Asara = Asara + EEPROM.read(p);
   p++;

   Magriba = 0;
   Magriba = EEPROM.read(p) * 100;
   p++;
   Magriba = Magriba + EEPROM.read(p);
   p++;
   
   Ishaa = 0;
   Ishaa = EEPROM.read(p) * 100;
   p++;
   Ishaa = Ishaa + EEPROM.read(p);
   p++;

   Jumaa = 0;
   Jumaa = EEPROM.read(p) * 100;
   p++;
   Jumaa = Jumaa + EEPROM.read(p);
   p++;
}

/*
boolean readjsonfile()
{
  
  if (SPIFFS.exists("/times.json"))
  {
    File tempFile = SPIFFS.open("/times.json", "r");
    if (!tempFile)
    {
       Serial.println("tempFile = 0 ");
       return false;
    }
    Serial.println("tempFile = 1 ");
    size_t size = tempFile.size();
    // Allocate a buffer to store contents of the file.
    std::unique_ptr<char[]> buf(new char[size]);
    tempFile.readBytes(buf.get(), size);
    tempFile.close();
    Serial.println("tempFile = 2 ");
    DynamicJsonBuffer jsonBuffer(size + 20);
    JsonArray& root1 = jsonBuffer.parseArray(buf.get()); 
    Serial.println("tempFile = 3 "); 
    JsonObject& root = root1[0];
    Serial.println("tempFile = 4 ");
    fajar = root["F"];
    tulu = root["T"];
    zuhar = root["Z"];
    asar = root["A"];
    magrib = root["M"];
    isha = root["I"];
    jsonBuffer.clear();
    Serial.println(fajar);
    Serial.println(tulu);
    Serial.println(zuhar);
    Serial.println(asar);
    Serial.println(magrib);
    Serial.println(isha);
  }
} */
