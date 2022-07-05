
#include "Pocuter.h"
#include <stdio.h>

#ifndef POCUTER_DISABLE_RGBled
#include "include/hal/esp32-c3/esp32_c3_RGBled.h"
#endif

#ifndef POCUTER_DISABLE_DISPLAY
#include "include/hal/esp32-c3/SSD1131_Display.h"
#endif

#ifndef POCUTER_DISABLE_BUTTONS
#include "include/hal/esp32-c3/esp32_c3_Buttons.h"
#endif

#include "include/hal/esp32-c3/esp32_c3_hmac.h"

#ifndef POCUTER_DISABLE_SD_CARD
#include "include/hal/esp32-c3/esp32_c3_SDCard.h"
#include "include/hal/esp32-c3/esp32_c3_OTA.h"
#endif

#ifndef POCUTER_DISABLE_WIFI  
#include "include/hal/esp32-c3/esp32_c3_WIFI.h"
#endif

#ifndef POCUTER_DISABLE_HTTP
#include "include/hal/esp32-c3/esp32_c3_HTTP.h"
#endif

#include "include/hal/esp32-c3/esp32_c3_I2C.h"

#ifndef POCUTER_DISABLE_ACC
#include "include/hal/esp32-c3/MXC4005XC_Accelerometer.h"
#endif

#ifndef POCUTER_DISABLE_LIGHTSENSOR
#include "include/hal/esp32-c3/esp32_c3_LightSensor.h"
#endif

#ifndef POCUTER_DISABLE_MICROPHONE
#include "include/hal/esp32-c3/esp32_c3_Mic.h"
#endif

#ifndef POCUTER_DISABLE_ADC
#include "include/hal/esp32-c3/esp32_c3_ADC.h"
#endif

#ifndef POCUTER_DISABLE_TIME
#include "include/hal/esp32-c3/esp32_c3_Time.h"
#endif

#ifndef POCUTER_DISABLE_SERVER
#include "include/hal/PocuterServer.h"
#endif



#include <string.h>


#define VERSION "1.0"
using namespace PocuterLib::HAL;

#ifndef POCUTER_DISABLE_RGBled 
RGBled* Pocuter::RGBLed = NULL;
#endif

#ifndef POCUTER_DISABLE_LIGHTSENSOR 
PocuterLightSensor* Pocuter::LightSensor = NULL;
#endif

PocuterI2C* Pocuter::I2C = NULL;

#ifndef POCUTER_DISABLE_DISPLAY  
PocuterDisplay* Pocuter::Display = NULL; 
UGUI* Pocuter::ugui = NULL; 
UG_GUI Pocuter::uGUI;
#endif

#ifndef POCUTER_DISABLE_BUTTONS
PocuterButtons* Pocuter::Buttons = NULL;
#endif


PocuterHMAC* Pocuter::HMAC = NULL;

#ifndef POCUTER_DISABLE_SD_CARD  
PocuterSDCard* Pocuter::SDCard = NULL;
PocuterOTA* Pocuter::OTA = NULL;
#endif

#ifndef POCUTER_DISABLE_WIFI 
PocuterWIFI* Pocuter::WIFI = NULL;
#endif

#ifndef POCUTER_DISABLE_HTTP
PocuterHTTP* Pocuter::HTTP = NULL;
#endif
#ifndef POCUTER_DISABLE_ACC   
PocuterAccelerometer* Pocuter::Accelerometer = NULL;
#endif

#ifndef POCUTER_DISABLE_MICROPHONE
PocuterMicrophone* Pocuter::Microphone = NULL;
#endif

#ifndef POCUTER_DISABLE_TIME
PocuterTime* Pocuter::PocTime = NULL;
#endif

#ifndef POCUTER_DISABLE_SERVER
PocuterServer* Pocuter::Server = NULL;
#endif

Pocuter::Pocuter() {
  
    
}
void Pocuter::begin(PocuterDisplay::BUFFER_MODE bm) {
   I2C = new esp32_c3_I2C(0);
   
#ifndef POCUTER_DISABLE_EXPANDER    
   esp32_c3_Expander::Instance()->registerI2Cbus(I2C);
#endif   
   
#ifndef POCUTER_DISABLE_DISPLAY   
   Display = new SSD1131_Display(bm);
#endif
   
#ifndef POCUTER_DISABLE_RGBled  
   RGBLed = new esp32_c3_RGBled();
#endif
   
 
#ifndef POCUTER_DISABLE_BUTTONS   
   Buttons = new esp32_c3_Buttons();
#endif
   
   HMAC = new esp32_c3_hmac();
   
#ifndef POCUTER_DISABLE_SD_CARD     
   SDCard = new esp32_c3_SDCard();
   
   OTA = new esp32_c3_OTA(SDCard);
#endif  
   
#ifndef POCUTER_DISABLE_WIFI 
   WIFI = new esp32_c3_WIFI();
   WIFI->connect();
#endif
#ifndef POCUTER_DISABLE_HTTP
   HTTP = new esp32_c3_HTTP();
#endif

#ifndef POCUTER_DISABLE_ACC   
   Accelerometer = new MXC4005XC_Accelerometer(I2C);
#endif
#ifndef POCUTER_DISABLE_LIGHTSENSOR 
    LightSensor = new esp32_c3_LightSensor();
#endif
    
#ifndef POCUTER_DISABLE_MICROPHONE
    Microphone = new esp32_c3_Mic();
#endif
#ifndef POCUTER_DISABLE_TIME
    PocTime = new esp32_c3_Time();
#endif
#ifndef POCUTER_DISABLE_SERVER
    Server = new PocuterServer(HMAC, OTA, HTTP);
#endif
    

#ifndef POCUTER_DISABLE_DISPLAY       
   uint16_t sizeX;
   uint16_t sizeY;
   Display->getDisplaySize(sizeX, sizeY);
   ugui = new UGUI(&uGUI);
   ugui->UG_Init(&uGUI, &driver_pixelSet, sizeX, sizeY);
   ugui->UG_DriverRegister(DRIVER_FILL_FRAME,  (void*)&Pocuter::driver_fillFrame);
   ugui->UG_DriverRegister(DRIVER_DRAW_LINE,  (void*)&Pocuter::driver_drawLine);
   ugui->UG_DriverRegister(DRIVER_FILL_AREA,  (void*)&Pocuter::driver_fillFrame);
   ugui->UG_DriverRegister(DRIVER_DRAW_SCANLINE,  (void*)&Pocuter::driver_drawScanLine);
#endif     
 
    
}

Pocuter::~Pocuter() {

}

#ifndef POCUTER_DISABLE_RGBled 
int Pocuter::setStatusLED(uint8_t r, uint8_t g, uint8_t b) {
    return RGBLed->setRGB(0,r,g,b);
    
}
#endif

#ifndef POCUTER_DISABLE_DISPLAY  
void Pocuter::driver_pixelSet(UG_S16 x,UG_S16 y,UG_COLOR c) {
   Display->setPixel(x,y,c);
  
}
int8_t Pocuter::driver_fillFrame(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c) {
    Display->drawRectangle(x1, y1, x2, y2, c);
    return UG_RESULT_OK;
}
int8_t Pocuter::driver_drawLine(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c) {
    Display->drawLine(x1,y1,x2,y2,c);
    return UG_RESULT_OK;
}
int8_t Pocuter::driver_drawScanLine(UG_S16 x, UG_S16 y, UG_S16 width, UG_COLOR* c) {
    Display->drawScanLine(x,y,width,c);
    return UG_RESULT_OK;
}
#endif