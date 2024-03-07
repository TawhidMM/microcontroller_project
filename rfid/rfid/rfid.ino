#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>

#define SS_PIN 10
#define RST_PIN 9
#define RX 2    // Connect this to the TX pin of the other device
#define TX 3    // Connect this to the RX pin of the other device

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.
SoftwareSerial ATmegaSerial(RX, TX);

void setup() {
    pinMode(6, OUTPUT);
    digitalWrite(6, LOW);

    Serial.begin(9600);  // Initiate a serial communication
    ATmegaSerial.begin(9600);

    SPI.begin();         // Initiate  SPI bus
    mfrc522.PCD_Init();  // Initiate MFRC522

    Serial.println("Approximate your card to the reader...");
    Serial.println();
}
void loop() {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "D3 B9 4D 25")  //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
  
    digitalWrite(6, 1);
    
    ATmegaSerial.write(15);
    Serial.print("Sent byte: ");
    Serial.println(15);


    delay(1000);
    // digitalWrite(6,0);
    // delay(200);
    // digitalWrite(6,1);
    // delay(1000);
    // digitalWrite(6,0);
    // delay(200);
    // digitalWrite(6,1);
    // delay(200);
    //digitalWrite(6, 0);
    //delay(1000);
  } else {
    Serial.println(" Access denied");
    Serial.println();

    ATmegaSerial.write((uint8_t)0);
    Serial.print("Sent byte: ");
    Serial.println(0);
    // //delay(3000);
    // digitalWrite(7, 1);
    // delay(200);
    // digitalWrite(7, 0);
    // delay(200);
    // digitalWrite(7, 1);
    // delay(200);
    // digitalWrite(7, 0);
    // delay(200);
    // digitalWrite(7, 1);
    // delay(200);
    // digitalWrite(7, 0);
    delay(1000);
  }
}
