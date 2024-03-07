#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>

#define	GATE_OPEN 0xF0
#define	ACCESS_DENIED 0xE0
#define	SEND_ENTER_SMS 0xD0
#define	SEND_LEAVE_SMS 0xC0
#define	RECEIVE_SMS 0xB0

#define SS_PIN 10
#define RST_PIN 9
#define GSM_RX 4
#define GSM_TX 5
#define ATMEGA_RX 2
#define ATMEGA_TX 3
   

#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX
char receivedChar;
const int ledPin = 8; // LED connected to digital pin 8

void setup() {
    pinMode(7, OUTPUT);
    Serial.begin(9600); // Set baud rate to match ATmega32
     // set the data rate for the SoftwareSerial port
    mySerial.begin(9600);
   // Serial.println("nah");
    // while (!mySerial) {
    //   ; // wait for serial port to connect. Needed for Native USB only
    // }
    digitalWrite(7, HIGH); // Turn on LED
}

void loop() {
    if (Serial.available() > 0) { // Check if data is available to read
        receivedChar = Serial.read(); // Read the incoming byte
        //Serial.println(receivedChar);
        delay(10000);
        if (receivedChar == 'Y') {
            digitalWrite(7, LOW); // Turn on LED
        }
    }
    // digitalWrite(ledPin, HIGH);
    digitalWrite(7, HIGH); // Turn on LED
}



















