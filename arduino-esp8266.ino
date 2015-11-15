#include <AltSoftSerial.h>

// Arduino pin 08 for RX
// Arduino Pin 09 for TX

AltSoftSerial espSerial;

const bool printReply = true;

// @todo fix this horrible thing, Sorry
char reply[2500]; // Buffer size

void setup() {
      Serial.begin(9600); // Setu debug serial
      espSerial.begin(9600); // Setup ESP

      //Enter the SSID and password for your own network
      espSerial.print("AT+CWJAP=\"SSID\",\"SSID_PASSWORD\"\r\n");
      getReply( 5000 );

      // Connect to server
      Serial.println("Connecting to Server");
      espSerial.print("AT+CIPSTART=\"TCP\",\"SERVER_IP\",SERVER_PORT\r\n");
      getReply( 2500 );

      // Setup Send
      Serial.println("Settig up send value");
      espSerial.print("AT+CIPSEND=35\r\n");
      getReply( 2500 );

      // Make Request
      Serial.println("Making request");
      espSerial.print("GET /v1/rest/list/game HTTP/1.0\r\n\r\n");
      getReply( 5000 );
}

void loop() {

}

void getReply(int wait)
{
    // Sorry
    int index = 0;
    long int time = millis();
    while( (time + wait) > millis())
    {
        while(espSerial.available())
        {
            char c = espSerial.read();
              reply[index] = c;
              index++;
        }
        reply[index] = 0;
    }

    Serial.println( reply ); // Output reponse
    Serial.println("\r\n"); // New line for legibility
}
