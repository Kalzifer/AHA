#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <aha.h>

const char* ssid = "***WiFI SSID***";
const char* wifi_password = "***WiFi PASSWORD***";
const char* fingerprint = "00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";
WiFiClientSecure client;
const char* host = "fritz.box";
AHA aha;
String fritzbox_user = "user";
String fritzbox_password = "password";

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, wifi_password);
    WiFi.mode(WIFI_STA);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        Serial.println(WiFi.localIP());
    }
    client.connect(host, 443);
    delay(2000);
    aha = AHA(&client, fritzbox_user, fritzbox_password, host);
    if(aha.verify(fingerprint)) {
        Serial.print("good");
    }
    else {
        Serial.print("bad");
    }
    aha.login();
    int count = aha.getSwitchCount();
    String switches[count];
    aha.getSwitchList(switches);

    for(int i=0; i<count;i++) {
      Serial.println(switches[i]);
    }

    Serial.println("init done");
}

void loop() {
  int count = aha.getSwitchCount();
  String switches[count];
  aha.getSwitchList(switches);
  for(int i=0; i<count;i++) {
    float temperature = aha.getSwitchTemperature(switches[i]);
    Serial.print("Temperature: ");
    Serial.print(temperature);

    int power = aha.getSwitchPower(switches[i]);
    Serial.print(" Wh: ");
    Serial.println(power);
  }
  delay(5000);
}
