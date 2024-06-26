#pragma once
#include "wled.h"
#include "leds.h"
#include "colorcode.h"


class GlowOSWiFi {
  private:
    LEDs leds;
    ColorCode colorcode;
    String defaultwifimode;

    bool justbooted = true;
    bool wifi_name_customized;
    bool wifi_on;

  public:

    void turnOffOnboot(){
      #ifdef WIFI_OFF_ON_BOOT
      if (justbooted && WiFi.getMode() != WIFI_OFF){
        WiFi.mode(WIFI_OFF);
        DEBUG_PRINTLN(F("WiFi turned off to save power."));
        Serial.println("WiFi turned off to save power.");
        justbooted = false;
      }
      #endif
    }

    void toggleWifi(){
      DEBUG_PRINTLN(F("Toggle WiFi"));
      Serial.println("Toggle WiFi");
      if(WiFi.getMode() == WIFI_OFF){
        Serial.println("Turn wifi on");
        DEBUG_PRINTLN(F("Turn wifi on"));
        // TODO: show LED animation for "Wifi on"

        // turn wifi on and start hotspot
        WiFi.mode(WIFI_AP);
        // TODO: set the SSID and password for the hotspot
        // WiFi.softAP(apSSID, apPass);

      } else {
        Serial.println("Turn wifi off");
        DEBUG_PRINTLN(F("Turn wifi off"));
        // TODO: show LED animation for "Wifi off"
        
        // turn wifi off
        WiFi.mode(WIFI_OFF);
      }
    }

    void resetSettings(){
        // TODO
        DEBUG_PRINTLN(F("Reset WiFi settings"));
        DEBUG_PRINTLN(F("To confirm, press Power Button."));
        DEBUG_PRINTLN(F("To cancel, press Action Button or wait 5 seconds."));

        DEBUG_PRINTLN(F("WiFi settings reset to defaults."));
        DEBUG_PRINTLN(F("WiFi name:"));
        DEBUG_PRINTLN(F("WiFi password:"));
        leds.glowColorCode();

        DEBUG_PRINTLN(F("Reset WiFI settings cancelled. Action Button was pressed."));

        DEBUG_PRINTLN(F("Reset WiFI settings cancelled. 5 seconds time out."));
    }

    void check_if_wifi_ssid_changed(){
      if (forceReconnect==true){
        DEBUG_PRINTLN(F("WiFi hotspot name was changed. Saving to config."));
        wifi_name_customized = true;
        serializeConfig();
      }
    }

    void setup(String hotspot_name = WLED_AP_SSID){
      // check if user has set a custom wifi name before (check usermod json settings)
      if (wifi_name_customized == false){
      // if not, get colorcode via colorcode.h and and use it to setup wifi name
        String colorcode_string = colorcode.getString();
        hotspot_name += String(" ") + colorcode_string;
        strcpy_P(apSSID, PSTR(hotspot_name.c_str()));
      }
      // TODO: then, start custom effect with colorcode on status LED
    }

    void addToConfig(JsonObject& root,  const char* _name){
      JsonObject wifiConfig = root.createNestedObject(FPSTR(_name));
      wifiConfig["wifi_name_customized"] = wifi_name_customized;
      wifiConfig["wifi_on"] = wifi_on;

      Serial.println("Saved WiFi on: " + String(wifi_on) + " to config.");

      DEBUG_PRINTLN(F("WiFi config saved."));
    }

    bool readFromConfig(JsonObject& root,  const char* _name){
      JsonObject wifiConfig = root[FPSTR(_name)];
      if (wifiConfig.isNull()) 
      {
        DEBUG_PRINTLN(F(": No config found. (Using defaults.)"));
        return false;
      }

      wifi_name_customized = wifiConfig["wifi_name_customized"] | false;
      wifi_on = wifiConfig["wifi_on"] | true;

      Serial.println("Loaded WiFi on: " + String(wifi_on));

      return true;
    }

};
