#ifndef POCUTERDISPLAY_H
#define POCUTERDISPLAY_H
#include <stdint.h>
/**
* @brief this class has basic display operations only. If you want to write a GUI, consider to use ugui instead.
*  
*/
class PocuterDisplay {
    public:
        enum BUFFER_MODE {
            BUFFER_MODE_NO_BUFFER,
            BUFFER_MODE_DOUBLE_BUFFER

        };
        /**
        * @brief get the Display size
        * 
        * @param sizeX output
        * @param sizeY output
        * 
        */       
        virtual void getDisplaySize(uint16_t& sizeX, uint16_t& sizeY) = 0;

        /**
        * @brief set a Pixel on the display
        * 
        * @param x x coordinate 
        * @param y y coordinate
        * @param color a 24 bit color value (RGB888)
        * 
        */     
        virtual void setPixel(int16_t x, int16_t y, uint32_t color) = 0;

        /**
        * @brief draw a line
        * 
        * @param x1 x start coordinate 
        * @param y1 y start coordinate
        * @param x2 x end coordinate 
        * @param y2 y end coordinate
        * @param color a 24 bit color value (RGB888)
        * 
        */             
        virtual void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color) = 0;

        /**
        * @brief draw a horizontal line
        *
        * @note this draws one horizontal line beginning at x. It is faster than drawing every pixel (especially in no buffer mode) or using drawLine
        * @note it will stop drawing if the line is longer than the display can show
        * 
        * @param x x start coordinate 
        * @param y y start coordinate
        * @param width number of pixels to draw
        * @param colors a 24 bit color array (RGB888)
        * 
        */             
        virtual void drawScanLine(int16_t x, int16_t y, uint16_t width, uint32_t* colors) = 0;
        
        /**
        * @brief draw a horizontal line with 16 bit color instead of 24 bit
        *
        * @note this draws one line beginning at x. It is even faster than drawScanLine
        * @note it will stop drawing if the line is longer than the display can show
        * 
        * @param x x start coordinate 
        * @param y y start coordinate
        * @param width number of pixels to draw
        * @param colors 16 bit color array (RGB565)
        * 
        */         
        virtual void draw16BitScanLine(int16_t x, int16_t y, uint16_t width, uint16_t* colors) = 0;
        
        
        /**
        * @brief draws a filled rectangle
        * 
        * @param x1 x start coordinate 
        * @param y1 y start coordinate
        * @param x2 x end coordinate 
        * @param y2 y end coordinate
        * @param color a 24 bit color value (RGB888)
        * 
        */        
        virtual void drawRectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color) = 0;
        
        /**
        * @brief set the display brightness
        * 
        * @param brightness 0 - 15
        */        
        virtual void setBrightness(uint8_t brightness) = 0;
        
        /**
        * @brief clear a rectangle
        * 
        * @note in NO BUFFER mode this is hardware accelerated, otherwise it is the same as drawRectangle with color 0
        * 
        * @param x x start coordinate 
        * @param y y start coordinate
        * @param width the width
        * @param height the height

        */              
        virtual void clearWindow(int16_t x, int16_t y, int16_t width, int16_t height) = 0;
        
        /**
        * @brief clear the hole screen
        * 
        * @note in NO BUFFER mode this is hardware accelerated, otherwise it is the same as drawRectangle on the hole screen with color 0
        */           
        virtual void clearScreen() = 0;
        
        
        /**
        * @brief automatic screen update
        * 
        * @note if you set this to false you have to call updateScreen every time you want to update the screen. This can prevent flickering. It is in available in double buffer mode only.
        * 
        * @param on switch automatic screen update on / off
        *
        */           
        virtual void continuousScreenUpdate(bool on) = 0;
        
        /**
        * @brief update the screen. Only available in DOUBLE BUFFER mode
        * 
        * @note this function copies the backbuffer to the frontbuffer (double buffering) and returns immediately. Display drawing is executed in another thread. If the frontbuffer is currently sending to the display, the function waits until this is done.
        * 
        *
        */          
        virtual void updateScreen() = 0;
        virtual void updateScreen(uint16_t *buffer) = 0;
        
        
        /**
        * @brief get the current buffer mode
        * 
        * 
        * @return BUFFER_MODE return the current buffer mode
        *
        */                
        virtual BUFFER_MODE getBufferMode() = 0;
        
        
        /**
        * @brief let the display sleep
        *
        */                
        virtual void doSleep(bool deep = false) = 0;
        
        /**
        * @brief wake up the display 
        * 
        * @param deep sets display into deep sleep
        */                
        virtual void doWakeUp() = 0;

        virtual uint16_t* getBackBuffer() = 0;
        
    private:

};

#endif /* POCUTERDISPLAY_H */

