#ifndef POCUTERDISPLAY_H
#define POCUTERDISPLAY_H
#include <stdint.h>

class PocuterDisplay {
    public:
        enum BUFFER_MODE {
            BUFFER_MODE_NO_BUFFER,
            BUFFER_MODE_DOUBLE_BUFFER

        };
                
        virtual void getDisplaySize(uint16_t& sizeX, uint16_t& sizeY) = 0;
        virtual void setPixel(uint16_t x, uint16_t y, uint32_t color) = 0;
        virtual void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color) = 0;
        virtual void drawScanLine(uint16_t x, uint16_t y, uint16_t width, uint32_t* colors) = 0;
        virtual void draw16BitScanLine(uint16_t x, uint16_t y, uint16_t width, uint16_t* colors) = 0;
        virtual void drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color) = 0;
        virtual void setBrightness(uint8_t brightness) = 0;
        virtual void clearWindow(uint8_t x, uint8_t y, uint8_t width, uint8_t height) = 0;
        virtual void clearScreen() = 0;
        virtual void continuousScreenUpdate(bool on) = 0;
        virtual BUFFER_MODE getBufferMode() = 0;
        virtual void updateScreen() = 0;
    private:

};

#endif /* POCUTERDISPLAY_H */

