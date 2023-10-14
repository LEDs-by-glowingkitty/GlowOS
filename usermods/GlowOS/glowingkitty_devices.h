#ifndef GlowingkittyDevices_h
#define GlowingkittyDevices_h

#ifdef MAGIQWAND_1_0_BETA3
    // Setup the LED settings
    #define DATA_PINS 16, 17
    #define PIXEL_COUNTS 21, 21
    #define ABL_MILLIAMPS_DEFAULT 600
    #define RLYPIN 23

    // Setup the buttons
    #define POWER_BTN_PIN 4
    #define ACTION_BTN_PIN 2
    #define POWER_BTN_PIN_STATE_IF_PRESSED 0
    #define ACTION_BTN_PIN_STATE_IF_PRESSED 0

    // Setup WiFi
    #define DEFAULT_AP_SSID "MagiqWand"
    #define WIFI_OFF_ON_BOOT

    // Setup microphone
    #define USERMOD_AUDIOREACTIVE
    #define SR_DMTYPE 5
    #define I2S_WSPIN 14
    #define I2S_CKPIN 15

    // Setup battery
    #define BATT_IC
    #define USERMOD_BATTERY_MEASUREMENT_PIN 34
#endif

#ifdef MAGIQWAND_1_0_RC1
    // Setup the LED settings
    #define DATA_PINS 16, 17
    #define PIXEL_COUNTS 21, 21
    #define ABL_MILLIAMPS_DEFAULT 600
    #define RLYPIN 23

    // Setup the buttons
    #define POWER_BTN_PIN 33
    #define ACTION_BTN_PIN 4
    #define POWER_BTN_PIN_STATE_IF_PRESSED 0
    #define ACTION_BTN_PIN_STATE_IF_PRESSED 0

    // Setup WiFi
    #define DEFAULT_AP_SSID "MagiqWand"
    #define WIFI_OFF_ON_BOOT

    // Setup microphone
    #define USERMOD_AUDIOREACTIVE
    #define SR_DMTYPE 5
    #define I2S_WSPIN 14
    #define I2S_CKPIN 15
    #define I2S_WSPIN 14
    #define I2S_CKPIN 15
    
    // Setup battery
    #define BATT_IC
    #define USERMOD_BATTERY_MEASUREMENT_PIN 26
#endif

#ifdef MAGIQWAND_1_0_RC2
    // Setup the LED settings
    #define DATA_PINS 16, 17
    #define PIXEL_COUNTS 21, 21
    #define ABL_MILLIAMPS_DEFAULT 600
    #define RLYPIN 23
    #define RLYLATCHPIN 22

    // Setup the buttons
    #define POWER_BTN_PIN 33
    #define ACTION_BTN_PIN 4
    #define POWER_BTN_PIN_STATE_IF_PRESSED 0
    #define ACTION_BTN_PIN_STATE_IF_PRESSED 0

    // Setup WiFi
    #define DEFAULT_AP_SSID "MagiqWand"
    #define WIFI_OFF_ON_BOOT

    // Setup microphone
    #define USERMOD_AUDIOREACTIVE
    #define SR_DMTYPE 5
    #define I2S_WSPIN 14
    #define I2S_CKPIN 15
    
    // Setup battery
    #define BATT_IC
    #define USERMOD_BATTERY_MEASUREMENT_PIN 26
#endif

#ifdef GLOWCORE_1_0_RC1
    // Setup the LED settings
    #define DATA_PINS 16, 17
    #define PIXEL_COUNTS 60, 60
    #define ABL_MILLIAMPS_DEFAULT 2500

    // Setup the buttons
    #define ACTION_BTN_PIN 0
    #define ACTION_BTN_PIN_STATE_IF_PRESSED 0

    // Setup WiFi
    #define DEFAULT_AP_SSID "GlowCore"

    // Setup microphone
    #define USERMOD_AUDIOREACTIVE
    #define SR_DMTYPE 5
    #define I2S_WSPIN 14
    #define I2S_CKPIN 15
#endif

#ifdef GLOWCORE_2_0
    // Setup the LED settings
    #define DATA_PINS 16, 17
    #define PIXEL_COUNTS 60, 60
    #define ABL_MILLIAMPS_DEFAULT 3000
    #define RLYPIN 23
    #define RLYLATCHPIN 22

    // Setup the buttons
    #define POWER_BTN_PIN 33
    #define ACTION_BTN_PIN 0
    #define POWER_BTN_PIN_STATE_IF_PRESSED 1
    #define ACTION_BTN_PIN_STATE_IF_PRESSED 0

    // Setup WiFi
    #define DEFAULT_AP_SSID "GlowCore"

    // Setup microphone
    #define USERMOD_AUDIOREACTIVE
    #define SR_DMTYPE 5
    #define I2S_WSPIN 14
    #define I2S_CKPIN 15
#endif


#ifdef GLOWCORE_3_0
    // Setup the LED settings
    #define DATA_PINS 42, 41
    #define PIXEL_COUNTS 60, 60
    #define STATUS_LED_DATA 40
    #define STATUS_LED_CLOCK 39
    #define RLYPIN 16
    #define RLYLATCHPIN 15

    // Setup the buttons
    #define POWER_BTN_PIN 1
    #define ACTION_BTN_PIN 3
    #define POWER_BTN_PIN_STATE_IF_PRESSED 1
    #define ACTION_BTN_PIN_STATE_IF_PRESSED 1

    // Setup WiFi
    #define DEFAULT_AP_SSID "GlowCore"
#endif


#ifdef GLOWONESIE_1_0
    // BASED ON GLOWCORE 3.0

    // Setup the LED settings
    #define DATA_PINS 42
    #define PIXEL_COUNTS 50
    #define STATUS_LED_DATA 40
    #define STATUS_LED_CLOCK 39
    #define RLYPIN 16
    #define RLYLATCHPIN 15
    #define ABL_MILLIAMPS_DEFAULT 3000

    // Setup the buttons
    #define POWER_BTN_PIN 1
    #define ACTION_BTN_PIN 3
    #define POWER_BTN_PIN_STATE_IF_PRESSED 1
    #define ACTION_BTN_PIN_STATE_IF_PRESSED 1
    #define SWITCH_LEFT 36
    #define SWITCH_RIGHT 37

    // Setup WiFi
    #define DEFAULT_AP_SSID "GlowOnesie"
    #define WIFI_OFF_ON_BOOT
#endif


#ifdef GLOWBIKE_1_0
    // BASED ON GLOWCORE 3.0

    // Setup the LED settings
    #define DATA_PINS 42
    #define PIXEL_COUNTS 176
    #define STATUS_LED_DATA 40
    #define STATUS_LED_CLOCK 39
    #define RLYPIN 16
    #define RLYLATCHPIN 15
    #define ABL_MILLIAMPS_DEFAULT 3000

    // Setup the buttons
    #define POWER_BTN_PIN 1
    #define ACTION_BTN_PIN 3
    #define POWER_BTN_PIN_STATE_IF_PRESSED 1
    #define ACTION_BTN_PIN_STATE_IF_PRESSED 1
    #define SWITCH_LEFT 36
    #define SWITCH_RIGHT 37

    // Setup WiFi
    #define DEFAULT_AP_SSID "GlowBike"
    // #define WIFI_OFF_ON_BOOT
#endif

#ifdef GLOWCORE_MINI_2_0
    // Setup the LED settings
    #define DATA_PINS 16
    #define PIXEL_COUNTS 60
    #define STATUS_LED_DATA 5
    #define STATUS_LED_CLOCK 18
    #define ABL_MILLIAMPS_DEFAULT 2000
    #define RLYPIN 23
    #define RLYLATCHPIN 22

    // Setup the buttons
    #define POWER_BTN_PIN 33
    #define ACTION_BTN_PIN 0
    #define POWER_BTN_PIN_STATE_IF_PRESSED 1
    #define ACTION_BTN_PIN_STATE_IF_PRESSED 0

    // Setup WiFi
    #define DEFAULT_AP_SSID "GlowCore Mini"

    #define BATT_IC
    #define BATT_IC_RT9471D

    // Setup microphone
    #define USERMOD_AUDIOREACTIVE
    #define SR_DMTYPE 5
    #define I2S_WSPIN 14
    #define I2S_CKPIN 15
#endif

#ifdef GLOWCORE_MINI_3_0
    // Setup the LED settings
    #define DATA_PINS 42, 41
    #define PIXEL_COUNTS 60, 60
    #define STATUS_LED_DATA 40
    #define STATUS_LED_CLOCK 39
    #define ABL_MILLIAMPS_EXTERNAL_POWER_DEFAULT 2400
    #define RLYPIN 16
    #define RLYLATCHPIN 15

    // Setup the buttons
    #define POWER_BTN_PIN 1
    #define ACTION_BTN_PIN 0
    #define POWER_BTN_PIN_STATE_IF_PRESSED 1
    #define ACTION_BTN_PIN_STATE_IF_PRESSED 0

    // Setup WiFi
    #define DEFAULT_AP_SSID "GlowCore Mini"

    // Setup microphone
    #define USERMOD_AUDIOREACTIVE
    #define SR_DMTYPE 5
    #define I2S_WSPIN 14
    #define I2S_CKPIN 15

    // Setup battery
    #define BATT_IC
    #define USERMOD_BATTERY_MEASUREMENT_PIN 2
    #define BATT_IC_RT9471D
    #define I2C_SDA_PIN 8
    #define I2C_SCL_PIN 9
    #define I2C_BATT_IC_INT 11
    #define BATT_IC_EN 47

    // Setup gyro
    #define GYRO_LSM6DS
    #define I2C_GYRO_INT 10
#endif

#ifdef GLOWCORE_MINI_3_1
    // Setup the LED settings
    #define DATA_PINS 42, 41
    #define PIXEL_COUNTS 60, 60
    #define STATUS_LED_DATA 40
    #define STATUS_LED_CLOCK 39
    #define ABL_MILLIAMPS_EXTERNAL_POWER_DEFAULT 2400
    #define RLYPIN 16
    #define RLYLATCHPIN 15

    // Setup the buttons
    #define POWER_BTN_PIN 1
    #define ACTION_BTN_PIN 3
    #define POWER_BTN_PIN_STATE_IF_PRESSED 1
    #define ACTION_BTN_PIN_STATE_IF_PRESSED 1

    // Setup WiFi
    #define DEFAULT_AP_SSID "GlowCore Mini"

    // Setup microphone
    #define USERMOD_AUDIOREACTIVE
    #define SR_DMTYPE 5
    #define I2S_WSPIN 14
    #define I2S_CKPIN 15

    // Setup battery
    #define BATT_IC
    #define USERMOD_BATTERY_MEASUREMENT_PIN 2
    #define BATT_IC_RT9471D
    #define I2C_SDA_PIN 8
    #define I2C_SCL_PIN 9
    #define I2C_BATT_IC_INT 11
    #define BATT_IC_EN 47

    // Setup gyro
    #define GYRO_LSM6DS
    #define I2C_GYRO_INT 10
#endif

#ifdef GLOWBAND_1_0_BETA1
    // Setup the LED settings
    #define DATA_PINS 16, 17
    #define PIXEL_COUNTS 15, 15
    #define ABL_MILLIAMPS_DEFAULT 600
    #define RLYPIN 23
    #define RLYLATCHPIN 19

    // Setup the buttons
    #define POWER_BTN_PIN 33
    #define ACTION_BTN_PIN 4
    #define POWER_BTN_PIN_STATE_IF_PRESSED 1
    #define ACTION_BTN_PIN_STATE_IF_PRESSED 1

    // Setup WiFi
    #define DEFAULT_AP_SSID "GlowBand"
    #define WIFI_OFF_ON_BOOT

    // Setup microphone
    #define USERMOD_AUDIOREACTIVE
    #define SR_DMTYPE 5
    #define I2S_WSPIN 14
    #define I2S_CKPIN 15

    // Setup battery
    #define BATT_IC
    #define USERMOD_BATTERY_MEASUREMENT_PIN 26
    #define BATT_LP653042_820mA
#endif

#ifdef GLOWSIGN_1_0_BETA2
    // Setup the LED settings
    #define DATA_PINS 16
    #define PIXEL_COUNTS 35
    #define ABL_MILLIAMPS_DEFAULT 600
    #define RLYPIN 23

    // Setup the buttons
    #define POWER_BTN_PIN 18
    #define POWER_BTN_PIN_STATE_IF_PRESSED 0

    // Setup WiFi
    #define DEFAULT_AP_SSID "GlowSign"
    #define WIFI_OFF_ON_BOOT

    // Setup microphone
    #define USERMOD_AUDIOREACTIVE
    #define SR_DMTYPE 5
    #define I2S_WSPIN 14
    #define I2S_CKPIN 15

    // Setup battery
    #define BATT_IC
    #define USERMOD_BATTERY_MEASUREMENT_PIN 12
#endif

#if defined(GLOWSIGN_1_0_RC1) || defined(GLOWSIGN_1_0_RC2)
    // Setup the LED settings
    #define DATA_PINS 16
    #define PIXEL_COUNTS 33
    #define ABL_MILLIAMPS_DEFAULT 600
    #define RLYPIN 23

    // Setup the buttons
    #define POWER_BTN_PIN 33
    #define POWER_BTN_PIN_STATE_IF_PRESSED 0

    // Setup WiFi
    #define DEFAULT_AP_SSID "GlowSign"
    #define WIFI_OFF_ON_BOOT

    // Setup microphone
    #define USERMOD_AUDIOREACTIVE
    #define SR_DMTYPE 5
    #define I2S_WSPIN 14
    #define I2S_CKPIN 15

    // Setup battery
    #define BATT_IC
    #define USERMOD_BATTERY_MEASUREMENT_PIN 26
#endif

#ifdef GLOWTOWER_1_0
    // Setup the LED settings
    #define DATA_PINS 16
    #define PIXEL_COUNTS 240
    #define ABL_MILLIAMPS_DEFAULT 7500

    // Setup the buttons
    #define ACTION_BTN_PIN 33
    #define ACTION_BTN_PIN_STATE_IF_PRESSED 0

    // Setup WiFi
    #define DEFAULT_AP_SSID "GlowTower"

    // Setup microphone
    #define USERMOD_AUDIOREACTIVE
    #define SR_DMTYPE 5
    #define I2S_WSPIN 14
    #define I2S_CKPIN 15
#endif

#ifdef GLOWTOWER_2_0_DEV_2
    // Setup the LED settings
    #define DATA_PINS 42, 41, 4, 5
    #define PIXEL_COUNTS 60, 60, 60, 60
    #define ABL_MILLIAMPS_DEFAULT 14000
    #define STATUS_LED_DATA 40
    #define RLYPIN 16
    #define RLYLATCHPIN 15

    // Setup the buttons
    #define POWER_BTN_PIN 1
    #define ACTION_BTN_PIN 3
    #define POWER_BTN_PIN_STATE_IF_PRESSED 1
    #define ACTION_BTN_PIN_STATE_IF_PRESSED 1

    // Setup USB-C PD
    #define USB_PD_FAULT 2
    #define USB_PD_MODE_DETECTION 6

    // Setup microphone
    // #define USERMOD_AUDIOREACTIVE
    // #define SR_DMTYPE 5
    // #define I2S_WSPIN 12
    // #define I2S_CKPIN 13
    // #define I2S_SDPIN 14

    // Setup WiFi
    #define DEFAULT_AP_SSID "GlowTower"
#endif

#ifdef GLOWLIGHT_1_0
    // Setup the LED settings
    #define DATA_PINS 16
    #define PIXEL_COUNTS 60
    #define ABL_MILLIAMPS_DEFAULT 2500

    // Setup the buttons
    #define ACTION_BTN_PIN 33
    #define ACTION_BTN_PIN_STATE_IF_PRESSED 0

    // Setup WiFi
    #define DEFAULT_AP_SSID "GlowLight"

    // Setup microphone
    #define USERMOD_AUDIOREACTIVE
    #define SR_DMTYPE 5
    #define I2S_WSPIN 14
    #define I2S_CKPIN 15
#endif

#ifdef GLOWLIGHT_2_0_DEV_2
    // Setup the LED settings
    #define DATA_PINS 42, 41, 4, 5
    #define PIXEL_COUNTS 15, 15, 15, 15
    #define ABL_MILLIAMPS_DEFAULT 3300
    #define STATUS_LED_DATA 40
    #define RLYPIN 16
    #define RLYLATCHPIN 15

    // Setup the buttons
    #define POWER_BTN_PIN 1
    #define ACTION_BTN_PIN 3
    #define POWER_BTN_PIN_STATE_IF_PRESSED 1
    #define ACTION_BTN_PIN_STATE_IF_PRESSED 1

    // Setup USB-C PD
    #define USB_PD_FAULT 2
    #define USB_PD_MODE_DETECTION 6

    // Setup microphone
    // #define USERMOD_AUDIOREACTIVE
    // #define SR_DMTYPE 5
    // #define I2S_WSPIN 12
    // #define I2S_CKPIN 13
    // #define I2S_SDPIN 14

    // Setup WiFi
    #define DEFAULT_AP_SSID "GlowLight"
#endif

#ifdef GLOWLIGHT_GLOWTOWER_2_0
    // Setup the LED settings
    #define DATA_PINS 42, 41, 4, 5
    // #define PIXEL_COUNTS 15, 15, 15, 15
    // #define ABL_MILLIAMPS_DEFAULT 3300
    #define STATUS_LED_DATA 40
    #define VBUS_TO_5V_MOSFET 2
    #define VBUS_TO_5V_LATCH 10

    // Setup the buttons
    #define POWER_BTN_PIN 1
    #define ACTION_BTN_PIN 3
    #define POWER_BTN_PIN_STATE_IF_PRESSED 1
    #define ACTION_BTN_PIN_STATE_IF_PRESSED 1

    // Setup USB-C PD
    #define USB_PD_AMPS_DETECTION 6
    #define USB_PD_VOLTS_DETECTION 7

    // Setup microphone
    // #define USERMOD_AUDIOREACTIVE
    // #define SR_DMTYPE 5
    // #define I2S_WSPIN 12
    // #define I2S_CKPIN 13
    // #define I2S_SDPIN 14

    // Setup WiFi
    // #define DEFAULT_AP_SSID "GlowLight"
#endif

#ifdef GLOWTUBE_1_0
    // Setup the LED settings
    #define DATA_PINS 16, 17
    #define PIXEL_COUNTS 60, 60
    #define ABL_MILLIAMPS_DEFAULT 2500

    // Setup the buttons
    #define ACTION_BTN_PIN 33
    #define ACTION_BTN_PIN_STATE_IF_PRESSED 0

    // Setup WiFi
    #define DEFAULT_AP_SSID "GlowTube"
    
    // Setup microphone
    #define USERMOD_AUDIOREACTIVE
    #define SR_DMTYPE 5
    #define I2S_WSPIN 14
    #define I2S_CKPIN 15
#endif

#endif

#ifndef WLED_DEFINE_GLOBAL_VARS
# define WLED_GLOBAL extern
# define _INIT(x)
# define _INIT_N(x)
#else
# define WLED_GLOBAL
# define _INIT(x) = x
#endif