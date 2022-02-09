#ifndef POCUTER_H
#define POCUTER_H

#include <stdint.h>
#include "include/hal/RGBled.h"
#include "include/hal/PocuterDisplay.h"
#include "include/ugui/UGUI.h"
#include "include/hal/PocuterButtons.h"
#include "include/hal/PocuterHMAC.h"
class Pocuter {
public:
    enum POCUTER_RESULT {
        POCUTER_RESULT_OK = 0,
        POCUTER_RESULT_FAILED
    };
    
    Pocuter();
    virtual ~Pocuter();
    void begin();
    int setStatusLED(uint8_t r, uint8_t g, uint8_t b);
    static RGBled* RGBLed;
    static PocuterDisplay* Display;
    static UGUI* ugui;
    static PocuterButtons* Buttons;
    static PocuterHMAC* HMAC;
    
private:
    
   
    static UG_GUI uGUI;
    
    
    static void driver_pixelSet(UG_S16,UG_S16,UG_COLOR);
    static int8_t driver_drawLine(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c);
    static int8_t driver_fillFrame(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c);
    static int8_t driver_drawScanLine(UG_S16 x, UG_S16 y, UG_S16 width, UG_COLOR* c);
    
};

#endif /* POCUTER_H */

