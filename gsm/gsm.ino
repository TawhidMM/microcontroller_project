#include <SoftwareSerial.h>

//Create software serial object to communicate with SIM900
SoftwareSerial mySerial(7, 8); //SIM900 Tx & Rx is connected to Arduino #7 & #8

void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  
  //Begin serial communication with Arduino and SIM900
  mySerial.begin(9600); 

  Serial.println("Initializing..."); 
  delay(1000);

  mySerial.print("AT\r\n"); //Handshaking with SIM900
  updateSerial();
  //delay(500);

  mySerial.print("AT+CMGF=1\r\n"); // Configuring TEXT mode
  updateSerial();
  //delay(500);
  mySerial.print("AT+CMGS=\"+8801856471172\"\r\n");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  //delay(500);
  mySerial.print("GSM testing"); //text content
  updateSerial();
  mySerial.write(26);
}

void loop()
{
}

void updateSerial()
{
  delay(500);
  // while (Serial.available()) 
  // {
  //   mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  // }


  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}