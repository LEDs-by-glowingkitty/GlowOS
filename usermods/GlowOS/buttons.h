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

    void setup_power_btn(){
      DEBUG_PRINTLN(F("Setup power button"));

      #ifdef POWER_BTN_PIN
      btnPin[0] = POWER_BTN_PIN;
      if (POWER_BTN_PIN_STATE_IF_PRESSED==1){
        buttonType[0] = BTN_TYPE_PUSH_ACT_HIGH;
        pinMode(POWER_BTN_PIN, INPUT_PULLDOWN);
        esp_sleep_enable_ext0_wakeup(static_cast<gpio_num_t>(POWER_BTN_PIN),1);
      } else {
        buttonType[0] = BTN_TYPE_PUSH;
        pinMode(POWER_BTN_PIN, INPUT_PULLUP);
        esp_sleep_enable_ext0_wakeup(static_cast<gpio_num_t>(POWER_BTN_PIN),0);
      }
      #endif
    }

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

    void actionStartPowerSavingMode(){
      DEBUG_PRINTLN(F("Start power saving mode"));
      serializeConfig();
      esp_deep_sleep_start();
    }

    void actionResetAll(){
      // TODO
      DEBUG_PRINTLN(F("Reset to factory settings"));
      DEBUG_PRINTLN(F("To confirm, press Power Button."));
      DEBUG_PRINTLN(F("To cancel, press Action Button or wait 5 seconds."));
    }

    // void writeAudioReactiveStateOn(JsonObject& root)
    // {
    //   if (!initDone) return;  // prevent crash on boot applyPreset()
    //   JsonObject usermod = root[FPSTR("AudioReactive")];
    //   if (usermod.isNull()) {
    //     usermod = root.createNestedObject(FPSTR(_name));
    //   }
    //   usermod["on"] = true;
    // }

    void actionNextPreset(){
      DEBUG_PRINTLN(F("Action: Next preset"));

      // test activate AudioReactive processing, by writing config
      bool turnAudioReactiveOn = true;
      
      // if (turnAudioReactiveOn){
      //   serializeState(); // this calls the 
      // }

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

    void actionDefaultPreset(){
      DEBUG_PRINTLN(F("Action: Default preset"));
      applyPreset(1, 0);
    }

    void actionChangeBrightness(long dur){
      DEBUG_PRINTLN(F("actionChangeBrightness"));
      if ((lastduration != dur) && (dur % 500 == 0)){
        if (bri<20){
          brightnessDirection = false;
        }
        else if (bri>235){
          brightnessDirection = true;
        }
        if (brightnessDirection){
          leds.setBrightness(bri-20);
        } else {
          leds.setBrightness(bri+20);
        }
        DEBUG_PRINTLN(bri);
        lastduration = dur;
      }
    }

    void actionShowBatteryLevel(){
      // TODO
      DEBUG_PRINTLN(F("actionShowBatteryLevel"));
    }

    void actionPress(uint8_t b){
      DEBUG_PRINTLN(F("actionPress"));
      #ifdef ACTION_BTN_PIN
      if (btnPin[b]==ACTION_BTN_PIN){
        actionNextPreset();
      }
      #endif
      #ifdef POWER_BTN_PIN
      if (btnPin[b]==POWER_BTN_PIN){
        actionStartStandby();
      }
      #endif
    }

    void actionPressPress(uint8_t b){
      DEBUG_PRINTLN(F("actionPressPress"));
      #ifdef ACTION_BTN_PIN
      if (btnPin[b]==ACTION_BTN_PIN){
        actionDefaultPreset();
      }
      #endif
      #ifdef POWER_BTN_PIN
      if (btnPin[b]==POWER_BTN_PIN){
        Serial.println(F("Power button double pressed"));
        glowoswifi.turnOnOff();
      }
      #endif
    }

    void actionPressHold(uint8_t b, long dur){
      DEBUG_PRINTLN(F("actionPressHold"));
      #ifdef POWER_BTN_PIN
      if (btnPin[b]==POWER_BTN_PIN){
        if (dur>=10000){
          // TODO
          actionResetAll();
        }
        else if (dur>=6000){
          // TODO
          glowoswifi.resetSettings();
        } else if (dur>=3000){
          // TODO
          actionShowBatteryLevel();
        } else {
          #ifdef RLYLATCHPIN
          actionStartPowerSavingMode();
          #endif
        }
      }
      #endif
    }

public:
    void setup(){
        setup_power_btn();
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

      // TODO: maybe needs pull up resistor? check voltage on pin if switch not triggered
      if (buttonType[b] == BTN_TYPE_SWITCH) {

        if (buttonPressedBefore[b] != isButtonPressed(b)) {
          buttonPressedTime[b] = now;
          buttonPressedBefore[b] = !buttonPressedBefore[b];
          
          if (isButtonPressed(b)) {
            if (b==2){
              leftswitch = true;
            } else if (b==3){
              rightswitch = true;
            }
          } else {
            if (b==2){
              leftswitch = false;
            } else if (b==3){
              rightswitch = false;
            }
          }

          if (leftswitch && rightswitch){
            DEBUG_PRINTLN(F("Start glowing"));
            applyPreset(1);
          }

          else if (!leftswitch && !rightswitch){
            DEBUG_PRINTLN(F("Start safe driving"));
            applyPreset(2);
          }

          else if (leftswitch && !rightswitch){
            DEBUG_PRINTLN(F("Turn left"));
            applyPreset(3);
          }
          else if (!leftswitch && rightswitch){
            DEBUG_PRINTLN(F("Turn right"));
            applyPreset(4);
          }
        }

        if (buttonLongPressed[b] == buttonPressedBefore[b]){
          return handled;
        }

        return handled;

      }

      else {

        // handle push button
        if (isButtonPressed(b)) { //pressed
          // while action button is pressed, change brightness
          #ifdef ACTION_BTN_PIN
          if (btnPin[b]==ACTION_BTN_PIN){
            long dur = now - buttonPressedTime[b];
            if (buttonPressedBefore[b] && dur > 600){
              actionChangeBrightness(dur);
            }
          }
          #endif
          if (!buttonPressedBefore[b]) buttonPressedTime[b] = now;
          buttonPressedBefore[b] = true;

        } else if (!isButtonPressed(b) && buttonPressedBefore[b]) { //released
          long dur = now - buttonPressedTime[b];
          brightnessDirection = true;
          if (dur < 50) {
            buttonPressedBefore[b] = false; 
            return false;
          } //too short "press", debounce
          bool doublePress = buttonWaitTime[b]; //did we have a short press before?
          buttonWaitTime[b] = 0;

          if (dur > 600){
            actionPressHold(b,dur);

          } else if (!buttonLongPressed[b]) { //short press
            //NOTE: this interferes with double click handling in usermods so usermod needs to implement full button handling
            if (doublePress) {
              actionPressPress(b);
            } else {
              buttonWaitTime[b] = now;
            }
          }
          
          // reset variables
          buttonPressedBefore[b] = false;
          buttonLongPressed[b] = false;
        }

        //if 350ms elapsed since last short press release it is a short press
        if (buttonWaitTime[b] && now - buttonWaitTime[b] > 350 && !buttonPressedBefore[b]) {
          buttonWaitTime[b] = 0;
          actionPress(b);
        }
      }


      return handled;
    }

};