
#include "Pocuter.h"
#include <stdio.h>

#include "include/hal/esp32-c3/esp32_c3_DeviceTypeDetection.h"

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

#ifndef POCUTER_DISABLE_HIGH_SCORES
#include "include/hal/PocketStarHighScores.h"
#endif

#ifndef POCUTER_DISABLE_PORTS
#include "include/hal/esp32-c3/esp32_c3_Ports.h"
#endif

#ifndef POCUTER_DISABLE_SLEEP
#include "include/hal/esp32-c3/esp32_c3_sleep.h"
#endif

#ifndef POCUTER_DISABLE_SOUND
#include "include/hal/esp32-c3/esp32_c3_Sound.h"
#endif



#include <string.h>

PocuterDeviceType::DEVICE_TYPES PocuterDeviceType::deviceType = PocuterDeviceType::DEVICE_TYPE_UNKNOWN;


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
GFX* Pocuter::gfx = NULL; 

#ifndef POCUTER_DISABLE_UGUI
UGUI* Pocuter::ugui = NULL; 
UG_GUI Pocuter::uGUI;
#endif

#endif

#ifndef POCUTER_DISABLE_PORTS
PocuterPorts* Pocuter::Ports = NULL; 
#endif

#ifndef POCUTER_DISABLE_SLEEP
PocuterSleep* Pocuter::Sleep = NULL; 
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

#ifndef POCUTER_DISABLE_HIGH_SCORES
PocketStarHighScores* Pocuter::HighScores = NULL;
#endif
#ifndef POCUTER_DISABLE_SOUND
PocuterSound* Pocuter::Sound = NULL; 
#endif 


#ifndef POCUTER_DISABLE_PAUSE_MENU
#include "PocketStarPause.h"
#endif

Pocuter::Pocuter() {
  
    
}
void Pocuter::begin(PocuterDisplay::BUFFER_MODE bm) {
    // get device type
    esp32_c3_DeviceTypeDetection* dt = new esp32_c3_DeviceTypeDetection();
    PocuterDeviceType::deviceType = dt->getDeviceType();
    delete(dt);
    
    
    
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
 #ifndef POCUTER_DISABLE_PORTS
   Ports = new esp32_c3_Ports();
#endif
#ifndef POCUTER_DISABLE_ACC   
   Accelerometer = new MXC4005XC_Accelerometer(I2C);
#endif

#ifndef POCUTER_DISABLE_SLEEP
    #ifndef POCUTER_DISABLE_PORTS
       #ifndef POCUTER_DISABLE_DISPLAY  
            #ifndef POCUTER_DISABLE_ACC 
                Sleep = new esp32_c3_sleep(Display, Ports, Accelerometer);
            #else
                Sleep = new esp32_c3_sleep(Display, Ports);
            #endif
       #else
           Sleep = new esp32_c3_sleep(NULL, Ports);
       #endif
    #else
       #ifndef POCUTER_DISABLE_DISPLAY  
            Sleep = new esp32_c3_sleep(Display, NULL, NULL);
       #else
           Sleep = new esp32_c3_sleep(NULL, NULL);
       #endif
    #endif
   
#endif

       
       
    HMAC = new esp32_c3_hmac();
   
#ifndef POCUTER_DISABLE_SD_CARD     
   SDCard = new esp32_c3_SDCard();
   
   OTA = new esp32_c3_OTA(SDCard, HMAC);
#endif  
   
#ifndef POCUTER_DISABLE_WIFI 
   WIFI = new esp32_c3_WIFI();
   WIFI->connect();
#endif
#ifndef POCUTER_DISABLE_HTTP
   HTTP = new esp32_c3_HTTP();
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
#ifndef POCUTER_DISABLE_HIGH_SCORES
    HighScores = new PocketStarHighScores(HMAC, OTA, HTTP);
#endif
#ifndef POCUTER_DISABLE_SOUND
    Sound = new esp32_c3_Sound(); 
#endif 

#ifndef POCUTER_DISABLE_DISPLAY       
   //uint16_t sizeX;
   //uint16_t sizeY;
   //Display->getDisplaySize(sizeX, sizeY);
   gfx = new GFX();
   gfx->Init(Display, Buttons);
   
   #ifndef POCUTER_DISABLE_PAUSE_MENU
    m_pauseMenu = new PocketStarPauseMenu(this);
   #endif
   
   
   
   
#ifndef POCUTER_DISABLE_UGUI   
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
   
#endif     
#ifndef POCUTER_DISABLE_SLEEP
#ifndef POCUTER_DISABLE_SD_CARD  
   //set sleep standard
    uint64_t appId = 1;
    PocuterConfig* config = new PocuterConfig((const uint8_t*)"INACT_SLEEP", &appId);
    uint32_t sec = config->get((const uint8_t*)"SLEEP", (const uint8_t*)"timeout");
    delete(config);
    if (sec == 0) sec = 30;
    if(PocuterDeviceType::deviceType == PocuterDeviceType::DEVICE_TYPE_POCUTER_1) {   
        Sleep->setSleepMode(PocuterSleep::SLEEP_MODE_LIGHT);
        Sleep->setWakeUpModes(PocuterSleep::WAKEUP_MODE_ANY_BUTTON | PocuterSleep::WAKEUP_MODE_SHAKE); 
        Sleep->setInactivitySleep(sec, PocuterSleep::SLEEPTIMER_INTERRUPT_BY_BUTTON);
    } else {
        Sleep->setSleepMode(PocuterSleep::SLEEP_MODE_DEEP);
        Sleep->setInactivitySleep(sec, PocuterSleep::SLEEPTIMER_INTERRUPT_BY_BUTTON);
    }
#endif    
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
#ifndef POCUTER_DISABLE_UGUI
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
#endif
