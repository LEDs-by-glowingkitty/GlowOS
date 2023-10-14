#pragma once
#include <Wire.h>

class RT9471D {
private:

    void begin(){
        bool i2c_initialized = Wire.begin(26, 13);
        if (i2c_initialized == false) {
        // DEBUG_PRINTLN(F("Failed to initialize I2C bus driver."));
          Serial.println("Failed to initialize I2C bus driver.");
        }
    }

    void write(uint8_t reg, uint8_t val){
        Serial.println("RT9471D.write()");
        Serial.println(reg);
        Serial.println(val);
        Wire.beginTransmission(BATT_IC_ADDRESS);
        Wire.write((uint8_t)reg);
        Wire.write((uint8_t)val);
        uint8_t i2cErr = Wire.endTransmission();  // i2cErr == 0 means OK
        Serial.println(i2cErr);
        if (i2cErr != 0) {
        // DEBUG_PRINTLN("RT9471D I2C write failed");
          Serial.println("RT9471D I2C write failed");
        }
    }

    void readFUNCTION(){
      uint8_t data = 0;
      Wire.beginTransmission(BATT_IC_ADDRESS);
      Wire.write(0x02); // Register to read from
      Wire.endTransmission();
      Wire.requestFrom(BATT_IC_ADDRESS, 1); // Request 1 byte of data
      if (Wire.available() >= 1) {
        Serial.println("FUNCTION register:");
        data = Wire.read();
        const char *labels[] = { "CHG_EN", "OTG_EN", "UUG_FULLON", "BUCK_PFM_DIS", "", "HZ", "BATFET_DIS_DLY", "BATFET_DIS" };
        for (int i = 0; i < 8; i++) {
          if (i == 4) {
            continue;
          }
          Serial.println(String(labels[i]) + ": " + String((data & (1 << i)) >> i, BIN));
        }
      } else {
        Serial.println("readFUNCTION failed");
      }
    }

    void readIBUS() {
      uint8_t data = 0;
      Wire.beginTransmission(BATT_IC_ADDRESS); // Address of the device on the I2C bus
      Wire.write(0x03); // Register to read from
      Wire.endTransmission();
      Wire.requestFrom(BATT_IC_ADDRESS, 1); // Request 1 byte of data
      if (Wire.available() >= 1) {
        Serial.println("IBUS register:");
        data = Wire.read();

        Serial.println("IAICR: "+String((data & 0x3F)*50,DEC)+"mA");
        Serial.println("AUTO_AICR: " + String((data & (1 << 6)) >> 6, BIN)); // bit 6
        Serial.println("AICC_EN: " + String((data & (1 << 7)) >> 7, BIN)); // bit 7
      } else {
        Serial.println("readIBUS failed");
      }
    }

    void readVSYS_MIN() {
      uint8_t data = 0;
      Wire.beginTransmission(BATT_IC_ADDRESS); // Address of the device on the I2C bus
      Wire.write(0x06); // Register to read from
      Wire.endTransmission();
      Wire.requestFrom(BATT_IC_ADDRESS, 1); // Request 1 byte of data
      if (Wire.available() >= 1) {
        data = Wire.read();

        Serial.println("VSYS_MIN: " + String(((data & 0x0F)*100)+2600, DEC)+"mV"); // bits 3:0
      } else {
        Serial.println("readVSYS_MIN failed");
      }
    }

    void readIC_STATUS(){
      uint8_t data = 0;
      Wire.beginTransmission(BATT_IC_ADDRESS); // Address of the device on the I2C bus
      Wire.write(0x0F); // Register to read from
      Wire.endTransmission();
      Wire.requestFrom(BATT_IC_ADDRESS, 1); // Request 1 byte of data
      if (Wire.available() >= 1) {
        data = Wire.read();

        switch (data & 0x0F) {
          case 0b0000: Serial.println("IC_STAT: 0000 : HZ/SLEEP"); break;
          case 0b0001: Serial.println("IC_STAT: 0001 : VBUS ready for charge"); break;
          case 0b0010: Serial.println("IC_STAT: 0010 : Trickle-charge"); break;
          case 0b0011: Serial.println("IC_STAT: 0011 : Pre-charge"); break;
          case 0b0100: Serial.println("IC_STAT: 0100 : Fast-charge"); break;
          case 0b0101: Serial.println("IC_STAT: 0101 : IEOC-charge (EOC and TE = 0)"); break;
          case 0b0110: Serial.println("IC_STAT: 0110 : Back-Ground charge (EOC and TE = 1 and before turn off power path)"); break;
          case 0b0111: Serial.println("IC_STAT: 0111 : Charge done (EOC and TE = 1 and power path off)"); break;
          case 0b1000: Serial.println("IC_STAT: 1000 : Charge fault (VAC_OV / CHG_BUSUV / CHG_TOUT / CHG_SYSOV / CHG_BATOV / JEITA_HOT / JEITA_COLD / OTP)"); break;
          case 0b1111: Serial.println("IC_STAT: 1111 : OTG"); break;
          default: Serial.println("Unknown IC_STAT value: " + String(data & 0x0F, BIN)); break;
        }

        switch ((data >> 4) & 0x0F) {
          case 0b0000: Serial.println("PORT_STAT: 0000 : No information"); break;
          case 0b1000: Serial.println("PORT_STAT: 1000 : VBUS = device 1 (2100mA-APPLE10w)"); break;
          case 0b1001: Serial.println("PORT_STAT: 1001 : VBUS = device 2 (2000mASAMSUNG-10w)"); break;
          case 0b1010: Serial.println("PORT_STAT: 1010 : VBUS = device 3 (1000mA-APPLE5w)"); break;
          case 0b1011: Serial.println("PORT_STAT: 1011 : VBUS = device 4 (2400mA-APPLE12w)"); break;
          case 0b1100: Serial.println("PORT_STAT: 1100 : VBUS = unknown / NSDP (500mA)"); break;
          case 0b1101: Serial.println("PORT_STAT: 1101 : VBUS = SDP (500mA) / PSEL = High"); break;
          case 0b1110: Serial.println("PORT_STAT: 1110 : VBUS = CDP (1500mA)"); break;
          case 0b1111: Serial.println("PORT_STAT: 1111 : VBUS = DCP (2400mA) / PSEL = Low"); break;
          default: Serial.println("Unknown PORT_STAT value: " + String((data >> 4) & 0x0F, BIN)); break;
        }
      } else {
        Serial.println("readIC_STATUS failed");
      }
    }


    void config(){
      // IBUS
      const uint8_t AICC_EN   = BATT_IC_AICC_EN        << 7; // Adaptive Input Current Control enable
      const uint8_t AUTO_AICR = BATT_IC_AUTO_AICR      << 6; // Average Input Current Regulation cannot be detected via USB D+ and D-, because they are used for ESP32 S3. Therefore value set manually.
      const uint8_t IAICR     = BATT_IC_IAICR_BINARY   << 0; // IAICR -> average current that can be drawn from the USB port. Set manually, because D+ and D- are used for ESP32 S3.
      const uint8_t IBUS      = AICC_EN | AUTO_AICR | IAICR;
      write(0x03,IBUS);

      // REGU
      write(0x06,BATT_IC_VSYS_MIN_BINARY); // VSYS_MIN -> boost converter output min voltage.

      // VCHG
      write(0x07,BATT_IC_VBAT_REG_BINARY); // VBAT_REG -> max voltage when fully charged. Recommended: 4100mV
      
      // ICHG
      write(0x08,BATT_IC_ICHG_REG_BINARY); // ICHG_REG -> max charging speed. Depends on battery.

      // JEITA_EN -> 0 (disable)
      
      // OTG_EN -> 1 (Enable OTG)
      const uint8_t BATFET_DIS      = 0b0   << 7;
      const uint8_t BATFET_DIS_DLY  = 0b1   << 6;
      const uint8_t HZ              = 0b0   << 5;
      const uint8_t Reserved        = 0b0   << 4;
      const uint8_t BUCK_PFM_DIS    = 0b0   << 3;
      const uint8_t UUG_FULLON      = 0b0   << 2;
      const uint8_t OTG_EN          = 0b1   << 1; // Enable OTG
      const uint8_t CHG_EN          = 0b1   << 0; // Enable charge
      const uint8_t FUNCTION        = BATFET_DIS | BATFET_DIS_DLY | HZ | Reserved | BUCK_PFM_DIS | UUG_FULLON | OTG_EN | CHG_EN;
      write(0x02,FUNCTION);

      // STAT_EN -> 0 (STAT pin function disable)
      
    }

    void readall(){
      readFUNCTION();
      readIBUS();
      readVSYS_MIN();
      readIC_STATUS();
    }

public:

    void setup(){
      begin();
      config();
      readall();
    }

    void checkstatus(){
      Serial.println("Checking status...");
      readFUNCTION();
      readIBUS();
      readVSYS_MIN();
      readIC_STATUS();
    }

};