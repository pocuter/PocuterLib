#ifndef POCUTERLIBCONFIG_H
#define POCUTERLIBCONFIG_H
/*
 * Pocuter Lib Configuration
 * 
 */
 

/* Dont use Display functions. This includes uGUI */
//#define POCUTER_DISABLE_DISPLAY

/* Dont use SD-Card functions. This includes OTA */
// #define POCUTER_DISABLE_SD_CARD

/* Don't use Server */
//#define POCUTER_DISABLE_SERVER

/* Dont use WIFI functions */
//#define POCUTER_DISABLE_WIFI

/* Don't include the RGBled function */
//#define POCUTER_DISABLE_RGBled

/* Don't include the Buttons function */
// #define POCUTER_DISABLE_BUTTONS

/* Don't include the port expander. This will also disable RGBled, OTA, SD-Card, Buttons and Display */
// #define POCUTER_DISABLE_EXPANDER

/* Don't include the accelerator sensor */
//#define POCUTER_DISABLE_ACC

/* Swap SDA and SCL at the I2C bus. This will deactivate the expander and with it RGBled, OTA, SD-Card, Buttons and Display */
// #define POCUTER_SWITCH_SDA_SCL

/* Disable Light Sensor */
//#define POCUTER_DISABLE_LIGHTSENSOR

/* Disable the Microphone */
//#define POCUTER_DISABLE_MICROPHONE

/* Don't include UGUI */
//#define POCUTER_DISABLE_UGUI


/* Disable ADC  (Will also disable the Micrphone and light sensor) */
//#define POCUTER_DISABLE_ADC

/* Disable Pause Menu (used in PocketStar, disable in Pocuter) */
//#define POCUTER_DISABLE_PAUSE_MENU

//#define POCUTER_DISABLE_PORTS

#define POCUTER_EXTENDED_BUTTONS

/* Please do not change anything from this point */


/*#ifndef POCUTER_SWITCH_SDA_SCL

#ifndef POCUTER_DISABLE_ACC
#define POCUTER_DISABLE_ACC
#endif

#endif*/

#ifdef POCUTER_DISABLE_ADC
#define POCUTER_DISABLE_MICROPHONE
#define POCUTER_DISABLE_LIGHTSENSOR
#endif

#ifdef POCUTER_SWITCH_SDA_SCL

#ifndef POCUTER_DISABLE_EXPANDER
#define POCUTER_DISABLE_EXPANDER
#endif

#endif /* POCUTER_SWITCH_SDA_SCL */

#ifdef POCUTER_DISABLE_SD_CARD
#define POCUTER_DISABLE_SERVER
#endif


#ifdef POCUTER_DISABLE_EXPANDER

#ifndef POCUTER_DISABLE_DISPLAY
#define POCUTER_DISABLE_DISPLAY
#endif

#ifndef POCUTER_DISABLE_SD_CARD
#define POCUTER_DISABLE_SD_CARD
#endif

#ifndef POCUTER_DISABLE_RGBled
#define POCUTER_DISABLE_RGBled
#endif

#ifndef POCUTER_DISABLE_BUTTONS
#define POCUTER_DISABLE_BUTTONS
#endif

#endif /* POCUTER_DISABLE_EXPANDER */


#endif /* POCUTERLIBCONFIG_H */

