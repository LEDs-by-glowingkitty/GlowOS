#pragma once

#include <Arduino.h>

#define V_REF 3.3   // Reference voltage
#define ADC_RESOLUTION 4096  // 12-bit

// code for interacting with CYPD3177 USB PD controller by Cypress/Infineon

class CYPD3177 {
    private:
        void updateVBUSvoltage(){
            // detect what voltage is on VBUS and update vbus_voltage and usb_pd_negotiation_failed
            int usb_pd_volt_pin_adc_value = analogRead(USB_PD_VOLTS_DETECTION);
            float usb_pd_volt_pin_voltage = (usb_pd_volt_pin_adc_value / (float)ADC_RESOLUTION) * V_REF;

            // if usb_pd_volt_pin is 0V, this should mean that USB PD negotiation failed and that VBUS is 5V
            if (usb_pd_volt_pin_voltage == 0.0){
                vbus_voltage = 5.0;
                usb_pd_negotiation_failed = true;
            } else {
                // consider that the usb_pd_volt_pin is connected to VBUS via a voltage divider
                // therefore, if VBUS => 20V, then voltage => 3.3V
                // convert voltage to VBUS
                vbus_voltage = usb_pd_volt_pin_voltage * 20.0 / 3.3;
                usb_pd_negotiation_failed = false;
            }
        }

        void updateRequestedAmps(){
            float calculated_amps = 0.0;

            // return the requested amps based on the voltage_adc_value
            int usb_pd_amp_pin_adc_value = analogRead(USB_PD_AMPS_DETECTION);
            float usb_pd_amp_pin_voltage = (usb_pd_amp_pin_adc_value / (float)ADC_RESOLUTION) * V_REF;
            // 0.0 - 0.248V -> 0A
            // 0.249 - 0.786V -> 1A
            // 0.787 - 1.347V -> 2A
            // 1.348 - 1.92V -> 3A
            // 1.921 - 2.778V -> 4A
            // > 2.778V -> 5A

            // get correct calculated_amps
            if (usb_pd_amp_pin_voltage < 0.249){
                calculated_amps = 0.0;
            } else if (usb_pd_amp_pin_voltage < 0.787){
                calculated_amps = 1.0;
            } else if (usb_pd_amp_pin_voltage < 1.348){
                calculated_amps = 2.0;
            } else if (usb_pd_amp_pin_voltage < 1.921){
                calculated_amps = 3.0;
            } else if (usb_pd_amp_pin_voltage < 2.778){
                calculated_amps = 4.0;
            } else {
                calculated_amps = 5.0;
            }
            
            
            #ifdef USB_PD_AMPS_FINE_DETECTION
            // if fine setting is also available, add that value to the requested amps
            int usb_pd_amp_fine_pin_adc_value = analogRead(USB_PD_AMPS_FINE_DETECTION);
            float usb_pd_amp_fine_pin_voltage = (usb_pd_amp_fine_pin_adc_value / (float)ADC_RESOLUTION) * V_REF;
            // 0.0 - 0.248V -> +0
            // 0.249 - 0.786V -> +0.25
            // 0.787 - 1.347V -> +0.5
            // 1.348 - 1.92V -> +0.75
            // > 1.92V -> +1

            // get correct calculated_amps
            if (usb_pd_amp_fine_pin_voltage < 0.249){
                calculated_amps += 0.0;
            } else if (usb_pd_amp_fine_pin_voltage < 0.787){
                calculated_amps += 0.25;
            } else if (usb_pd_amp_fine_pin_voltage < 1.348){
                calculated_amps += 0.5;
            } else if (usb_pd_amp_fine_pin_voltage < 1.92){
                calculated_amps += 0.75;
            } else {
                calculated_amps += 1.0;
            }
            #endif

            requested_amps = calculated_amps;
        }
        
    public:
        float requested_amps;
        float vbus_voltage;
        bool usb_pd_negotiation_failed;

        void setup(){
            #ifdef VBUS_TO_5V_MOSFET
            pinMode(VBUS_TO_5V_MOSFET, OUTPUT);
            digitalWrite(VBUS_TO_5V_MOSFET, LOW); // keep it closed by default
            #endif
            #ifdef VBUS_TO_5V_LATCH
            pinMode(VBUS_TO_5V_LATCH, OUTPUT);
            digitalWrite(VBUS_TO_5V_LATCH, HIGH);
            digitalWrite(VBUS_TO_5V_LATCH, LOW);
            #endif
            delay(1000); // add an extra delay, just in case USB PD IC still is trying to negotiate
            analogReadResolution(12);  // Set ADC resolution to 12-bit

            updateRequestedAmps();
            updateVBUSvoltage();
            // print
            Serial.print("Requested amps:");
            Serial.println(requested_amps);
            Serial.print("VBUS voltage:");
            Serial.println(vbus_voltage);
            Serial.print("USB PD negotiation failed:");
            Serial.println(usb_pd_negotiation_failed);

            #ifdef VBUS_TO_5V_MOSFET
            // if USB PD negotiation failed, open VBUS to 5V MOSFET
            if (vbus_voltage<5.2){
                Serial.println("opening VBUS to 5V MOSFET...");
                digitalWrite(VBUS_TO_5V_MOSFET, HIGH);
                Serial.println("VBUS to 5V MOSFET opened");
            } else {
                digitalWrite(VBUS_TO_5V_MOSFET, LOW);
                Serial.println("VBUS to 5V MOSFET closed");
            }
            #endif
            #ifdef VBUS_TO_5V_LATCH
            pinMode(VBUS_TO_5V_LATCH, OUTPUT);
            digitalWrite(VBUS_TO_5V_LATCH, HIGH);
            pinMode(VBUS_TO_5V_LATCH, OUTPUT);
            digitalWrite(VBUS_TO_5V_LATCH, LOW);
            #endif
        }
    };