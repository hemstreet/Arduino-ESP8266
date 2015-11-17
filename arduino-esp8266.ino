#include "config.h"
#include <AltSoftSerial.h>
#include <aJSON.h>

// D8 for RX, D9 for TX
AltSoftSerial altSerial;

char* parseJson(aJsonObject* root, char* parameter);

// @todo fix this horrible thing, Sorry
char reply[500]; // Buffer size

void setup() {
  
      Serial.begin(9600); // Setup debug serial
      altSerial.begin(9600); // Setup ESP

      Serial.println("Start");
      
      //Enter the SSID and password for your own network

      Serial.println("Connecting to wifi");
      altSerial.print("AT+CWJAP=\"" + SSID_NAME + "\",\"" + SSID_PASSWORD + "\"\r\n");
      getReply( 5000 );

      // Connect to server
      Serial.println("Connecting to Server");
      altSerial.print("AT+CIPSTART=\"TCP\",\"" + HOST+ "\"," + PORT+ "\r\n");
      getReply( 3500 );

      // Setup Send
      Serial.println("Setting up send value");
      altSerial.print("AT+CIPSEND=35\r\n");
      getReply( 2500 );

      // Make Request
      Serial.println("Making request");
      altSerial.print("GET " + BASE_REST + "list/game HTTP/1.0\r\n\r\n");
      getGames( 5000 );
}

void loop() {

}

void getJSONFromResponse(char* responseData) {
   
   char *p, *i;
   Serial.println("\r\n\r\n--------------\r\n\r\n");
   p = strtok_r(responseData, STARTDELIMITER,&i);
   p = strtok_r(i,ENDDELIMITER,&i);
   Serial.println(p);

   Serial.println("\r\n\r\n----JSON--------\r\n\r\n");

//   char* nameOfGame = parseJson(p, "name");   
//   Serial.println(nameOfGame);
   
   char* plays = parseJson(p, "plays");   
   Serial.println(plays);
   
   Serial.println("\r\n\r\n\r\n--------------\r\n\r\n");

   
}

void getReply(int wait)
{
    int index = 0;
    long int time = millis();

    // Sorry
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


// Simplify method so we do no repeat getReply
void getGames(int wait)
{

    int index = 0;
    
    long int time = millis();

    // Sorry
    while( (time + wait) > millis())
    {
        while(altSerial.available())
        {
            char c = altSerial.read();
              reply[index] = c;
              index++;
        }
    }

    getJSONFromResponse(reply);
    
    Serial.println( reply ); // Output response
    Serial.println("\r\n"); // New line for legibility
}

char* parseJson(char* p, char* parameter) 
{
    // @todo Very inefficient :(
    aJsonObject* root = aJson.parse(p);
    
    if (root != NULL) {
        return aJson.getObjectItem(root, parameter)->valuestring; 
    }
    else
    {
      Serial.println("Root was null :(");
    }
}

