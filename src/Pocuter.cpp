
#include "Pocuter.h"
#include <stdio.h>
#include "include/hal/esp32-c3/esp32_c3_RGBled.h"
#include "include/hal/esp32-c3/SSD1131_Display.h"
#include "include/hal/esp32-c3/esp32_c3_Buttons.h"
#include "include/hal/esp32-c3/esp32_c3_hmac.h"
#include "include/hal/esp32-c3/esp32_c3_SDCard.h"
#include "include/hal/esp32-c3/esp32_c3_OTA.h"
#include "include/hal/esp32-c3/esp32_c3_WIFI.h"
#include "include/hal/esp32-c3/esp32_c3_I2C.h"

#include <string.h>


#define VERSION "1.0"
using namespace PocuterLib::HAL;
RGBled* Pocuter::RGBLed = NULL; 
PocuterI2C* Pocuter::I2C = NULL;

#ifndef POCUTER_DISABLE_DISPLAY  
PocuterDisplay* Pocuter::Display = NULL; 
#endif

UGUI* Pocuter::ugui = NULL; 
UG_GUI Pocuter::uGUI;
PocuterButtons* Pocuter::Buttons = NULL;
PocuterHMAC* Pocuter::HMAC = NULL;

#ifndef POCUTER_DISABLE_SD_CARD  
PocuterSDCard* Pocuter::SDCard = NULL;
PocuterOTA* Pocuter::OTA = NULL;
#endif


PocuterWIFI* Pocuter::WIFI = NULL;

Pocuter::Pocuter() {
  
    
}
void Pocuter::begin() {
#ifndef POCUTER_DISABLE_DISPLAY   
   Display = new SSD1131_Display();
#endif
   
   I2C = new esp32_c3_I2C(0);
   esp32_c3_Expander::Instance()->registerI2Cbus(I2C);
   
   RGBLed = new esp32_c3_RGBled();
   RGBLed->setRGB(0,255,0,0);
   Buttons = new esp32_c3_Buttons();
   HMAC = new esp32_c3_hmac();
   RGBLed->setRGB(0,0,255,0);
#ifndef POCUTER_DISABLE_SD_CARD     
   SDCard = new esp32_c3_SDCard();
   RGBLed->setRGB(0,0,0,255);
   OTA = new esp32_c3_OTA(SDCard);
#endif   
   WIFI = new esp32_c3_WIFI();
   RGBLed->setRGB(0,255,255,255);
   
#ifndef POCUTER_DISABLE_SD_CARD
   if (OTA->getCurrentPartition() != PocuterOTA::PART_MENUE) {
       OTA->bootPartition(PocuterOTA::PART_MENUE);
   }
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
    delete RGBLed;
}

int Pocuter::setStatusLED(uint8_t r, uint8_t g, uint8_t b) {
    return RGBLed->setRGB(0,r,g,b);
    
}
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