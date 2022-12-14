#ifndef SSD1131_DISPLAY_H
#define SSD1131_DISPLAY_H
#include <stdint.h>
#include "esp32_c3_SPI.h"
#include "esp32_c3_Expander.h"
#include "include/hal/PocuterDisplay.h"
#include <freertos/semphr.h>



namespace PocuterLib {
    namespace HAL {
        class SSD1131_Display : public PocuterDisplay {
            public:
                
                SSD1131_Display(BUFFER_MODE bm = BUFFER_MODE_DOUBLE_BUFFER);
                virtual ~SSD1131_Display();
                
                void getDisplaySize(uint16_t& sizeX, uint16_t& sizeY);
                void setPixel(int16_t x, int16_t y, uint32_t color);
                void set16BitPixel(int16_t x, int16_t y, uint16_t color);
                void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color);
                void drawScanLine(int16_t x, int16_t y, uint16_t width, uint32_t* colors);
                void draw16BitScanLine(int16_t x, int16_t y, uint16_t width, uint16_t* colors);
                
                void drawRectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color);
                void continuousScreenUpdate(bool on);
                void updateScreen();
                
                void setBrightness(uint8_t brightness);
                void clearWindow(int16_t x, int16_t y, int16_t width, int16_t height);
                void clearScreen();
                
                void doSleep();
                void doWakeUp();
                
                BUFFER_MODE getBufferMode();
                
                
            private:
                esp32_c3_SPI* m_spi;
                esp32_c3_Expander* m_expander;
                void reset();

                uint16_t* m_currentBackBuffer;
                uint16_t* m_currentFrontBuffer;
                BUFFER_MODE m_bm;
                static void updateTask(void *arg);
                static bool g_runUpdateTask;
                static bool g_continuouseScreenUpdate;
               
                static SemaphoreHandle_t g_displaySemaphore;
                static SemaphoreHandle_t g_displayPauseSemaphore;
                static bool g_initializing;
                uint16_t color24to16(uint32_t color);

                bool m_continuouseScreenUpdateBeforeSleep;
                
               
        };
    }

}

#endif /* SSD1131_DISPLAY_H */

