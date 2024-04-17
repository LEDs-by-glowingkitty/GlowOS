#pragma once
#include "wled.h"
#include "battery_settings.h"
#ifdef BATT_IC_RT9471D
#include "battery_ics.h"
#endif

class Battery {
  private:

    #ifdef BATT_IC_RT9471D
    RT9471D rt9471d;
    #endif

    // DEFINE BATTERY PARAMETERS
    // battery pin can be defined in my_config.h
    #ifdef USERMOD_BATTERY_MEASUREMENT_PIN
    int8_t batteryPin = USERMOD_BATTERY_MEASUREMENT_PIN;
    #endif
    // how often to read the battery voltage
    unsigned long readingInterval = USERMOD_BATTERY_MEASUREMENT_INTERVAL;
    unsigned long nextReadTime = 0;
    unsigned long lastReadTime = 0;
    // battery min. voltage
    float minBatteryVoltage = USERMOD_BATTERY_MIN_VOLTAGE;
    // battery max. voltage
    float maxBatteryVoltage = USERMOD_BATTERY_MAX_VOLTAGE;
    // 0 - 1024 for esp8266 (10-bit resolution)
    // 0 - 4095 for esp32 (Default is 12-bit resolution)
    float adcPrecision = USERMOD_BATTERY_ADC_PRECISION;
    // raw analog reading 
    float rawValue = 0.0;
    // calculated voltage            
    float voltage = 0.0;
    // mapped battery level based on voltage
    long batteryLevel = 0;
    bool initDone = false;
    bool initializing = true;

    // custom map function
    // https://forum.arduino.cc/t/floating-point-using-map-function/348113/2
    double mapf(double x, double in_min, double in_max, double out_min, double out_max){
      return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    float truncate(float val, byte dec){
      float x = val * pow(10, dec);
      float y = round(x);
      float z = x - y;
      if ((int)z == 5)
      {
          y++;
      }
      x = y / pow(10, dec);
      return x;
    }

    bool isCharging(){
      // Read the value of the VBUS pin
      int vbus = digitalRead(25);
      DEBUG_PRINTLN(F("vbus:"));
      DEBUG_PRINTLN(F(vbus));

      // If the value is above a certain threshold, VBUS is connected
      if (vbus == HIGH) {
        return true;
      }
      // If the value is below the threshold, VBUS is not connected
      else {
        return false;
      }
    }

    void showBatteryEmpty(){
      // battery is empty
      Serial.println("Battery is empty. Please charge. Going to sleep now...");

      Serial.println("Battery voltage:");
      Serial.println(voltage);

      Serial.println("Battery percentage:");
      Serial.println(batteryLevel);
      
      // #ifdef RLYPIN
      // pinMode(RLYPIN, OUTPUT);
      // digitalWrite(RLYPIN, HIGH);
      // #endif
      // #ifdef RLYLATCHPIN
      // pinMode(RLYLATCHPIN, OUTPUT);
      // digitalWrite(RLYLATCHPIN, HIGH);
      // digitalWrite(RLYLATCHPIN, LOW);
      // #endif

      // // Create a CRGB array to hold the LED colors
      // CRGB leds[1];

      // FastLED.addLeds<WS2812B, 16, GRB>(leds, 1);

      // // Set the first LED to red at 10% brightness
      // leds[0] = CRGB::Red;
      // leds[0].nscale8(10);

      // // Update the LED strip
      // FastLED.show();

      // // Delay for 2 seconds
      // delay(2000);

      // // Turn off the first LED
      // leds[0] = CRGB::Black;

      // // Update the LED strip
      // FastLED.show();

      #ifdef RLYPIN
      pinMode(RLYPIN, OUTPUT);
      digitalWrite(RLYPIN, LOW);
      #endif
      #ifdef RLYLATCHPIN
      pinMode(RLYLATCHPIN, OUTPUT);
      digitalWrite(RLYLATCHPIN, HIGH);
      digitalWrite(RLYLATCHPIN, LOW);
      #endif

      esp_deep_sleep_start();
    }

    void checkIfBatteryEmpty(){
      // TODO:
      // - define battery status details global, so voltage can be read out everywhere
      // - implement empty battery check every 5 minutes when in deep sleep
      // - clean up code
      // - make compatible with other LED types as well, maybe use WLED functions
      // - update battery charge level -> 3.0v => 0%, 4.2v => 100%
      // - show battery charging level if USB-C connected
      // - show battery charge level if button pressed for 5 seconds

      rawValue = analogRead(USERMOD_BATTERY_MEASUREMENT_PIN);

      if (rawValue!=0.0){
        // calculate the voltage     
        voltage = (rawValue / adcPrecision) * 4.2f;
        // check if voltage is within specified voltage range
        voltage = voltage<2.6f||voltage>4.2f?-1.0f:voltage;

        // translate battery voltage into percentage
        /*
          the standard "map" function doesn't work
          https://www.arduino.cc/reference/en/language/functions/math/map/  notes and warnings at the bottom
        */
        batteryLevel = mapf(voltage, minBatteryVoltage, maxBatteryVoltage, 0, 100);

        DEBUG_PRINTLN(F("Battery voltage:"));
        DEBUG_PRINTLN(F(voltage));

        DEBUG_PRINTLN(F("Battery percentage:"));
        DEBUG_PRINTLN(F(batteryLevel));

        // see if battery is charging
        if (isCharging()){
          // battery is charging
          DEBUG_PRINTLN(F("Battery is charging..."));

          // TODO: show battery charging level via LEDs
          
        }

        else if (voltage<=USERMOD_BATTERY_MIN_VOLTAGE){
          showBatteryEmpty();
        }
      }
    }

  public:

    void setup(){
      #ifdef BATT_IC_RT9471D
      rt9471d.setup();
      #endif

      #ifdef ARDUINO_ARCH_ESP32
        // TODO: also allocate other pins
        DEBUG_PRINTLN(F("Allocating battery pin..."));
        if (batteryPin >= 0 && pinManager.allocatePin(batteryPin, false, PinOwner::UM_GlowOS)) 
        {
          DEBUG_PRINTLN(F("Battery pin allocation succeeded."));
        } else {
          if (batteryPin >= 0) DEBUG_PRINTLN(F("Battery pin allocation failed."));
          batteryPin = -1;  // allocation failed
        }
      #else //ESP8266 boards have only one analog input pin A0

        pinMode(batteryPin, INPUT);
      #endif

      nextReadTime = millis() + readingInterval;
      lastReadTime = millis();

      initDone = true;
      
      pinMode(25, INPUT);
      #ifdef USERMOD_BATTERY_MEASUREMENT_PIN
      checkIfBatteryEmpty();
      #endif
    }

    void checkBatteryLevel(){
      // check the battery level every USERMOD_BATTERY_MEASUREMENT_INTERVAL (ms)
      if (millis() < nextReadTime) return;


      nextReadTime = millis() + readingInterval;
      lastReadTime = millis();
      initializing = false;

      checkIfBatteryEmpty();
      

      // SmartHome stuff
      if (WLED_MQTT_CONNECTED) {
        char subuf[64];
        strcpy(subuf, mqttDeviceTopic);
        strcat_P(subuf, PSTR("/voltage"));
        mqtt->publish(subuf, 0, false, String(voltage).c_str());
      }
    }

    void addToJsonInfo(JsonObject& user){
      // info modal display names
      JsonArray batteryPercentage = user.createNestedArray("Battery level");
      JsonArray batteryVoltage = user.createNestedArray("Battery voltage");

      if (initializing) {
        batteryPercentage.add((nextReadTime - millis()) / 1000);
        batteryPercentage.add(" sec");
        batteryVoltage.add((nextReadTime - millis()) / 1000);
        batteryVoltage.add(" sec");
        return;
      }

      if(batteryLevel < 0) {
        batteryPercentage.add(F("invalid"));
      } else {
        batteryPercentage.add(batteryLevel);
      }
      batteryPercentage.add(F(" %"));

      if(voltage < 0) {
        batteryVoltage.add(F("invalid"));
      } else {
        batteryVoltage.add(truncate(voltage, 2));
      }
      batteryVoltage.add(F(" V"));
    }

    void addToConfig(JsonObject& root,  const char* _name,  const char* _readInterval){
      JsonObject battery = root.createNestedObject(FPSTR(_name)); // usermodname
      #ifdef ARDUINO_ARCH_ESP32
        battery["pin"] = batteryPin;                              // usermodparam
      #endif
      battery["minBatteryVoltage"] = minBatteryVoltage;           // usermodparam
      battery["maxBatteryVoltage"] = maxBatteryVoltage;           // usermodparam
      battery[FPSTR(_readInterval)] = readingInterval;

      DEBUG_PRINTLN(F("Battery config saved."));
    }

    bool readFromConfig(JsonObject& root,  const char* _name, const char* _readInterval){
      #ifdef ARDUINO_ARCH_ESP32
        int8_t newBatteryPin = batteryPin;
      #endif

      JsonObject battery = root[FPSTR(_name)];
      if (battery.isNull()) 
      {
        DEBUG_PRINT(FPSTR(_name));
        DEBUG_PRINTLN(F(": No config found. (Using defaults.)"));
        return false;
      }

      #ifdef ARDUINO_ARCH_ESP32
        newBatteryPin     = battery["pin"] | newBatteryPin;
      #endif
      minBatteryVoltage   = battery["minBatteryVoltage"] | minBatteryVoltage;
      //minBatteryVoltage = min(12.0f, (int)readingInterval);
      maxBatteryVoltage   = battery["maxBatteryVoltage"] | maxBatteryVoltage;
      //maxBatteryVoltage = min(14.4f, max(3.3f,(int)readingInterval));
      readingInterval     = battery["read-interval-ms"] | readingInterval;
      readingInterval     = max(3000, (int)readingInterval); // minimum repetition is >5000ms (5s)

      DEBUG_PRINT(FPSTR(_name));

      #ifdef ARDUINO_ARCH_ESP32
        if (!initDone) 
        {
          // first run: reading from cfg.json
          newBatteryPin = batteryPin;
          DEBUG_PRINTLN(F(" config loaded."));
        } 
        else 
        {
          DEBUG_PRINTLN(F(" config (re)loaded."));

          // changing paramters from settings page
          if (newBatteryPin != batteryPin) 
          {
            // deallocate pin
            pinManager.deallocatePin(batteryPin,PinOwner::UM_GlowOS);
            batteryPin = newBatteryPin;
            // initialise
            setup();
          }
        }
      #endif

      return !battery[FPSTR(_readInterval)].isNull();
    }

};