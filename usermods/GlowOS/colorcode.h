#pragma once
#include "wled.h"

class ColorCode {
private:

    static uint32_t crc32_for_uint64(uint64_t value) {
      uint32_t crc = 0xffffffff;
      for (int i = 0; i < 8; i++) {
        uint8_t byte = (value >> (i * 8)) & 0xff;
        crc = crc ^ byte;
        for (int j = 0; j < 8; j++) {
          crc = (crc >> 1) ^ ((crc & 1) ? 0xedb88320 : 0);
        }
      }
      return crc ^ 0xffffffff;
    }

public:
    void getValues() {
        
    }

    String getString() {
      // TODO:
      // - set default wifi name with color code
      // - set default animation to color code
      // - if power button pressed for 5 seconds, show color code
      
      // glow the unique color code for this device
      const char *color_names[] = {"🟢", "🔵", "🟣", "🟠", "🔴", "⚪"};
      const int num_colors = 6;

      // Use a PRNG to generate three indices in the list of color names
      srand(crc32_for_uint64(ESP.getEfuseMac()));
      int index1 = rand() % num_colors;
      int index2 = rand() % num_colors;
      int index3 = rand() % num_colors;

      String colorString = color_names[index1];
      colorString.concat(color_names[index2]);
      colorString.concat(color_names[index3]);
      return colorString;
    }
};