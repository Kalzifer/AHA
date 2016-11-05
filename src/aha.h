#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "md5.h"



class AHA {
    private:
        String passwd;
        String user;
        const char*  host;
        int port = 443;
        String sid;
        WiFiClientSecure *client;
        String smart_home_url = "/webservices/homeautoswitch.lua";
    public:
        AHA(WiFiClientSecure* wifi_client, String username, String password, const char* hostname);
        AHA(void);
        boolean verify(const char* fingerprint);
        void login(void);
        void logout(void);
        String executeCMD(String cmd);
        String executeCMD(String cmd, String device_id);
        int getSwitchCount(void);
        void getSwitchList(String* switches);
        String getDeviceName(String id);
        void setSwitchOff(String id);
        void setSwitchOn(String id);
        float getSwitchTemperature(String id);
        int getSwitchPower(String id);
        int getSwitchEnergy(String id);
};
