#pragma once

#include "wled.h"
#include "colorcode.h"
#include "leds.h"
#include "wifi.h"
#include "buttons.h"
#ifdef BATT_IC
#include "battery.h"
#endif
#ifdef USB_PD_VOLTS_DETECTION
#ifdef USB_PD_AMPS_DETECTION
#include "usbpd_ics.h"
#endif
#endif


class UsermodGlowOS : public Usermod 
{
  private:
    Buttons buttons;
    GlowOSWiFi glowoswifi;
    LEDs leds;
    #ifdef BATT_IC
    Battery battery;
    #endif
    #ifdef USB_PD_VOLTS_DETECTION
    #ifdef USB_PD_AMPS_DETECTION
    CYPD3177 usbpd;
    #endif
    #endif

    String hotspot_name = WLED_AP_SSID;
    // set int leds_length_per_strip
    int leds_length_per_strip = 60;

    // strings to reduce flash memory usage (used more than twice)
    static const char _name[];
    static const char _readInterval[];

    void lampUdated(){
      //call for notifier -> 0: init 1: direct change 2: button 3: notification 4: nightlight 5: other (No notification)
      // 6: fx changed 7: hue 8: preset cycle 9: blynk 10: alexa
      //setValuesFromFirstSelectedSeg(); //to make transition work on main segment (should no longer be required)
      stateUpdated(CALL_MODE_BUTTON);
      updateInterfaces(CALL_MODE_BUTTON);
    }

  public:
    //Functions called by WLED

    /*
     * setup() is called once at boot. WiFi is not yet connected at this point.
     * You can use it to initialize variables, sensors or similar.
     */
    void setup(){
      #ifdef USB_PD_VOLTS_DETECTION
      #ifdef USB_PD_AMPS_DETECTION
      usbpd.setup();
      #ifdef GLOWLIGHT_GLOWTOWER_2_0
      // detect if GlowLight or GlowerTower is connected
      if (usbpd.requested_amps==3.0){
        hotspot_name = "GlowTower";
        leds_length_per_strip = 59;
        #define PIXEL_COUNT 59
        strip.ablMilliampsMax = 12000;
        // setup length of LED strip
      } else {
        hotspot_name = "GlowLight";
        leds_length_per_strip = 14;
        #define PIXEL_COUNT 14
        strip.ablMilliampsMax = 2500;
        // setup length of LED strip
      }

      // TODO for every entry in class Bus, check if length matches leds_length_per_strip. 
      // If not all match, delete all busses and recreate with correct number of LEDs

      // TODO then update all segments and playlists 
      
      #endif
      #endif
      #endif
      #ifdef BATT_IC
      battery.setup();
      #endif
      buttons.setup();
      leds.setup();
      glowoswifi.setup(hotspot_name);

      // do not show welcome page, but control interface instead (test if this works or gets overwritten)
      showWelcomePage = false;
    }


    /*
     * connected() is called every time the WiFi is (re)connected
     * Use it to initialize network interfaces
     */
    // void connected(){
      
    // }


    /*
     * loop() is called continuously. Here you can check for events, read sensors, etc.
     * 
     */
    void loop(){
      if(strip.isUpdating()) return;
      // set brightness
      // leds.loadLastbrightness();

      // save current preset number, if preset changed
      buttons.checkIfPresetChanged();

      glowoswifi.turnOffOnboot();
      #ifdef BATT_IC
      battery.checkBatteryLevel();
      #endif
      glowoswifi.check_if_wifi_ssid_changed();
    }

    /**
     * handleButton() can be used to override default button behaviour. Returning true
     * will prevent button working in a default way.
     * Replicating button.cpp
     */


    bool handleButton(uint8_t b) {
      return buttons.handle(b);
    }
  


    /*
     * addToJsonInfo() can be used to add custom entries to the /json/info part of the JSON API.
     * Creating an "u" object allows you to add custom key/value pairs to the Info section of the WLED web UI.
     * Below it is shown how this could be used for e.g. a light sensor
     */
    void addToJsonInfo(JsonObject& root)
    {
      
      JsonObject user = root["u"];
      if (user.isNull()) user = root.createNestedObject("u");

      #ifdef BATT_IC
      battery.addToJsonInfo(user);
      #endif
    }


    /*
     * addToJsonState() can be used to add custom entries to the /json/state part of the JSON API (state object).
     * Values in the state object may be modified by connected clients
     */
    /*
    void addToJsonState(JsonObject& root)
    {

    }
    */


    /*
     * readFromJsonState() can be used to receive data clients send to the /json/state part of the JSON API (state object).
     * Values in the state object may be modified by connected clients
     */
    /*
    void readFromJsonState(JsonObject& root)
    {
    }
    */


    /*
     * addToConfig() can be used to add custom persistent settings to the cfg.json file in the "um" (usermod) object.
     * It will be called by WLED when settings are actually saved (for example, LED settings are saved)
     * If you want to force saving the current state, use serializeConfig() in your loop().
     * 
     * CAUTION: serializeConfig() will initiate a filesystem write operation.
     * It might cause the LEDs to stutter and will cause flash wear if called too often.
     * Use it sparingly and always in the loop, never in network callbacks!
     * 
     * addToConfig() will make your settings editable through the Usermod Settings page automatically.
     *
     * Usermod Settings Overview:
     * - Numeric values are treated as floats in the browser.
     *   - If the numeric value entered into the browser contains a decimal point, it will be parsed as a C float
     *     before being returned to the Usermod.  The float data type has only 6-7 decimal digits of precision, and
     *     doubles are not supported, numbers will be rounded to the nearest float value when being parsed.
     *     The range accepted by the input field is +/- 1.175494351e-38 to +/- 3.402823466e+38.
     *   - If the numeric value entered into the browser doesn't contain a decimal point, it will be parsed as a
     *     C int32_t (range: -2147483648 to 2147483647) before being returned to the usermod.
     *     Overflows or underflows are truncated to the max/min value for an int32_t, and again truncated to the type
     *     used in the Usermod when reading the value from ArduinoJson.
     * - Pin values can be treated differently from an integer value by using the key name "pin"
     *   - "pin" can contain a single or array of integer values
     *   - On the Usermod Settings page there is simple checking for pin conflicts and warnings for special pins
     *     - Red color indicates a conflict.  Yellow color indicates a pin with a warning (e.g. an input-only pin)
     *   - Tip: use int8_t to store the pin value in the Usermod, so a -1 value (pin not set) can be used
     *
     * See usermod_v2_auto_save.h for an example that saves Flash space by reusing ArduinoJson key name strings
     * 
     * If you need a dedicated settings page with custom layout for your Usermod, that takes a lot more work.  
     * You will have to add the setting to the HTML, xml.cpp and set.cpp manually.
     * See the WLED Soundreactive fork (code and wiki) for reference.  https://github.com/atuline/WLED
     * 
     * I highly recommend checking out the basics of ArduinoJson serialization and deserialization in order to use custom settings!
     */
    
    void addToConfig(JsonObject& root){
      leds.addToConfig(root,_name);

      #ifdef BATT_IC
      battery.addToConfig(root,_name,_readInterval);
      #endif
      #ifdef ACTION_BTN_PIN
      buttons.addToConfig(root,_name);
      #endif
      glowoswifi.addToConfig(root,_name);
    }


    /*
     * readFromConfig() can be used to read back the custom settings you added with addToConfig().
     * This is called by WLED when settings are loaded (currently this only happens immediately after boot, or after saving on the Usermod Settings page)
     * 
     * readFromConfig() is called BEFORE setup(). This means you can use your persistent values in setup() (e.g. pin assignments, buffer sizes),
     * but also that if you want to write persistent values to a dynamic buffer, you'd need to allocate it here instead of in setup.
     * If you don't know what that is, don't fret. It most likely doesn't affect your use case :)
     * 
     * Return true in case the config values returned from Usermod Settings were complete, or false if you'd like WLED to save your defaults to disk (so any missing values are editable in Usermod Settings)
     * 
     * getJsonValue() returns false if the value is missing, or copies the value into the variable provided and returns true if the value is present
     * The configComplete variable is true only if the "exampleUsermod" object and all values are present.  If any values are missing, WLED will know to call addToConfig() to save them
     * 
     * This function is guaranteed to be called on boot, but could also be called every time settings are updated
     */
    bool readFromConfig(JsonObject& root){
      bool readSuccessfull = false;
      readSuccessfull = glowoswifi.readFromConfig(root,_name);
      readSuccessfull = leds.readFromConfig(root,_name);
      #ifdef BATT_IC
      readSuccessfull = battery.readFromConfig(root,_name,_readInterval);
      #endif
      #ifdef ACTION_BTN_PIN
      readSuccessfull = buttons.readFromConfig(root,_name);
      #endif
      
      return readSuccessfull;
    }

   
    /*
     * getId() allows you to optionally give your V2 usermod an unique ID (please define it in const.h!).
     * This could be used in the future for the system to determine whether your usermod is installed.
     */
    uint16_t getId()
    {
      return USERMOD_ID_GlowOS;
    }
};

// strings to reduce flash memory usage (used more than twice)
const char UsermodGlowOS::_name[]         PROGMEM = "GlowOS";
const char UsermodGlowOS::_readInterval[] PROGMEM = "read-interval-ms";