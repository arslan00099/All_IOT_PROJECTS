#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>
#include <Preferences.h>
WiFiManager wm;
String channelName = "";
String user_id = "";
String user_secret = "";
String access_token;
Preferences preferences;
void filterData(String jsonStr)
{

    DynamicJsonDocument doc(2500);
    DynamicJsonDocument filter(2000);
    filter["data"][0]["game_name"] = true;
    filter["data"][0]["is_live"] = true;
    deserializeJson(doc, jsonStr, DeserializationOption::Filter(filter));

    // Print the result
    serializeJsonPretty(doc, Serial);
    Serial.println("##############################################");
    for (int i = 0; i < 1; i++)
    {
        Serial.println("##############  " + (String)i + "  ###################");
        String gamename = doc["data"][0]["game_name"];

        String livestatus = doc["data"][0]["is_live"];

        Serial.println("GAME NAME : " + (String)gamename);
        Serial.println("LIVESTATUS : " + (String)livestatus);
        if(livestatus.indexOf("true")>-1)
        {
            Serial.println("Channel Is live turn On LED");
            digitalWrite(2,HIGH);
        }
         else
        {
            Serial.println("Channel Is offline turn OFF LED");
             digitalWrite(2,LOW);
        }
    }
}
void getpreferences()
{
    preferences.begin("my-app", false);
    channelName = preferences.getString("channel");
    user_id = preferences.getString("userid");
    user_secret = preferences.getString("usersec");
    access_token = preferences.getString("accesstoken");
    Serial.println("channelName: " + channelName);
    Serial.println("user_id: " + user_id);
    Serial.println("user_secret: " + user_secret);
    Serial.println("access_token: " + access_token);
    preferences.end();
}

void setupWifiManger()
{

    getpreferences();

    WiFiManagerParameter custom_text_box("my_text", "Enter Channel Name", "", 50);
    wm.addParameter(&custom_text_box);
    WiFiManagerParameter custom_text_box1("my_text1", "Enter User ID", "", 50);
    wm.addParameter(&custom_text_box1);
    WiFiManagerParameter custom_text_box2("my_text2", "Enter User Secret", "", 50);
    wm.addParameter(&custom_text_box2);
    if (channelName.length() > 1)
    {
        custom_text_box.setValue(channelName.c_str(), channelName.length());
    }
    if (user_id.length() > 1)
    {
        custom_text_box1.setValue(user_id.c_str(), user_id.length());
    }
    if (user_secret.length() > 1)
    {
        custom_text_box2.setValue(user_secret.c_str(), user_secret.length());
    }

    bool res;
    wm.setTimeout(10000);
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect("TwitchTV", "12345678"); // password protected ap

    if (!res)
    {
        Serial.println("Failed to connect");
        // ESP.restart();
    }
    else
    {
        // if you get here you have connected to the WiFi
        Serial.println("connected...yeey :)");
    }
    Serial.println(custom_text_box.getValue());
    Serial.println(custom_text_box1.getValue());
    Serial.println(custom_text_box2.getValue());
    preferences.begin("my-app", false);
    preferences.putString("channel", custom_text_box.getValue());
    preferences.putString("userid", custom_text_box1.getValue());
    preferences.putString("usersec", custom_text_box2.getValue());
    preferences.end();
}
