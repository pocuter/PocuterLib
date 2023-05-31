#ifndef _PST_GFX_
#define _PST_GFX_

////////////////////////////////////////////////////////////////////////////////
// Macros:                                                                    //
////////////////////////////////////////////////////////////////////////////////

#ifndef NULL
   #define NULL ((void*) 0)
#endif

// screen dimension
#define SCREEN_WIDTH    96
#define SCREEN_HEIGHT   64

// opaque / translucency helpers
#define PSCOLOR_RGB_MASK_OPAQUE         0xFFDF
#define PSCOLOR_RGB_MASK_TRANSLUCENT    0xF79E
#define PSCOLOR_OPAQUE_FLAG             0x0020
#define PSCOLOR_TRANSPARENT_MASK        0x0861
#define PSCOLOR_TRANSLUCENCY_MASK       0x0841
#define PSCOLOR_TRANSLUCENCY_0_8TH      0x0000
#define PSCOLOR_TRANSLUCENCY_1_8TH      0x0001
#define PSCOLOR_TRANSLUCENCY_2_8TH      0x0040
#define PSCOLOR_TRANSLUCENCY_3_8TH      0x0041
#define PSCOLOR_TRANSLUCENCY_4_8TH      0x0800
#define PSCOLOR_TRANSLUCENCY_5_8TH      0x0801
#define PSCOLOR_TRANSLUCENCY_6_8TH      0x0840
#define PSCOLOR_TRANSLUCENCY_7_8TH      0x0841
#define PSCOLOR_TRANSLUCENCY_8_8TH      PSCOLOR_OPAQUE_FLAG
#define PSC_2(c)    (((c) & 0xF79E) >> 1)
#define PSC_4(c)    (((c) & 0xE71C) >> 2)
#define PSC_8(c)    (((c) & 0xC618) >> 3)
#define isOpaque(c)         (  (c) & PSCOLOR_OPAQUE_FLAG)
#define isTransparent(c)    (!((c) & PSCOLOR_TRANSPARENT_MASK))

// color creation
#define RGB(r,g,b) ((((r) & 0x1F) << 11) | (((g) & 0x1F) << 6) | ((b) & 0x1F) | PSCOLOR_OPAQUE_FLAG)
#define RGBA(r,g,b,a) ((((r) & 0xF) << 12) | (((g) & 0x1F) << 6) | ((b) & 0x1F) | (a))

// sprite flags
#define SPRITE_FLAG_FLIP_H   0x1
#define SPRITE_FLAG_FLIP_V   0x2
#define SPRITE_FLAG_FLIP_D   0x4

// colors

#define GFX_RED           0xF820
#define GFX_ORANGE        0xFC20
#define GFX_YELLOW        0xFFE0
#define GFX_GREEN         0x07E0
#define GFX_CYAN          0x07FF
#define GFX_BLUE          0x003F
#define GFX_MAGENTA       0xF83F
#define GFX_BLACK         0x0020
#define GFX_GRAY          0x8430
#define GFX_WHITE         0xFFFF


#define GFX_TRANSPARENT   0x0000
////////////////////////////////////////////////////////////////////////////////
// Types:                                                                     //
////////////////////////////////////////////////////////////////////////////////

typedef uint16_t color_t;

struct Bitmap {
    const color_t *data;
    uint8_t width, height;
    uint8_t anchorX, anchorY;
};

struct Font {
    uint8_t firstChar, lastChar;
    uint8_t fontHeight, charHeight, stride;
    uint8_t bpp;
    const uint8_t *charWidths;
    const uint16_t *charOffsets;
    const uint8_t *data;
};

enum TextAlign : uint8_t {
    ALIGN_LEFT = 0,
    ALIGN_CENTER,
    ALIGN_RIGHT,
};
class PocuterDisplay;
class PocuterButtons;

class GFX {
  private:
    PocuterDisplay *pDisplay;
    PocuterButtons *pButtons;
    color_t *colorBuffer; //[SCREEN_WIDTH*SCREEN_HEIGHT];
    color_t fontColors[2];
    
  public:
    void Init(PocuterDisplay *display, PocuterButtons *buttons);
    void UpdateDisplay();
    
    color_t RGB2Color(uint8_t r, uint8_t g, uint8_t b);
    color_t RGBA2Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void DrawPixel(int16_t x, int16_t y, color_t c);
    void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, color_t c);
    void DrawRect(int16_t x, int16_t y, uint16_t w, uint16_t h, color_t c);
    void FillRect(int16_t x, int16_t y, uint16_t w, uint16_t h, color_t c);
    void DrawSprite(const Bitmap *bitmap, int x, int y);
    void DrawSprite(int x, int y, const Bitmap *bitmap);
    void DrawSprite(int x, int y, uint8_t flipFlags, const Bitmap *bitmap);
    void SetFontColors(color_t col0, color_t col1);
    int StringWidth(const char* str, const Font *font);
    int DrawString(const char* str, int x, int y, TextAlign align, const Font *font);
    int DrawChar(char c, int x, int y, const Font *font);
    
    color_t GetPixel(int16_t x, int16_t y);
    
  private:
    struct Keyboard {
        char *charBuffer;
        uint8_t charBufferLen;
        bool canBeCanceled;
        uint8_t xPos, yPos;
        color_t colFont, colBackground, colGrid, colSelectFont, colSelectBG, colSelectGrid;
        
        uint8_t cursorX, cursorY;
        bool isDone, isCanceled;
    };
    Keyboard keyboard;
    const char *keyboardChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_ee"; // e is placeholder for OK
    
  public:
    void KeyboardSetup(char *buf, uint8_t len, bool canBeCanceled, uint8_t xPos, uint8_t yPos);
    void KeyboardColor(color_t font, color_t background, color_t grid, color_t selectFont, color_t selectBG, color_t selectGrid);
    void KeyboardUpdate();
    void KeyboardDraw();
    bool KeyboardIsDone();
    bool KeyboardIsCanceled();
};

#include "fonts.h"

#endif /* _PST_GFX_ */