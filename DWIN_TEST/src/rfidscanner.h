//Libraries
#include <SPI.h>//https://www.arduino.cc/en/reference/SPI
#include <MFRC522.h>//https://github.com/miguelbalboa/rfid

//Constants
#define SS_PIN 21
#define RST_PIN 15
String rfidSt;
int count=0;

//Parameters
const int ipaddress[4] = {103, 97, 67, 25};

//Variables
byte nuidPICC[4] = {0, 0, 0, 0};
MFRC522::MIFARE_Key key;
MFRC522 rfid = MFRC522(SS_PIN, RST_PIN);

void setupRfid() {
 
 	Serial.println(F("Initialize System"));
 	//init rfid D8,D5,D6,D7
 	SPI.begin();
 	rfid.PCD_Init();
 	Serial.print(F("Reader :"));
 	rfid.PCD_DumpVersionToSerial();
}


void printDec(byte *buffer, byte bufferSize) {
 	for (byte i = 0; i < bufferSize; i++) {
 			Serial.print(buffer[i] < 0x10 ? "0" : "");
 			Serial.print(buffer[i], DEC);
            rfidSt += buffer[i];
			count++;
 	}
}

void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? "0" : "");
    Serial.print(buffer[i], HEX);
	 rfidSt += buffer[i];
  }
}

void readRFID(void ) { /* function readRFID */
 	////Read RFID card

 	for (byte i = 0; i < 6; i++) {
 			key.keyByte[i] = 0xFF;
 	}
 	// Look for new 1 cards
 	if ( ! rfid.PICC_IsNewCardPresent())
 			return;

 	// Verify if the NUID has been readed
 	if ( 	!rfid.PICC_ReadCardSerial())
 			return;

 	// Store NUID into nuidPICC array
 	for (byte i = 0; i < 4; i++) {
 			nuidPICC[i] = rfid.uid.uidByte[i];
 	}

 	Serial.print(F("RFID In dec: "));
 	printDec(rfid.uid.uidByte, rfid.uid.size);
 	Serial.println();

 	// Halt PICC
 	rfid.PICC_HaltA();

 	// Stop encryption on PCD
 	rfid.PCD_StopCrypto1();

}




