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
   


#define RX 6    // Connect this to the TX pin of the other device
#define TX 7    // Connect this to the RX pin of the other device

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.
SoftwareSerial atmegaSerial(RX, TX);


SoftwareSerial gsmSerial(GSM_RX, GSM_TX); 
//SoftwareSerial atmegaSerial(ATMEGA_RX, ATMEGA_TX);
//MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

void setup() {
    Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
    gsmSerial.begin(9600); // Setting the baud rate of GSM Module
    atmegaSerial.begin(9600);

    SPI.begin();         // Initiate  SPI bus
    mfrc522.PCD_Init();  // Initiate MFRC522

    delay(1000);
    Serial.println("Preparing to read received SMSs");
}


void loop() {
    // ReadUnreadMessages();
    // delay(1000);
    read_RFID();
    read_atmega();
}

void read_atmega() {
    //atmegaSerial.listen();
    if(atmegaSerial.available() > 0) {
        
        Serial.println("receiving command......");
        
        uint8_t received = atmegaSerial.read();
        Serial.print("atmega command : ");
        Serial.println(received);
        delay(500);

        // SMS receive command
        if(received==RECEIVE_SMS) {
            // get the ammmount
            if(atmegaSerial.available() > 0) {
                uint8_t to_be_paid = atmegaSerial.read();
                Serial.print("amount : ");
                Serial.println(to_be_paid);
            }
        }
    }
    
}



void ReadUnreadMessages() {
    String receivedString = "";
    
    //Serial.println("Setting the GSM in text mode");

    gsmSerial.println("AT+CMGF=1\r");
    while (gsmSerial.available() > 0) {
        //Serial.write(gsmSerial.read());
        gsmSerial.read();
    }
    delay(2000);

    //Serial.println("Fetching list of received unread SMS!");
    gsmSerial.println("AT+CMGL=\"REC UNREAD\"\r");
    while (gsmSerial.available() > 0) {
        //Serial.write(gsmSerial.read());
        gsmSerial.read();
    }

    delay(2000);
    // Print the response on the Serial Monitor
    while (gsmSerial.available() > 0) {
        char a= gsmSerial.read();
        //Serial.write(gsmSerial.read());
        receivedString += a;
    }
    Serial.println(receivedString);
    //Serial.println(receivedString.length());
    unsigned int a=0;
    String tk="";
    for(;a<receivedString.length();a++){
        if(receivedString[a]=='T'){
        a++;
        if(receivedString[a]=='k'){
            a++;
            while(receivedString[a]!='f'){
            tk+=receivedString[a];
            a++;
            }
            break;
        }
        }
    }

    Serial.println(tk);
}


void read_RFID() {
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
     
        atmegaSerial.write(GATE_OPEN);
        Serial.print("Sent byte: ");
        Serial.println(GATE_OPEN);

        delay(1000);
    } 
    else {
        Serial.println("Access denied");
        Serial.println();

        atmegaSerial.write((uint8_t)0);
        Serial.print("Sent byte: ");
        Serial.println(0);

        delay(1000);
    }
}


















