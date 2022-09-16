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
#include "include/hal/PocuterHTTP.h"
#include "include/hal/PocuterTime.h"
#include "include/hal/PocuterServer.h"
#include "include/hal/PocuterPorts.h"

class Pocuter {
public:
    enum POCUTER_RESULT {
        POCUTER_RESULT_OK = 0,
        POCUTER_RESULT_FAILED
    };
    
    /**
    * @brief Pocuter constructor
    * 
    * @note you have to call begin() after this
    * 
    * @return
    *     - the pocuter class
    */
    Pocuter();
    
    /**
    * @brief Pocuter destructor
    * 
    */
    
    virtual ~Pocuter();

    /**
    * @brief initializes your Pocuter
    *
    * @note BUFFER_MODE_NO_BUFFER saves memory, but can lead to flickering of the display in certain applications
    * 
    * @param bm (optional) the buffer mode for the Pocuter. 
    *
    */
    void begin(PocuterDisplay::BUFFER_MODE bm = PocuterDisplay::BUFFER_MODE_NO_BUFFER);
    
#ifndef POCUTER_DISABLE_RGBled    
    /**
    * @brief Setting Status LED
    *
    * @param r value
    * @param g value
    * @param b value
    *
    * @return
    *     - LEDERROR_OK Success
    *     - LEDERROR_FAILURE failed to set RGB
    */
    int setStatusLED(uint8_t r, uint8_t g, uint8_t b);
    
   
    static RGBled* RGBLed;                      /*!< RGBLed Class Acccess */
    
#endif
    static PocuterI2C* I2C;                     /*!< PocuterI2C Class Acccess */

#ifndef POCUTER_DISABLE_DISPLAY      
    static PocuterDisplay* Display;             /*!< PocuterDisplay Class Acccess */
    static UGUI* ugui;                          /*!< UGUI Class Acccess */
#endif 
#ifndef POCUTER_DISABLE_LIGHTSENSOR 
    static PocuterLightSensor* LightSensor;     /*!< PocuterLightSensor Class Acccess */
#endif
#ifndef POCUTER_DISABLE_BUTTONS    
    static PocuterButtons* Buttons;             /*!< PocuterButtons Class Acccess */
#endif
#ifndef POCUTER_DISABLE_PORTS    
    static PocuterPorts* Ports  ;             /*!< PocuterPorts Class Acccess */
#endif   
    static PocuterHMAC* HMAC;                   /*!< PocuterHMAC Class Acccess */
    
#ifndef POCUTER_DISABLE_SD_CARD      
    static PocuterSDCard* SDCard;               /*!< PocuterSDCard Class Acccess */
    static PocuterOTA* OTA;                     /*!< PocuterOTA Class Acccess */
#endif  
    
#ifndef POCUTER_DISABLE_WIFI  
    static PocuterWIFI* WIFI;                   /*!< PocuterWIFI Class Acccess */
#endif  

#ifndef POCUTER_DISABLE_HTTP
    static PocuterHTTP* HTTP;                   /*!< PocuterHTTP Class Acccess */
#endif 
    
#ifndef POCUTER_DISABLE_MICROPHONE    
    static PocuterMicrophone* Microphone;       /*!< PocuterMicrophone Class Acccess */
#endif

#ifndef POCUTER_DISABLE_TIME
    static PocuterTime* PocTime;                /*!< PocuterTime Class Acccess */
#endif

#ifndef POCUTER_DISABLE_SERVER
    static PocuterServer* Server;               /*!< PocuterServer Class Acccess */
#endif
    
#ifndef POCUTER_DISABLE_ACC       
    static PocuterAccelerometer* Accelerometer; /*!< PocuterAccelerometer Class Acccess */
#endif
    
private:
    
   
    static UG_GUI uGUI;
    
    
    static void driver_pixelSet(UG_S16,UG_S16,UG_COLOR);
    static int8_t driver_drawLine(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c);
    static int8_t driver_fillFrame(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c);
    static int8_t driver_drawScanLine(UG_S16 x, UG_S16 y, UG_S16 width, UG_COLOR* c);
    
};

#endif /* POCUTER_H */

