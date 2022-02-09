/* -------------------------------------------------------------------------------- */
/* -- µGUI - Generic GUI module (C)Achim Döbler, 2015                            -- */
/* -------------------------------------------------------------------------------- */
// µGUI is a generic GUI module for embedded systems.
// This is a free software that is open for education, research and commercial
// developments under license policy of following terms.
//
//  Copyright (C) 2015, Achim Döbler, all rights reserved.
//  URL: http://www.embeddedlightning.com/
//
// * The µGUI module is a free software and there is NO WARRANTY.
// * No restriction on use. You can use, modify and redistribute it for
//   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
// * Redistributions of source code must retain the above copyright notice.
//
/* -------------------------------------------------------------------------------- */

#ifndef __UGUI_CONFIG_H
#define __UGUI_CONFIG_H

#include <stdint.h>
#include <stdlib.h>

/* -------------------------------------------------------------------------------- */
/* -- CONFIG SECTION                                                             -- */
/* -------------------------------------------------------------------------------- */


#define USE_STATIC_MEMORY
#define USE_IMAGE
/* Enable color mode */
//#define USE_COLOR_RGB565   // RGB = 0bRRRRRGGGGGGBBBBB

#ifndef USE_COLOR_RGB565
#define USE_COLOR_RGB888  1 // RGB = 0xFF,0xFF,0xFF
#endif

/* Feature enablers */
//#define USE_PRERENDER_EVENT
//#define USE_POSTRENDER_EVENT
//#define USE_PREUPDATE_EVENT
//#define USE_POSTUPDATE_EVENT

/* Enable needed fonts here */
#define  USE_FONT_4X6
#define  USE_FONT_5X8
//#define  USE_FONT_5X12
//#define  USE_FONT_6X8
//#define  USE_FONT_6X10
//#define  USE_FONT_7X12
//#define  USE_FONT_8X8
//#define  USE_FONT_8X12_CYRILLIC
//#define  USE_FONT_8X12
//#define  USE_FONT_8X12
//#define  USE_FONT_8X14
//#define  USE_FONT_10X16
//#define  USE_FONT_12X16
//#define  USE_FONT_12X20
//#define  USE_FONT_16X26
//#define  USE_FONT_22X36
//#define  USE_FONT_24X40
//#define  USE_FONT_32X53

typedef uint8_t      UG_U8;
typedef int8_t       UG_S8;
typedef uint16_t     UG_U16;
typedef int16_t      UG_S16;
typedef uint32_t     UG_U32;
typedef int32_t      UG_S32;

#define USE_INTERNAL_FONTS              // use fonts from ugui library


/* Specify platform-dependent integer types here */


#define __UG_FONT_DATA const
#define FONT_DATA_ACCESS(x) (x)

/* -------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------- */
/* -- µGUI FONTS                                                                 -- */
/* -- Source: http://www.mikrocontroller.net/user/show/benedikt                  -- */
/* -------------------------------------------------------------------------------- */
typedef enum
{
	FONT_TYPE_1BPP,
	FONT_TYPE_8BPP
} FONT_TYPE;

typedef struct
{
   unsigned char* p;
   FONT_TYPE font_type;
   UG_S16 char_width;
   UG_S16 char_height;
   UG_U16 start_char;
   UG_U16 end_char;
   UG_U8  *widths;
} UG_FONT;

/* -------------------------------------------------------------------------------- */
// the following moved to UGUI.h

/* Supported drivers 
#define NUMBER_OF_DRIVERS                             13
#define DRIVER_DRAW_LINE                              0
#define DRIVER_FILL_FRAME                             1
#define DRIVER_FILL_AREA                              2
#define DRIVER_FILL_ROUND_FRAME                       4
#define DRIVER_DRAW_FRAME                             5
#define DRIVER_DRAW_ROUND_FRAME                       6
#define DRIVER_DRAW_CIRCLE                            7
#define DRIVER_FILL_CIRCLE                            8
#define DRIVER_FILL_SCREEN                            9
#define DRIVER_DRAW_ARC                               10
#define DRIVER_PUT_CHAR                               11
#define DRIVER_PUT_STRING                             12
*/
#endif
