////////////////////////////////////////////////////////////////////////////////////
//
//                               PI and more
//               ESP8266 connecting to multi available wifi AP
//
// https://piandmore.wordpress.com/2016/10/11/esp-and-selecting-wifi-access-point/
//
////////////////////////////////////////////////////////////////////////////////////
//
// You will of course need to include the ESP8266WiFi.h which
// will give you access to the WiFi library of the ESP.
//
#include "ESP8266WiFi.h"
//
// The FS.h will give you access to all of the SPIFFS functions.
//
#include "FS.h"

//
// SERIAL is only defined if you wish to start the serial monitor,
// usually only done for debugging purposes.
// When putting it to production, you can remove this define.
//
#define SERIAL

//
// Setup. This will run only once and will connect you to a
// available AP to which you have the credentials.
//
void setup() {
  //
  // Start serial monitoring if required.
  //
#ifdef SERIAL
  Serial.begin(115200);
  Serial.println("Starting SPIFFS.");
#endif
  //
  // Start the file system.
  //
  bool result = SPIFFS.begin();
#ifdef SERIAL
  Serial.println("SPIFFS opened.");
#endif
  //
  // Set WiFi to station mode and disconnect from an AP if it was
  // previously connected.
  //
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
#ifdef SERIAL
  Serial.println("Wifi scan start.");
#endif
  //
  // WiFi.scanNetworks will return the number of networks found
  //
  int n = WiFi.scanNetworks();
#ifdef SERIAL
  Serial.println("Wifi scan done.");
#endif
  if (n==0) {
#ifdef SERIAL
    Serial.println("No networks found.");
#endif
  } else {
#ifdef SERIAL
    Serial.print(n);
    Serial.println(" networks found.");
#endif
    //
    // Loop through all the found SSIDs
    //
    for (int i=0;i<n;i++) {
      //
      // Check whether there is a file called /wifi/<SSID>.txt
      //
      if (SPIFFS.exists("/wifi/"+WiFi.SSID(i)+".txt")) {
#ifdef SERIAL
        Serial.print("Found ");
        Serial.println(WiFi.SSID(i));
#endif
        //
        // Open the file /wifi/<SSID>.txt to read the password.
        //
        File f = SPIFFS.open("/wifi/"+WiFi.SSID(i)+".txt","r");
        String line = f.readStringUntil('\n');
        char ssid[50];
        WiFi.SSID(i).toCharArray(ssid,50);
        char passw[50];
        line.toCharArray(passw,50);
        //
        // After converting the strings, start the wifi.
        //
        WiFi.begin(ssid,passw);
        //
        // After connecting to the wifi, stop looking.
        //
        break;
      }
    }
  }
}

void loop() {
}
