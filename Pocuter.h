#ifndef POCUTER_H
#define POCUTER_H

/*
 * Pocuter Lib 
 * 
 * Copyright (C) 2022 Pocuter GmbH  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */


#include "include/PocuterLibConfig.h"

#include <stdint.h>
#include "include/hal/RGBled.h"
#include "include/hal/PocuterDisplay.h"
#include "include/ugui/UGUI.h"
#include "include/hal/PocuterButtons.h"
#include "include/hal/PocuterHMAC.h"
#include "include/hal/PocuterSDCard.h"
#include "include/hal/PocuterConfig.h"
#include "include/hal/PocuterOTA.h"

#ifndef POCUTER_DISABLE_WIFI  
#include "include/hal/PocuterWIFI.h"
#endif
#include "include/hal/PocuterI2C.h"
#include "include/hal/PocuterAccelerometer.h"
#include "include/hal/PocuterLightSensor.h"
#include "include/hal/PocuterMicrophone.h"


class Pocuter {
public:
    enum POCUTER_RESULT {
        POCUTER_RESULT_OK = 0,
        POCUTER_RESULT_FAILED
    };
    
    Pocuter();
    virtual ~Pocuter();
    void begin(PocuterDisplay::BUFFER_MODE bm = PocuterDisplay::BUFFER_MODE_NO_BUFFER);
    
#ifndef POCUTER_DISABLE_RGBled    
    int setStatusLED(uint8_t r, uint8_t g, uint8_t b);
    static RGBled* RGBLed;
#endif
    static PocuterI2C* I2C;

#ifndef POCUTER_DISABLE_DISPLAY      
    static PocuterDisplay* Display;
    static UGUI* ugui;
#endif
#ifndef POCUTER_DISABLE_LIGHTSENSOR 
    static PocuterLightSensor* LightSensor;
#endif
#ifndef POCUTER_DISABLE_BUTTONS    
    static PocuterButtons* Buttons;
#endif
    
    static PocuterHMAC* HMAC;
    
#ifndef POCUTER_DISABLE_SD_CARD      
    static PocuterSDCard* SDCard;
    static PocuterOTA* OTA;
#endif  
    
#ifndef POCUTER_DISABLE_WIFI  
    static PocuterWIFI* WIFI;
#endif  

#ifndef POCUTER_DISABLE_MICROPHONE    
    static PocuterMicrophone* Microphone;
#endif
    
#ifndef POCUTER_DISABLE_ACC       
    static PocuterAccelerometer* Accelerometer;
#endif
    
private:
    
   
    static UG_GUI uGUI;
    
    
    static void driver_pixelSet(UG_S16,UG_S16,UG_COLOR);
    static int8_t driver_drawLine(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c);
    static int8_t driver_fillFrame(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c);
    static int8_t driver_drawScanLine(UG_S16 x, UG_S16 y, UG_S16 width, UG_COLOR* c);
    
};

#endif /* POCUTER_H */

