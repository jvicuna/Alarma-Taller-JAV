/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  Control another device using Bridge widget!

  Bridge is initialized with the token of any (Blynk-enabled) device.
  After that, use the familiar functions to control it:
    bridge.digitalWrite(8, HIGH)
    bridge.digitalWrite("A0", LOW) // <- target needs to support "Named pins"
    bridge.analogWrite(3, 123)
    bridge.virtualWrite(V1, "hello")
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "c5010c811afe4ababc07d7a720e5f125";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "WifiVH";
char pass[] = "12265165";

// Bridge widget on virtual pin 1
WidgetBridge bridge1(V1);

// Timer for blynking
BlynkTimer timer;

static bool value = true;
 
 
 
void blynkAnotherDevice() // Here we will send HIGH or LOW once per second
{
  // Send value to another device
  if (digitalRead(5)>0) {
    bridge1.digitalWrite(13, HIGH);  // Digital Pin 9 on the second board will be set HIGH
    bridge1.virtualWrite(V13, 1); // Sends 1 value to BLYNK_WRITE(V5) handler on receiving side.

    /////////////////////////////////////////////////////////////////////////////////////////
    // Keep in mind that when performing virtualWrite with Bridge,
    // second board will need to process the incoming command.
    // It can be done by using this handler on the second board:
    //
    //    BLYNK_WRITE(V13){
    //    int pinData = param.asInt(); // pinData variable will store value that came via Bridge
    //    }
    //
    /////////////////////////////////////////////////////////////////////////////////////////
  } else {
    bridge1.digitalWrite(13, LOW); // Digital Pin 9 on the second board will be set LOW
    bridge1.virtualWrite(V13, 0); // Sends 0 value to BLYNK_WRITE(V5) handler on receiving side.
  }
  // Toggle value
  //value = !value;
}

BLYNK_CONNECTED() {
  bridge1.setAuthToken("c706e092dca442b6af6f32ab05bcb765"); // Place the AuthToken of the second hardware here
}



void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Call blynkAnotherDevice every second
  timer.setInterval(1000L, blynkAnotherDevice);
}

void loop()
{
  Blynk.run();
  timer.run();
}
