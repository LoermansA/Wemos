#include <ESP8266WiFi.h>
#include <ESPAsyncUDP.h>
#include <MLEDScroll.h>
#include <WiFiManager.h>

#define SSID "wemosGears"

MLEDScroll matrix;

AsyncUDP udp;

struct UDPPacket
{
    float time; //Time
    float laptime; // Time of Current Lap
    float lapdistance; // Distance Driven on Current Lap
    float totaldistance; // Distance Driven Overall
    float x; // Position X
    float y; // Position Y
    float z; // Position Z
    float speed; // Velocity (Speed) [m/s]
    float vx; // Velocity X
    float vy; // Velocity Y
    float vz; // Velocity Z
    float rx; // Roll Vector X
    float ry; // Roll Vector Y
    float rz; // Roll Vector Z
    float px; // Pitch Vector X
    float py; // Pitch Vector Y
    float pz; // Pitch Vector Z
    float srl; // Position of Suspension Rear Left
    float srr;// Position of Suspension Rear Right
    float sfl; // Position of Suspension Front Left
    float sfr; // Position of Suspension Front Right
    float vsrl; // Velocity of Suspension Rear Left
    float csrr; // Velocity of Suspension Rear Right
    float vsfl; // Velocity of Suspension Front Left
    float vsfr; // Velocity of Suspension Front Right
    float vwrl; // Velocity of Wheel Rear Left
    float vwrr; // Velocity of Wheel Rear Right
    float vwfl; // Velocity of Wheel Front Left
    float vwfr; // Velocity of Wheel Front Right
    float throttle; // Position Throttle
    float steer; // Position Steer
    float brake; // Position Brake
    float clutch; // Position Clutch
    float gear; // Gear [0 = Neutral, 1 = 1, 2 = 2, ..., 10 = Reverse]
    float glat; // G-Force Lateral
    float glong; // G-Force Longitudinal
    float currentlap; // Current Lap
    float rpmdiv10; // Speed of Engine [rpm / 10]
    float h1;// ?
    float h2;// ?
    float h3;// ?
    float h4;// ?
    float h5;// ?
    float h6;// ?
    float h7;// ?
    float h8;// ?
    float h9;// ?
    float h10;// ?
    float h11;// ?
    float h12;// ?
    float h13;// ?
    float tbrl;// Temperature Brake Rear Left ?
    float tbrr;// Temperature Brake Rear Right ?
    float tbfl;// Temperature Brake Front Left ?
    float tbfr;// Temperature Brake Front Right ?
    float h14;// ?
    float h15;// ?
    float h16;// ?
    float h17;// ?
    float h18;// ?
    float numlaps; // Number of Laps in Total ?
    float tracklength;// Length of Track in Total
    float h19;// ?
    float maxrpmdiv10;// Maximum rpm / 10
};

UDPPacket telemetryPacket;

void updateLeds(float currentGear) {
    if (currentGear == 0) {
        matrix.character("R");
    } else if (currentGear == 1) {
        matrix.character("N");
    } else {
        char chrGear[1];
        dtostrf(currentGear - 1.0, 1, 0, chrGear);
        matrix.character(chrGear); 
    }
}

void setupWifi() {
  // maak een access point om je wifi netwerk in te kunnen stellen
  WiFiManager wifiManager;
  wifiManager.autoConnect(SSID); 
}

void setup()
{
    // First setup serial for debugging
    Serial.begin(115200);
    // Initialize the LED matrix
    Serial.println("Init matrix");
    matrix.begin();
    matrix.character("X");
    // serial en wifi init
    Serial.println("Matrix init done, starting Wifi");
    setupWifi();
    Serial.println("Wifi connected, start udp handler");
    if(udp.listen(20777)) {
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());
        udp.onPacket([](AsyncUDPPacket packet) {
            memcpy(&telemetryPacket, packet.data(), sizeof(telemetryPacket));

            // Update the gear indicator
            updateLeds(telemetryPacket.gear);
        });
    }
}

void loop() {
}