#ifndef BATTERY_SETTINGS

#define BATTERY_SETTINGS

// pin defaults
// for the esp32 it is best to use the ADC1: GPIO32 - GPIO39
// https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html
#ifndef USERMOD_BATTERY_MEASUREMENT_PIN
  #ifdef ARDUINO_ARCH_ESP32
    #define USERMOD_BATTERY_MEASUREMENT_PIN 32
  #else //ESP8266 boards
    #define USERMOD_BATTERY_MEASUREMENT_PIN A0
  #endif
#endif

// esp32 has a 12bit adc resolution
// esp8266 only 10bit
#ifndef USERMOD_BATTERY_ADC_PRECISION
  #ifdef ARDUINO_ARCH_ESP32
    // 12 bits
    #define USERMOD_BATTERY_ADC_PRECISION 4095.0f
  #else
    // 10 bits
    #define USERMOD_BATTERY_ADC_PRECISION 1024.0f
  #endif
#endif


// the frequency to check the battery, 30 sec
#ifndef USERMOD_BATTERY_MEASUREMENT_INTERVAL
  #define USERMOD_BATTERY_MEASUREMENT_INTERVAL 30000
#endif


#ifdef BATT_IC_RT9471D
    // define batt IC defaults
    #define BATT_IC_ADDRESS 0x53 // TODO: make sure the address is correct, check datasheet and try out
    #ifndef BATT_IC_STAT_EN
      #define BATT_IC_STAT_EN 0b0 // STAT pin function disabled
    #endif
    #ifndef BATT_IC_OTG_EN
      #define BATT_IC_OTG_EN 0b1 // Enable OTG
    #endif
    #ifndef BATT_IC_AICC_EN
      #define BATT_IC_AICC_EN 0b0
    #endif
    #ifndef BATT_IC_AUTO_AICR
      #define BATT_IC_AUTO_AICR 0b0 // no action
    #endif
    #ifndef BATT_IC_IAICR_MILLIAMPS
      #define BATT_IC_IAICR_MILLIAMPS 2400
    #endif
    #ifndef BATT_IC_IAICR_BINARY
      #define BATT_IC_IAICR_BINARY 0b110000
    #endif
     #ifndef BATT_IC_VSYS_MIN_MILLIVOLTS
      #define BATT_IC_VSYS_MIN_MILLIVOLTS 4100
    #endif
    #ifndef BATT_IC_VSYS_MIN_BINARY
      #define BATT_IC_VSYS_MIN_BINARY 0b1111
    #endif
    #ifndef ABL_MILLIAMPS_DEFAULT
      #define ABL_MILLIAMPS_DEFAULT 600
    #endif

    // define default battery values
    #ifndef BATT_IC_ICHG_REG_MILLIAMPS
      #define BATT_IC_ICHG_REG_MILLIAMPS 400 //mA
    #endif
    #ifndef BATT_IC_ICHG_REG_BINARY
      #define BATT_IC_ICHG_REG_BINARY 0b1000
    #endif
    #ifndef USERMOD_BATTERY_MAX_VOLTAGE
      #define USERMOD_BATTERY_MAX_VOLTAGE 4.1f
    #endif
    #ifndef BATT_IC_VBAT_REG_MILLIVOLTS
      #define BATT_IC_VBAT_REG_MILLIVOLTS 4100
    #endif
    #ifndef BATT_IC_VBAT_REG_BINARY
      #define BATT_IC_VBAT_REG_BINARY 0b10100
    #endif

    // then overwrite default values, if specific battery defined
    #ifdef BATT_LP601730_260mAh
      #define BATT_IC_ICHG_REG_MILLIAMPS 300
      #define BATT_IC_ICHG_REG_BINARY 0b110
      #define ABL_MILLIAMPS_DEFAULT 250
    #endif

    #ifdef BATT_LP542730_340mAh
      #define BATT_IC_ICHG_REG_MILLIAMPS 300
      #define BATT_IC_ICHG_REG_BINARY 0b110
      #define ABL_MILLIAMPS_DEFAULT 500
    #endif

    #ifdef BATT_LP803030_700mAh
      #define BATT_IC_ICHG_REG_MILLIAMPS 800
      #define BATT_IC_ICHG_REG_BINARY 0b10000
      #define ABL_MILLIAMPS_DEFAULT 500
    #endif

    #ifdef BATT_LP653042_820mAh
      #define BATT_IC_ICHG_REG_MILLIAMPS 800
      #define BATT_IC_ICHG_REG_BINARY 0b10000
      #define ABL_MILLIAMPS_DEFAULT 800   
    #endif

    #ifdef BATT_LP675776_3300mAh
      #define BATT_IC_ICHG_REG_MILLIAMPS 2000
      #define BATT_IC_ICHG_REG_BINARY 0b101000
      #define ABL_MILLIAMPS_DEFAULT 1000   
    #endif

    #ifdef BATT_LP103395_3500mAh
      #define BATT_IC_ICHG_REG_MILLIAMPS 2000
      #define BATT_IC_ICHG_REG_BINARY 0b101000
      #define ABL_MILLIAMPS_DEFAULT 1000  
    #endif

    #ifdef BATT_MOLICEL_INR18650_M35A
      #define BATT_IC_ICHG_REG_MILLIAMPS 1500
      #define BATT_IC_ICHG_REG_BINARY 0b11110
      #define ABL_MILLIAMPS_DEFAULT 10000
    #endif

#endif


// default for 18650 battery
// https://batterybro.com/blogs/18650-wholesale-battery-reviews/18852515-when-to-recycle-18650-batteries-and-how-to-start-a-collection-center-in-your-vape-shop
// Discharge voltage: 3.0 volt + .1 for personal safety
#ifndef USERMOD_BATTERY_MIN_VOLTAGE
  #define USERMOD_BATTERY_MIN_VOLTAGE 3.0f
#endif

#ifndef USERMOD_BATTERY_MAX_VOLTAGE
  #define USERMOD_BATTERY_MAX_VOLTAGE 4.2f
#endif

#endif