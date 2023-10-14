#pragma once // include guard to prevent multiple inclusion of this file
#include "wled.h" // include the main WLED header
#include "colorcode.h" // include the color code header

class LEDs {
private:
    BusDigital *ledBus; // pointer to a digital LED bus object
    byte ledBrightness; // variable to store the LED brightness
    bool justbooted = true; // flag to indicate if the device was just booted

    // TODO: find a different way to do this, without adding status LED multiple times.
    // void setupStatusLED(){ // function to set up the status LED
    //   #ifdef STATUS_LED_DATA // check if STATUS_LED_DATA is defined
    //   #ifdef STATUS_LED_CLOCK // check if STATUS_LED_CLOCK is defined
    //   byte _pins[5] = {STATUS_LED_DATA, STATUS_LED_CLOCK, 255, 255, 255}; // initialize an array with the data and clock pins
    //   BusConfig busCfg = BusConfig(TYPE_APA102, _pins, 0, 1, COL_ORDER_GRB, false, 0); // create a BusConfig object with the specified parameters
    //   busses.add(busCfg); // add the BusConfig object to the list of busses
    //   #endif
    //   #endif
    // }

    void setupPowerMOSFETS(){ // function to set up the power MOSFETs
      #ifdef RLYLATCHPIN // check if RLYLATCHPIN is defined
      if (RLYLATCHPIN>=0) { // check if RLYLATCHPIN is greater than or equal to zero
        pinMode(RLYLATCHPIN, OUTPUT); // set the mode of RLYLATCHPIN to output
        digitalWrite(RLYLATCHPIN, HIGH); // set the value of RLYLATCHPIN to high
      }
      #endif

      // Check if rlyPin is greater than or equal to zero
      if (rlyPin>=0){
        // If true, set the digital output of rlyPin based on the value of rlyMde and bri
        digitalWrite(rlyPin, (rlyMde ? bri : !bri));
      }
      

      #ifdef RLYLATCHPIN // check if RLYLATCHPIN is defined
      if (RLYLATCHPIN>=0) { // check if RLYLATCHPIN is greater than or equal to zero
        pinMode(RLYLATCHPIN, OUTPUT); // set the mode of RLYLATCHPIN to output
        digitalWrite(RLYLATCHPIN, LOW); // set the value of RLYLATCHPIN to low
      }
      #endif
    }

public:

    void addToConfig(JsonObject& root,  const char* _name){ // function to add LED configuration to JSON object
      JsonObject ledsConfig = root.createNestedObject(FPSTR(_name)); // create a nested JSON object with the specified name
      ledsConfig["lastbri"] = bri; // set the last brightness parameter in the JSON object to the current brightness
      DEBUG_PRINTLN(F("addToConfig brightness"));
      DEBUG_PRINTLN(F(bri));

      DEBUG_PRINTLN(F("Saved brightness to json."));
    }

    bool readFromConfig(JsonObject& root,  const char* _name){ // function to read LED configuration from JSON object
      JsonObject ledsConfig = root[FPSTR(_name)]; // get the nested JSON object with the specified name
      if (ledsConfig.isNull()) // check if the object is null
      {
        DEBUG_PRINTLN(F(": No config found. (Using defaults.)"));
        return false;
      }
      ledBrightness = ledsConfig["lastbri"] | ledBrightness; // set the brightness to the last brightness parameter in the JSON object
      return true;
    }

    void glowColorCode(){ // function to start the color code animation
      DEBUG_PRINTLN(F("Starting color code animation."));
    }

    void setBrightness(int value){ // function to set the LED brightness
      if (value == 0 && bri > 0) briLast = bri; // if the new brightness value is zero and the current brightness is greater than zero, set the last brightness to the current brightness
      bri = value; // set the brightness to the new value
      stateUpdated(CALL_MODE_DIRECT_CHANGE); // update the state with the direct change mode
    }

    void loadLastbrightness(){ // function to load the last brightness value
      if (justbooted){
        setBrightness(ledBrightness); // set the brightness to the last brightness value
        justbooted = false; // set the just booted flag to false
      }
    }

    void setup(){ // function to set up the LEDs
      setupPowerMOSFETS(); // call the setupPowerMOSFETS function
    }

    void setup_status_led(){ // function to setup the status LED. Checks first if Status LED is already setup
    }
};
