#pragma once
#include "wled.h"
#include "leds.h"
#include "wifi.h"
#define WLED_DEBOUNCE_THRESHOLD 50
// #include "../audioreactive/audio_reactive.h"

class Buttons {
private:

    LEDs leds;
    GlowOSWiFi glowoswifi;

    bool brightnessDirection = true; // brightness going up
    long lastduration = 0;
    byte previousPreset = 1;

    bool leftswitch = false;
    bool rightswitch = false;
    int pressCount = 1;
    unsigned long lastButtonPressTime = 0;

    /////////////////////////////////////////////
    //// Setup: Action Button
    /////////////////////////////////////////////
    void setup_action_btn(){
      DEBUG_PRINTLN(F("Setup action button"));
      #ifdef ACTION_BTN_PIN
      btnPin[1] = ACTION_BTN_PIN;
      if (ACTION_BTN_PIN_STATE_IF_PRESSED==1){
        buttonType[1] = BTN_TYPE_PUSH_ACT_HIGH;
        pinMode(ACTION_BTN_PIN, INPUT_PULLDOWN);
      } else {
        buttonType[1] = BTN_TYPE_PUSH;
        pinMode(ACTION_BTN_PIN, INPUT_PULLUP);
      }
      #endif
    }

    /////////////////////////////////////////////
    //// Actions
    /////////////////////////////////////////////
    void actionStartStandby(){
      DEBUG_PRINTLN(F("Start standby mode"));
      #ifdef RLYPIN
      pinMode(RLYPIN, OUTPUT);
      digitalWrite(RLYPIN, LOW);
      #endif
      #ifdef RLYLATCHPIN
      pinMode(RLYLATCHPIN, OUTPUT);
      digitalWrite(RLYLATCHPIN, HIGH);
      digitalWrite(RLYLATCHPIN, LOW);
      #endif
      serializeConfig();
      esp_deep_sleep_start();
      
    }

    void actionNextPreset(){
      DEBUG_PRINTLN(F("Action: Next preset"));

      // test activate AudioReactive processing, by writing config
      bool turnAudioReactiveOn = true;

      byte nextPreset = currentPreset;
      for (;;) {
        nextPreset++;

        String dummy = "";
        if (getPresetName(nextPreset, dummy)) {
          // preset exists
          break;
        }
        
        if (nextPreset == currentPreset || nextPreset > currentPreset + 10) {
          // no more presets
          nextPreset = 1;
          break;
        }
      };

      DEBUG_PRINTLN(F("Next preset..."));
      applyPreset(nextPreset, 0);
    }

    void actionPreviousPreset(){
      DEBUG_PRINTLN(F("Action: Previous preset"));
      byte nextPreset = currentPreset;
      for (;;) {
        nextPreset--;

        String dummy = "";
        if (getPresetName(nextPreset, dummy)) {
          // preset exists
          break;
        }
        
        if (nextPreset == currentPreset || nextPreset < currentPreset - 10) {
          // no more presets
          nextPreset = 1;
          break;
        }
      };

      DEBUG_PRINTLN(F("Previous preset..."));
      applyPreset(nextPreset, 0);
    }

    void actionDefaultPreset(){
      DEBUG_PRINTLN(F("Action: Default preset"));
      applyPreset(1, 0);
    }

    void actionIncreaseBrightness(long dur){
      DEBUG_PRINTLN(F("actionIncreaseBrightness"));
      if ((lastduration != dur) && (dur % 500 == 0)){
          if (bri <= 235){
              bri += 20;
              leds.setBrightness(bri);
          } else if (bri > 235 && bri < 255){
              bri = 255;
              leds.setBrightness(bri);
              DEBUG_PRINTLN(F("maximum brightness reached"));
          }
          DEBUG_PRINTLN(F(bri));
          lastduration = dur;
      }
  }

    void actionDecreaseBrightness(long dur){
      DEBUG_PRINTLN(F("actionDecreaseBrightness"));
      if ((lastduration != dur) && (dur % 500 == 0)){
          if (bri >= 25){
              bri -= 20;
              leds.setBrightness(bri);
          } else if (bri < 25 && bri > 5){
              bri = 5;
              leds.setBrightness(bri);
              DEBUG_PRINTLN(F("minimum brightness reached"));
          }
          DEBUG_PRINTLN(F(bri));
          lastduration = dur;
      }
  }


public:
    void setup(){
        setup_action_btn();
    }

    void checkIfPresetChanged(){
      if (currentPreset!=previousPreset){
        serializeConfig();
        previousPreset = currentPreset;
      }
    }

    void addToConfig(JsonObject& root,  const char* _name){
      JsonObject buttonsConfig = root.createNestedObject(FPSTR(_name)); // usermodname
      buttonsConfig["lastPreset"] = currentPreset;           // usermodparam

      DEBUG_PRINTLN(F("Buttons config saved."));
    }

    bool readFromConfig(JsonObject& root,  const char* _name){
      JsonObject buttonConfig = root[FPSTR(_name)];
      if (buttonConfig.isNull()) 
      {
        DEBUG_PRINTLN(F(": No config found. (Using defaults.)"));
        return false;
      }

      // start last used preset
      // TODO: only take last used preset if ESP32 awake from deep sleep, else take bootPreset
      // currentPreset = buttonConfig["lastPreset"] | previousPreset;
      // bootPreset = currentPreset;

      return true;
    }

    bool handle(uint8_t b) {
      yield();
      if (buttonType[b] == BTN_TYPE_NONE
       || buttonType[b] == BTN_TYPE_RESERVED
       || buttonType[b] == BTN_TYPE_PIR_SENSOR
       || buttonType[b] == BTN_TYPE_ANALOG
       || buttonType[b] == BTN_TYPE_ANALOG_INVERTED) {
        return false;
      }

      unsigned long now = millis();
      bool handled = true;

      if (isButtonPressed(b)) { //pressed
        long dur = now - buttonPressedTime[b];
        lastButtonPressTime = now;

        if (buttonPressedBefore[b] && dur > 600){
          if (pressCount == 1) {
            actionDecreaseBrightness(dur);
          } else if (pressCount == 2) {
            actionIncreaseBrightness(dur);
          }
        }
        if (!buttonPressedBefore[b]) buttonPressedTime[b] = now;
        buttonPressedBefore[b] = true;

      } else if (!isButtonPressed(b) && buttonPressedBefore[b]) { //released
        long dur = now - buttonPressedTime[b];
        if (dur < 50) {
          buttonPressedBefore[b] = false; 
          return false;
        } //too short "press", debounce
        bool doublePress = buttonWaitTime[b]; //did we have a short press before?
        buttonWaitTime[b] = 0;

        if (dur > 600){
          // No action on long press release
        } else if (!buttonLongPressed[b]) { //short press
          if (doublePress) {
            actionNextPreset();
          } else {
            buttonWaitTime[b] = now;
            pressCount += 1;
          }
        }
        
        // reset variables
        buttonPressedBefore[b] = false;
        buttonLongPressed[b] = false;
      }

      //if 350ms elapsed since last short press release it is a single press
      if (buttonWaitTime[b] && now - buttonWaitTime[b] > 350 && !buttonPressedBefore[b]) {
        DEBUG_PRINTLN(F("Turning LEDs on/off"));
        buttonWaitTime[b] = 0;
        toggleOnOff();
        stateUpdated(CALL_MODE_BUTTON);
      }

      // if 350ms elapsed since last button press, reset pressCount
      if (now - lastButtonPressTime > 350) {
        pressCount = 1;
      }

      return handled;
    }

};