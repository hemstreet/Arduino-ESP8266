#include "config.h"
#include <AltSoftSerial.h>

// Arduino pin 08 for RX
// Arduino Pin 09 for TX

AltSoftSerial altSerial;

// @todo fix this horrible thing, Sorry
char reply[1000]; // Buffer size

void setup() {
      Serial.begin(9600); // Setup debug serial
      altSerial.begin(9600); // Setup ESP

      //Enter the SSID and password for your own network
      altSerial.print("AT+CWJAP=\"" + SSID_NAME + "\",\"" + SSID_PASSWORD + "\"\r\n");
      getReply( 5000 );

      // Connect to server
      Serial.println("Connecting to Server");
      altSerial.print("AT+CIPSTART=\"TCP\",\"" + HOST+ "\"," + PORT+ "\r\n");
      getReply( 2500 );

      // Setup Send
      Serial.println("Setting up send value");
      altSerial.print("AT+CIPSEND=35\r\n");
      getReply( 2500 );

      // Make Request
      Serial.println("Making request");
      altSerial.print("GET /v1/rest/list/game HTTP/1.0\r\n\r\n");
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
        while(altSerial.available())
        {
            char c = altSerial.read();
              reply[index] = c;
              index++;
        }
        reply[index] = 0;
    }

    Serial.println( reply ); // Output response
    Serial.println("\r\n"); // New line for legibility
}
