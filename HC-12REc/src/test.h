#include <SoftwareSerial.h>
String sendtoserver;
SoftwareSerial mySerial(6, 5);

String gsm_send_serial(String command, int delay)
{
  String buff_resp = "";
  Serial.println("Send ->: " + command);
  mySerial.println(command);
  long wtimer = millis();
  while (wtimer + delay > millis())
  {
    if (mySerial.available())
    {
      buff_resp = mySerial.readString();
      Serial.println(buff_resp);
    }
  }
  Serial.println();
  return buff_resp;
}

void postdata()
{
  Serial.println("###### POSTING DATA #######");
  gsm_send_serial("AT+HTTPINIT", 2000);
  gsm_send_serial("AT+HTTPPARA=\"CID\",1", 1000);
  gsm_send_serial("AT+HTTPPARA=\"URL\",\"http://espstack.com/cattle/notify.php\"", 1000);  // write server code here
  gsm_send_serial("AT+HTTPPARA=\"CONTENT\",\"application/json\"", 1000);
  // gsm_send_serial("AT+HTTPSSL=1", 1000);
  // gsm_send_serial("AT+HTTPSSL?", 1000);
  String CSQ = gsm_send_serial("AT+CSQ", 300);
  String SQ = CSQ.substring(CSQ.indexOf(":") + 2, CSQ.indexOf(","));
  Serial.println("Signal Quality" + SQ);
 // sendtoserver = "{\"deviceimei\":\"" + deviceIMEI + "\",\"temp\":\"" + (String)dht1.readTemperature() + "\",\"hum\":\"" + (String)dht1.readHumidity() + "\",\"moist\":\"" + (String)soilmoisturepercent + "\",\"relayone\":\"" + (String)relayA_status + "\",\"relaytwo\":\"" + (String)relayB_status + "\",\"csq\":\"" + SQ + "\"}"; // put your main code here, to run repeatedly:
  sendtoserver = "{\"deviceimei\":\"I am NANO\"}";
  gsm_send_serial(" AT + HTTPDATA = " + String(sendtoserver.length()) + ", 100000 ", 1000);
  gsm_send_serial(sendtoserver, 1000);
  gsm_send_serial("AT+HTTPACTION=1", 1000);
  gsm_send_serial("AT+HTTPREAD", 3000);
}

void setup()
{
  Serial.begin(9600);

  mySerial.begin(9600);

  Serial.println("Initializing...");
  delay(1000);
}

void loop()
{
  postdata();
  Serial.println("POSTED");
 // gsm_send_serial("AT+CSCLK=2", 1000); // entering deep sleep
  delay(50000);
  // gsm_send_serial("AT", 1000); // dummy AT command
  // gsm_send_serial("AT+CSCLK=0", 1000); // wake-up 

}