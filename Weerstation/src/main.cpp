#include <Wire.h>  // Include Wire if you're using I2C
#include <SPI.h>  // Include SPI if you're using SPI
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library
#include <WEMOS_DHT12.h> // Include the DHT sensor
#include <ESP8266HTTPClient.h> // http client om buienradar api mee aan te roepen
#include <WiFiManager.h>  // Include WifiManager for WifiAccess (Oh, Rly?)

#define PIN_RESET 255  //
#define DC_JUMPER 0  // I2C Addres: 0 - 0x3C, 1 - 0x3D

#define SSID "wemos" // SSID for the WifiManager when no Wifi connected

//////////////////////////////////
// MicroOLED Object Declaration //
//////////////////////////////////
MicroOLED oled(PIN_RESET, DC_JUMPER);  // I2C Example

////////////////////////////////////////////////
// Temperature and Humidity sensor declaraion //
////////////////////////////////////////////////
DHT12 dht12;

void setupWifi() {
  // maak een access point om je wifi netwerk in te kunnen stellen
  WiFiManager wifiManager;
  wifiManager.autoConnect(SSID); 
}

void setupOled() {
  oled.begin();     // Initialize the OLED
  oled.clear(ALL);  // Clear the library's display buffer
  oled.setFontType(0);  // Set font to type 1
  oled.display();   // Display what's in the buffer (splashscreen)
}

void setup() {
  setupWifi();
  setupOled();
}

// // de timestamps van buienradar terugrekenen naar seconden sinds middernacht
// time_t decode(String hrs, String min) {
//   return 3600 * (hrs.toInt()) + 60 * (min.toInt());
// }

String raincheck() {
  bool somerain = false;

  if(WiFi.status()== WL_CONNECTED){ //Check WiFi connection status
  
      HTTPClient http; //Declare an object of class HTTPClient
      http.begin("http://gpsgadget.buienradar.nl/data/raintext/?lat=53.11&lon=6.88"); //Specify request destination
      int httpCode = http.GET(); //Send the request
      if (httpCode == 200 ) { //Check the returning code 
        String payload = http.getString(); //Get the request response payload
        http.end(); //Close connection

        Serial.println(payload); //Print the response payload
        boolean lineFound = false; // bijhouden of er wel data in het bericht zat (soms niet, ondanks de HTTP 200)

        // De eerste 7 regels lezen, dat vertegenwoordigt een half uur
        for (int x=0; x<6; x++) {
          // pluk de tijd uit de dataregel
          // time_t linetime = decode(payload.substring((x*11)+4, (x*11)+6), payload.substring((x*11)+7, (x*11)+9));
          // kijk of de timestamp tussen nu en 30 minuten valt
          // if (linetime > nu && linetime < straks) {
            lineFound = true;
            // Serial.println(payload.substring((x*11)+4, (x*11)+9));
            // als de eerste 3 karakters niet 000 zijn gaat het regenen!
            somerain = somerain || payload.substring(x*11, (x*11)+3) != "000";
          // }
        }
        // hebben we relevante data gekregen?
        if (lineFound) {
          return somerain ? "Yes" : "No rain";
        } else {
          return "EData";
        }        
      } else {
        return "EHTTP";
      }
   }
}


void loop() {
  oled.clear(PAGE);     // Clear the page

  if(dht12.get()==0){
    oled.setCursor(0, 0); // Set cursor to top-left
    oled.print("Weather");
    oled.setCursor(0, 9);
    oled.print("----------");
    oled.setCursor(0, 18);
    oled.print("T: ");
    oled.print(dht12.cTemp);
    oled.print(" C");
    oled.setCursor(0, 27);
    oled.print("H: ");
    oled.print(dht12.humidity);
    oled.print(" %");
    oled.setCursor(0, 36);
    oled.print("R: ");
    oled.print(raincheck());
  //   // Serial.println(dht12.cTemp);
  //   // Serial.print("Temperature in Fahrenheit : ");
  //   // Serial.println(dht12.fTemp);
  //   // Serial.print("Relative Humidity : ");
  //   // Serial.println(dht12.humidity);
  //   // Serial.println();
    oled.display();
  }
  // Wait 5 seconds
  delay(5000);
}