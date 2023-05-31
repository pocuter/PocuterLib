#ifndef POCUTER_DISABLE_DISPLAY   

#include <math.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "include/gfx/GFX.h"
#include "include/gfx/fonts.h"

#define swap16(val) ( (((val) >> 8) & 0x00FF) | (((val) << 8) & 0xFF00) )

void GFX::Init(PocuterDisplay *display, PocuterButtons *buttons) {
    pDisplay = display;
    pButtons = buttons;
    colorBuffer = pDisplay->getBackBuffer();
}

void GFX::UpdateDisplay() {
    
    pDisplay->updateScreen();
}

color_t GFX::RGB2Color(uint8_t r, uint8_t g, uint8_t b) {
    r = round(r*1.0f * 0x1F / 0xFF);
    g = round(g*1.0f * 0x1F / 0xFF);
    b = round(b*1.0f * 0x1F / 0xFF);
    return (r << 11) | (g << 6) | (b) | (0x20);
}

color_t GFX::RGBA2Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    a = round(a*1.0f * 0x8 / 0xFF);

    if (a == 0x8) {
        r = round(r*1.0f * 0x1F / 0xFF);
        g = round(g*1.0f * 0x1F / 0xFF);
        b = round(b*1.0f * 0x1F / 0xFF);
        return (r << 11) | (g << 6) | (b) | (0x20);
    } else {
        r = round(r*1.0f * 0xF / 0xFF);
        g = round(g*1.0f * 0xF / 0xFF);
        b = round(b*1.0f * 0xF / 0xFF);
        int x = (a >> 2) & 1;
        int y = (a >> 1) & 1;
        int z = (a     ) & 1;
        return (r << 12) | (x << 11) | (g << 7) | (y << 6) | (b << 1) | (z);
    }
}

void GFX::DrawPixel(int16_t x, int16_t y, color_t c) {
    if (x < 0 || y < 0 || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
        return;
    
    if (isTransparent(c))
        return;
    
    int index = x+y*SCREEN_WIDTH;
    
    if (isOpaque(c)) {
        colorBuffer[index] = swap16(c & PSCOLOR_RGB_MASK_OPAQUE);
        return;
    }
    
    color_t a = swap16(colorBuffer[index]), b = c & PSCOLOR_RGB_MASK_TRANSLUCENT, f = 0;
    
    switch (c & PSCOLOR_TRANSLUCENCY_MASK) {
        case PSCOLOR_TRANSLUCENCY_1_8TH: f = PSC_2(a) + PSC_4(a) + PSC_8(a) +                       PSC_8(b); break;
        case PSCOLOR_TRANSLUCENCY_2_8TH: f = PSC_2(a) + PSC_4(a) +          +            PSC_4(b)           ; break;
        case PSCOLOR_TRANSLUCENCY_3_8TH: f = PSC_2(a) +          + PSC_8(a) +          + PSC_4(b) + PSC_8(b); break;
        case PSCOLOR_TRANSLUCENCY_4_8TH: f = PSC_2(a) +                       PSC_2(b)                      ; break;
        case PSCOLOR_TRANSLUCENCY_5_8TH: f =          + PSC_4(a) + PSC_8(a) + PSC_2(b) +          + PSC_8(b); break;
        case PSCOLOR_TRANSLUCENCY_6_8TH: f =          + PSC_4(a) +          + PSC_2(b) + PSC_4(b)           ; break;
        case PSCOLOR_TRANSLUCENCY_7_8TH: f =          +          + PSC_8(a) + PSC_2(b) + PSC_4(b) + PSC_8(b); break;
    }
    
    colorBuffer[index] = swap16(f);
}

void GFX::DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, color_t c) {
    int dx =  abs(x1 - x0);
    int dy = -abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    
    int err = dx + dy, err2;

    while (true) {
        DrawPixel(x0, y0, c);
        
        if (x0 == x1 && y0 == y1)
            break;
        
        err2 = 2 * err;
        if (err2 > dy) {
            err += dy;
            x0 += sx;
        }
        if (err2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void GFX::DrawRect(int16_t x, int16_t y, uint16_t w, uint16_t h, color_t c) {
    int x2 = x+w-1, y2 = y+h-1;
    
    int xl = x  >= 0            ? x  : 0;
    int xr = x2 <= SCREEN_WIDTH ? x2 : SCREEN_WIDTH;
    
    for (int xx = xl; xx < xr+1; xx++) {
        DrawPixel(xx, y,  c);
        DrawPixel(xx, y2, c);
    }
    
    int yu = y  >= -1            ? y  : -1;
    int yd = y2 <= SCREEN_HEIGHT ? y2 : SCREEN_HEIGHT;
    
    for (int yy = yu+1; yy < yd; yy++) {
        DrawPixel(x,  yy, c);
        DrawPixel(x2, yy, c);
    }
}

void GFX::FillRect(int16_t x, int16_t y, uint16_t w, uint16_t h, color_t c) {
    int x2 = x+w, y2 = y+h;
    
    x  = x  >= 0             ? x : 0;
    y  = y  >= 0             ? y : 0;
    x2 = x2 <= SCREEN_WIDTH  ? x2 : SCREEN_WIDTH;
    y2 = y2 <= SCREEN_HEIGHT ? y2 : SCREEN_HEIGHT;
    
    for (int xx = x; xx < x2; xx++) {
        for (int yy = y; yy < y2; yy++) {
            DrawPixel(xx, yy, c);
        }
    }
}

void GFX::DrawSprite(const Bitmap *bitmap, int x, int y) {
    DrawSprite(x, y, bitmap);
}

void GFX::DrawSprite(int x, int y, const Bitmap *bitmap) {
    const color_t *px = bitmap->data;
    for (int yy = 0; yy < bitmap->height; yy++) {
        for (int xx = 0; xx < bitmap->width; xx++, px++) {
            DrawPixel(x+xx, y+yy, *px);
        }
    }
}

void GFX::DrawSprite(int x, int y, uint8_t flipFlags, const Bitmap *bitmap) {
    int w = bitmap->width;
    int h = bitmap->height;
    const color_t *px = bitmap->data;
    
    if (!(flipFlags & SPRITE_FLAG_FLIP_D)) {
        for (int iy = 0; iy < h; iy++) {
            for (int ix = 0; ix < w; ix++, px++) {
                int sx = x + ((!(flipFlags & SPRITE_FLAG_FLIP_H)) ? (ix - bitmap->anchorX) : (bitmap->anchorX - ix - 1));
                int sy = y + ((!(flipFlags & SPRITE_FLAG_FLIP_V)) ? (iy - bitmap->anchorY) : (bitmap->anchorY - iy - 1));
                if (sy < 0 || sy > 63 || sx < 0 || sx > 95)
                    continue;
                
                DrawPixel(sx, sy, *px);
            }
        }
    } else { // mirrorD
        int8_t p = x; x = y; y = p;
        
        for (int iy = 0; iy < h; iy++) {
            for (int ix = 0; ix < w; ix++, px++) {
                int sy = x + ((!(flipFlags & SPRITE_FLAG_FLIP_H)) ? (ix - bitmap->anchorX) : (bitmap->anchorX - ix - 1));
                int sx = y + ((!(flipFlags & SPRITE_FLAG_FLIP_V)) ? (iy - bitmap->anchorY) : (bitmap->anchorY - iy - 1));
                if (sy < 0 || sy > 63 || sx < 0 || sx > 95)
                    continue;
                
                DrawPixel(sx, sy, *px);
            }
        }
    }
}

void GFX::SetFontColors(color_t col0, color_t col1) {
    fontColors[0] = col0;
    fontColors[1] = col1;
}

int GFX::StringWidth(const char* str, const Font *font) {
    int width = 0, len;
    for (len = 0; str[len] != '\0'; len++) {
        int c = str[len];
        if (font->firstChar <= c && c <= font->lastChar)
            width += font->charWidths[c - font->firstChar];
    }
    return width + (len-1) * font->stride;
}

int GFX::DrawString(const char* str, int x, int y, TextAlign align, const Font *font) {
    if (align != ALIGN_LEFT) {
        int w = StringWidth(str, font);
        if (align == ALIGN_RIGHT)
            x -= w;
        if (align == ALIGN_CENTER)
            x -= w/2;
    }
    
    while (*str != '\0') {
        x += DrawChar(*str, x, y, font) + font->stride;
        str += 1;
    }
    return x;
}

int GFX::DrawChar(char c, int x, int y, const Font *font) {
    // does the font contain this char?
    if (c < font->firstChar || font->lastChar < c)
        return 0;
    
    int index = c - font->firstChar;
    uint8_t width = font->charWidths[index];
    const uint8_t *data = font->data + font->charOffsets[index];
    
    // does the font contain this char?
    if (width == 0)
        return 0;
    
    int sx, sy = y - font->charHeight;
    
    if (font->bpp == 1) {
        // b contains the current bit
        int b = 7;
        
        for (int yy = 0; yy < font->fontHeight; yy++, sy++) {
            sx = x;
            
            for (int xx = 0; xx < width; xx++, sx++, b--) {
                
                if (b == -1) {
                    b = 7;
                    data += 1;
                }
                
                int n = ((*data) >> b) & 0x1;
                int color = fontColors[n];
                
                DrawPixel(sx, sy, color);
            }
        }
    }
    
    return width;
}

color_t GFX::GetPixel(int16_t x, int16_t y) {
    if (x < 0 || y < 0 || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
        return 0x0000;
    
    return swap16(colorBuffer[x+y*SCREEN_WIDTH]);
}

void GFX::KeyboardSetup(char *buf, uint8_t len, bool canBeCanceled, uint8_t xPos, uint8_t yPos) {
    keyboard.charBuffer = buf;
    keyboard.charBufferLen = len;
    keyboard.canBeCanceled = canBeCanceled;
    keyboard.xPos = xPos;
    keyboard.yPos = yPos;
    
    // initial cursor placement: on first field if buffer is empty, on OK otherwise
    if (!buf[0]) {
        keyboard.cursorX = 0;
        keyboard.cursorY = 0;
    } else {
        keyboard.cursorX = 11;
        keyboard.cursorY = 2;
    }
    
    keyboard.isDone = false;
    keyboard.isCanceled = false;
}

void GFX::KeyboardColor(color_t font, color_t background, color_t grid, color_t selectFont, color_t selectBG, color_t selectGrid) {
    keyboard.colFont = font;
    keyboard.colBackground = background;
    keyboard.colGrid = grid;
    keyboard.colSelectFont = selectFont;
    keyboard.colSelectBG = selectBG;
    keyboard.colSelectGrid = selectGrid;
}

void GFX::KeyboardUpdate() {
    if (keyboard.isDone || keyboard.isCanceled)
        return;

    int len = strlen(keyboard.charBuffer);
    if (len < 8) {
        if (pButtons->getButtonStatus(PocuterButtons::BUTTON_NUM_LEFT) & PocuterButtons::BUTTON_ACTIVATED) {
            if (keyboard.cursorX > 0)
                keyboard.cursorX -= 1;
        }
        if (pButtons->getButtonStatus(PocuterButtons::BUTTON_NUM_RIGHT) & PocuterButtons::BUTTON_ACTIVATED) {
            if (keyboard.cursorX < 12)
                keyboard.cursorX += 1;
        }
        if (pButtons->getButtonStatus(PocuterButtons::BUTTON_NUM_UP) & PocuterButtons::BUTTON_ACTIVATED) {
            if (keyboard.cursorY > 0)
                keyboard.cursorY -= 1;
        }
        if (pButtons->getButtonStatus(PocuterButtons::BUTTON_NUM_DOWN) & PocuterButtons::BUTTON_ACTIVATED) {
            if (keyboard.cursorY < 2)
                keyboard.cursorY += 1;
        }
    }
    
    if (keyboard.cursorX == 12 && keyboard.cursorY == 2)
        keyboard.cursorX = 11;
    
    if (pButtons->getButtonStatus(PocuterButtons::BUTTON_NUM_A) & PocuterButtons::BUTTON_ACTIVATED) {
        if (keyboard.cursorX == 11 && keyboard.cursorY == 2) {
            keyboard.isDone = true;
        } else {
            if (len < 8) {
                keyboard.charBuffer[len] = keyboardChars[keyboard.cursorX + keyboard.cursorY*13];
                len += 1;
                
                if (len == 8) {
                    // auto-place on OK 
                    keyboard.cursorX = 11;
                    keyboard.cursorY = 2;
                }
            }
        }
    }
    
    if (pButtons->getButtonStatus(PocuterButtons::BUTTON_NUM_B) & PocuterButtons::BUTTON_ACTIVATED) {
        if (len == 0) {
            if (keyboard.canBeCanceled)
                keyboard.isCanceled = true;
        } else {
            len -= 1;
            keyboard.charBuffer[len] = '\0';
        }
    }
}

void GFX::KeyboardDraw() {
    FillRect(keyboard.xPos, keyboard.yPos, 92, 25, keyboard.colGrid);
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 13; x++) {
            if (y == 2 && x == 12)
                continue;
            
            uint16_t bgCol;
            
            if (x == keyboard.cursorX && y == keyboard.cursorY) {
                SetFontColors(GFX_TRANSPARENT, keyboard.colSelectFont);
                bgCol = keyboard.colBackground;
            } else {
                SetFontColors(GFX_TRANSPARENT, keyboard.colFont);
                bgCol = keyboard.colSelectBG;
            }
            
            if (y == 2 && x == 11) {
                FillRect(keyboard.xPos+1+x*7, keyboard.yPos+1+y*8, 13, 7, bgCol);
                DrawString("OK", keyboard.xPos+3+x*7, keyboard.yPos+7+y*8, ALIGN_LEFT, &psFont5x4);
            } else {
                FillRect(keyboard.xPos+1+x*7, keyboard.yPos+1+y*8, 6, 7, bgCol);
                DrawChar(keyboardChars[x+y*13], keyboard.xPos+2+x*7, keyboard.yPos+7+y*8, &psFont5x4);
            }
        }
    }
    
    if (keyboard.cursorX == 11 && keyboard.cursorY == 2)
        DrawRect(keyboard.xPos+keyboard.cursorX*7, keyboard.yPos+keyboard.cursorY*8, 15, 9, keyboard.colSelectGrid);
    else
        DrawRect(keyboard.xPos+keyboard.cursorX*7, keyboard.yPos+keyboard.cursorY*8, 8, 9, keyboard.colSelectGrid);
}

bool GFX::KeyboardIsDone() {
    return keyboard.isDone;
}

bool GFX::KeyboardIsCanceled() {
    return keyboard.isCanceled;
}

#endif
