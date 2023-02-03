#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>

const char *ssid = "channelorange";
const char *password = "champagne";

WiFiUDP Udp;
unsigned int multicastPort = 5007;
IPAddress multicastIp(224, 3, 29, 71);
char incomingPacket[256];

void setup() {
 pinMode(LED_BUILTIN, OUTPUT);
 
 Serial.begin(115200);
// Serial.println();
// Serial.print("Configuring access point...");
 
 WiFi.begin(ssid, password);
 
// while (WiFi.status() != WL_CONNECTED) {
//   delay(500);
//   Serial.print(".");
// }
 
// Serial.println("");
// Serial.println("WiFi connected");
// Serial.println("IP address: ");
// Serial.println(WiFi.localIP());
//
// Serial.println("");
// Serial.println("Starting UDP");

 Udp.begin(multicastPort);
 Udp.beginMulticast(WiFi.localIP(), multicastIp, multicastPort);
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    digitalWrite(LED_BUILTIN, LOW);
    int len = Udp.read(incomingPacket, 255);
    if (len > 0) {
      for (int i = 0; i < len; i++) {
        Serial.write((char)incomingPacket[i]);
        //Serial.print((int)incomingPacket[i]);
        //Serial.print(" ");
      }
        //Serial.println();
    }
    digitalWrite(LED_BUILTIN, HIGH);
  }
} 
