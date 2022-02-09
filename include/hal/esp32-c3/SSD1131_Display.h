#ifndef SSD1131_DISPLAY_H
#define SSD1131_DISPLAY_H
#include <stdint.h>
#include "esp32_c3_SPI.h"
#include "esp32_c3_Expander.h"
#include "include/hal/PocuterDisplay.h"



namespace PocuterLib {
    namespace HAL {
        class SSD1131_Display : public PocuterDisplay {
            public:
                SSD1131_Display();
                virtual ~SSD1131_Display();
                
                void getDisplaySize(uint16_t& sizeX, uint16_t& sizeY);
                void setPixel(uint16_t x, uint16_t y, uint32_t color);
                void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color);
                void drawScanLine(uint16_t x, uint16_t y, uint16_t width, uint32_t* colors);
                void drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color);
                
                
                void setBrightness(uint8_t brightness);
                void clearWindow(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
                void clearScreen();
                
                
                
            private:
                esp32_c3_SPI* m_spi;
                esp32_c3_Expander* m_expander;
                void reset();
        };
    }

}

#endif /* SSD1131_DISPLAY_H */

