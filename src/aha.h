/*
MIT License

Copyright (c) 2016 Frank Schmidt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*
    Part of this code is form https://www.open4me.de [1]
    Thanks to Sven and his Block "Svens Gedankensplitter"
    
    The post: "Fritzbox: ESP8266 lässt Telefon klingeln" shows how to can let ring your Phone by 
    using an ESP8266 and a "Fritzbox" Router.
    
    [1] https://www.open4me.de/index.php/2016/10/fritzbox-esp8266-laesst-telefon-klingeln/
*/

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
