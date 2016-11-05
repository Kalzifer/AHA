#include "aha.h"

int count_of_char(String s, char c)
{
  int count = 0;
  for(int i = 0; i < s.length(); i++ )
  {
    if(s.charAt(i) == c)
    {
      count++;
    }
  }
  return count;
}

void split_string(String target, char seperator, String * parts)
{
  int count = count_of_char(target,seperator);

  if (count == 0)
  {
    parts[1] = target;
    return;
  }

  for(int i = 0; i<count;i++)
  {
    int end = target.indexOf(seperator);
    parts[i] = target.substring(0,end);
    target.remove(0,end+1);
  }
  return;
}

String requestURL(String url,WiFiClientSecure* client, const char* host)
{
  if (!client->connect(host, 443)) {
  Serial.println("connection failed");
  return "";
}

  client->print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");
  while (client->connected()) {
    String line = client->readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }
  String line = client->readStringUntil('\n');
  return line;
}

AHA::AHA(WiFiClientSecure *wifi_client, String username, String password, const char* hostname) {
    passwd = password;
    user = username;
    host = hostname;
    client = wifi_client;
}
AHA::AHA(void){
    host = "fritz.box";
}

boolean AHA::verify(const char* fingerprint) {
    return this->client->verify(fingerprint, host);
}

void AHA::login(void ) {
  String start_tag = String("<Challenge>");
  String stop_tag = String("</Challenge>");

  String result = requestURL("/login_sid.lua", client, host);
  int begin = result.indexOf(start_tag);
  int end = result.indexOf(stop_tag);
  String challenge = result.substring(begin+11,end);
  String reponseASCII =  challenge + String("-") + passwd;
  String responseHEX = "";

  for (unsigned int i = 0; i  < reponseASCII.length(); i++) {
    responseHEX = responseHEX + String(reponseASCII.charAt(i), HEX) + "00";
  }

  MD5Builder md5;
  md5.begin();
  md5.addHexString(responseHEX);
  md5.calculate();

  String response = challenge + "-" + md5.toString();
  result = requestURL("/login_sid.lua?username=" + user + "&response=" + response, client, host);
  sid = result.substring(result.indexOf("<SID>") + 5,  result.indexOf("<SID>") + 21);
}

void AHA::logout(void) {
  requestURL("/login_sid.lua?logout=1&sid=" + sid, client, host);
}

String AHA::executeCMD(String cmd)
{
  String url = smart_home_url + "?sid=" + sid + "&switchcmd=" + cmd;
  return requestURL(url, client, host);
}

String AHA::executeCMD(String cmd, String device_id)
{
  String url = smart_home_url + "?sid=" + sid + "&switchcmd=" + cmd + "&ain=" + device_id;
  return requestURL(url, client, host);
}

int AHA::getSwitchCount(void) {
  String result = executeCMD(String("getswitchlist"));
  int count = count_of_char(result, ',');
  if (count == 0 && result.length() > 0) {
    count = 1;
  }
  return count;
}
void AHA::getSwitchList(String* switches) {
  int count = getSwitchCount();

  String result = executeCMD(String("getswitchlist"));

  if(count > 1) {
    split_string(result, ',', switches);
  }
  else if (count == 1) {
    switches[0] = result;
  }
}
String AHA::getDeviceName(String id) {
  return executeCMD("getswitchname", id);
}

void AHA::setSwitchOff(String id) {
  executeCMD("setswitchoff", id);
}
void AHA::setSwitchOn(String id) {
  String result = executeCMD("setswitchon", id);
}
float AHA::getSwitchTemperature(String id) {
  String result = executeCMD("gettemperature", id);
  return result.toFloat() / 10.0;
}
int AHA::getSwitchEnergy(String id) {
  String result = executeCMD("getswitchenergy", id);
  return result.toInt();
}
int AHA::getSwitchPower(String id) {
  String result = executeCMD("getswitchpower", id);
  return result.toInt();
}
