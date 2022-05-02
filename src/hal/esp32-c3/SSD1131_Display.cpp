#include "include/PocuterLibConfig.h"

#ifndef POCUTER_DISABLE_DISPLAY   
#include "include/hal/esp32-c3/SSD1131_Display.h"
#include <string.h>

#define OLED_SHDN_PORT  0
#define OLED_SHDN_PIN   2
#define OLED_DC_PORT    1
#define OLED_DC_PIN     5
#define OLED_RST_PORT   1
#define OLED_RST_PIN    4
#define OLED_CS_PORT    0
#define OLED_CS_PIN     7

#define DISPLAY_X       96
#define DISPLAY_Y       64

#define UPDATE_LINES_AT_ONCE 16

#define COMMAND_SET_COLOUMN     0x15
#define COMMAND_DRAW_LINE       0x21
#define COMMAND_DRAW_RECTANGLE  0x22
#define COMMAND_CLEAR_WINDOW    0x25
#define COMMAND_SET_FILL        0x26
#define COMMAND_SET_ROW         0x75
#define COMMAND_MASTER_CURRENT  0x87 // brightness
#define COMMAND_SET_REMAP       0xA0
#define COMMAND_DISPLAY_OFF     0xAE
#define COMMAND_DISPLAY_ON      0xAF

#define sgn(x) ((x<0)?-1:((x>0)?1:0)) /* macro to return the sign of a
                                         number */

#define swap16(val) ( (((val) >> 8) & 0x00FF) | (((val) << 8) & 0xFF00) )

using namespace PocuterLib::HAL;
bool SSD1131_Display::g_runUpdateTask = false;
bool SSD1131_Display::g_continuouseScreenUpdate = true;
SemaphoreHandle_t SSD1131_Display::g_displaySemaphore;
SemaphoreHandle_t SSD1131_Display::g_displayPauseSemaphore;
bool SSD1131_Display::g_initializing = true;

SSD1131_Display::SSD1131_Display(BUFFER_MODE bm) {
   
   m_spi = new esp32_c3_SPI();
   m_expander = esp32_c3_Expander::Instance();
   m_bm = bm;
   
    g_displaySemaphore = xSemaphoreCreateBinary();
    if (!g_displaySemaphore) abort();
    xSemaphoreGive(g_displaySemaphore);
    
    g_displayPauseSemaphore = xSemaphoreCreateBinary();
    if (!g_displayPauseSemaphore) abort();
    xSemaphoreGive(g_displayPauseSemaphore);
       
   if (bm == BUFFER_MODE_DOUBLE_BUFFER) {
       m_currentBackBuffer = (uint16_t*)heap_caps_malloc(DISPLAY_X*DISPLAY_Y*2, MALLOC_CAP_DMA | MALLOC_CAP_8BIT);
       if (m_currentBackBuffer) memset(m_currentBackBuffer, 0, DISPLAY_X*DISPLAY_Y*2);
       if (! m_currentBackBuffer) m_bm = BUFFER_MODE_NO_BUFFER;
       
   }
   if (bm == BUFFER_MODE_DOUBLE_BUFFER) {
       m_currentFrontBuffer = (uint16_t*)heap_caps_malloc(DISPLAY_X*DISPLAY_Y*2, MALLOC_CAP_DMA | MALLOC_CAP_8BIT);
       if (m_currentFrontBuffer) memset(m_currentFrontBuffer, 0, DISPLAY_X*DISPLAY_Y*2);
       if (! m_currentFrontBuffer){
           m_bm = BUFFER_MODE_NO_BUFFER;
           heap_caps_free(m_currentBackBuffer);
       }
   }

    m_expander->pinMode(OLED_SHDN_PORT, OLED_SHDN_PIN, EXPANDER_OUT);
   m_expander->pinMode(OLED_DC_PORT, OLED_DC_PIN, EXPANDER_OUT);
   m_expander->pinMode(OLED_CS_PORT, OLED_CS_PIN, EXPANDER_OUT);
   m_expander->pinMode(OLED_RST_PORT, OLED_RST_PIN, EXPANDER_OUT);
   
   m_expander->setPin(OLED_DC_PORT, OLED_DC_PIN, 0);
   m_expander->setPin(OLED_CS_PORT, OLED_CS_PIN, 0);
   m_expander->setPin(OLED_SHDN_PORT, OLED_SHDN_PIN, 1);
   
   reset();

   vTaskDelay(configTICK_RATE_HZ / 100);
   
    m_spi->sendCommand(COMMAND_DISPLAY_OFF);
    
    const uint8_t init[] = {
        0xA1, 0x00, // STARTLINE
        0xA2, 0x00, // DISPLAYOFFSET
        0xA4, 0xBC,// NORMALDISPLAY
        0xA8, 0x3F, // SETMULTIPLEX
        0xAD, 0x8E, // SETMASTER
        0xB0, 0x0B, // POWERMODE
        
        0xB1, 0x31, // PRECHARGE
        0xB3, 0xF0, // CLOCKDIV
        0x8A, 0x64, // PRECHARGE A
        0x8B, 0x78, // PRECHARGE B
        0x8C, 0x64, // PRECHARGE C
        0xBB, 0x3A, // PRECHARGELEVEL
        0xBE, 0x3E, // VCOMH
        0x81, 0x50, // CONTRAST A
        0x82, 0x50, // CONTRAST B
        0x83, 0x50, // CONTRAST C
        
        0x26, 0x01, // enable fill rectangle
        COMMAND_SET_REMAP, (1 << 6) | (1 << 5) | (1 << 4) | (1 << 1),
        COMMAND_MASTER_CURRENT, 5,
       
        
    };
    
    m_spi->sendCommand(init, sizeof(init));
    clearScreen();
    m_spi->sendCommand(COMMAND_DISPLAY_ON, 0xBC);
    
    if (bm == BUFFER_MODE_DOUBLE_BUFFER) {
       g_runUpdateTask = true;
       xTaskCreate(&updateTask, "updateTask", 4000, this, 10, NULL);
   }
    
}
void SSD1131_Display::updateScreen() {
    if (g_continuouseScreenUpdate) return;
    xSemaphoreTake(g_displaySemaphore, portMAX_DELAY);
    memcpy(m_currentFrontBuffer, m_currentBackBuffer, DISPLAY_X*DISPLAY_Y*2);
    xSemaphoreGive(g_displaySemaphore);
    xSemaphoreGive(g_displayPauseSemaphore);
}
void SSD1131_Display::continuousScreenUpdate(bool on) {
    if (m_bm != BUFFER_MODE_DOUBLE_BUFFER) return;
    xSemaphoreTake(g_displaySemaphore, portMAX_DELAY);
    g_continuouseScreenUpdate = on;
    if (on && ! uxSemaphoreGetCount(g_displayPauseSemaphore)) xSemaphoreGive(g_displayPauseSemaphore);
    xSemaphoreGive(g_displaySemaphore);
}
PocuterDisplay::BUFFER_MODE SSD1131_Display::getBufferMode() {
    return m_bm;
}
void SSD1131_Display::clearScreen() {
    clearWindow(0,0,DISPLAY_X, DISPLAY_Y);
    
}
void SSD1131_Display::clearWindow(int8_t x, int8_t y, uint8_t width, uint8_t height) {
    if (x > DISPLAY_X || y > DISPLAY_Y)
        return;
    int16_t x2 = x + width - 1;
    int16_t y2 = y + height - 1;
    if (x2 > DISPLAY_X)
        x2 = DISPLAY_X;
    if (y2 > DISPLAY_Y)
        y2 = DISPLAY_Y;
    if (m_bm == BUFFER_MODE_NO_BUFFER) {
        m_spi->sendCommandList(COMMAND_CLEAR_WINDOW, x, y, (uint8_t)x2, (uint8_t)y2, -1);
    } else {
        drawRectangle(x,y,x2,y2,0);
    }
    
    
}

void SSD1131_Display::setBrightness(uint8_t brightness) {
    if (brightness > 15)
        brightness = 15;    
    m_spi->sendCommand(COMMAND_MASTER_CURRENT, brightness);
   
    
}
void SSD1131_Display::set16BitPixel(int16_t x, int16_t y, uint16_t color) {
    if (x >= DISPLAY_X || y >= DISPLAY_Y || x < 0 || y < 0) return;
    if (m_bm == BUFFER_MODE_NO_BUFFER) return;
   
    xSemaphoreTake(g_displaySemaphore, portMAX_DELAY);
    m_currentBackBuffer[x + (DISPLAY_X * y)] = swap16(color);
    xSemaphoreGive(g_displaySemaphore);
    
}
void SSD1131_Display::setPixel(int16_t x, int16_t y, uint32_t color) {
    
    if (x >= DISPLAY_X || y >= DISPLAY_Y || x < 0 || y < 0) return;
    if (m_bm == BUFFER_MODE_NO_BUFFER) {
        drawLine(x,y,x,y,color);
        return;
    } else {
        xSemaphoreTake(g_displaySemaphore, portMAX_DELAY);
        m_currentBackBuffer[x + (DISPLAY_X * y)] = swap16(color24to16(color));
        xSemaphoreGive(g_displaySemaphore);
    }
    
    
}

void SSD1131_Display::drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color) {
    if (m_bm == BUFFER_MODE_NO_BUFFER) {
        if (x1 < 0) x1 = 0;
        if (y1 < 0) y1 = 0;
        uint8_t data[3];
        data[0] = (color >> 16);
        data[1] = (color >> 8);
        data[2] = (color);

        m_spi->sendCommandList(0x21, x1, y1, x2, y2, data[0], data[1], data[2], -1);
    } else {
        uint16_t c = color24to16(color);
        int i,dx,dy,sdx,sdy,dxabs,dyabs,x,y,px,py;
        dx=x2-x1;      /* the horizontal distance of the line */
        dy=y2-y1;      /* the vertical distance of the line */
        dxabs=abs(dx);
        dyabs=abs(dy);
        sdx=sgn(dx);
        sdy=sgn(dy);
        x=dyabs>>1;
        y=dxabs>>1;
        px=x1;
        py=y1;

        //VGA[(py<<8)+(py<<6)+px]=color;

        set16BitPixel(px,py,c);
        if (dxabs>=dyabs) /* the line is more horizontal than vertical */
        {
          for(i=0;i<dxabs;i++)
          {
            y+=dyabs;
            if (y>=dxabs)
            {
              y-=dxabs;
              py+=sdy;
            }
            px+=sdx;
            set16BitPixel(px,py,c);
          }
        }
        else /* the line is more vertical than horizontal */
        {
          for(i=0;i<dyabs;i++)
          {
            x+=dxabs;
            if (x>=dyabs)
            {
              x-=dyabs;
              px+=sdx;
            }
            py+=sdy;
            set16BitPixel(px,py,c);
          }
        }
    }
    
}

uint16_t SSD1131_Display::color24to16(uint32_t color) {
    uint8_t red = color >> 16;
    uint8_t green = color >> 8;
    uint8_t blue = color;
    uint16_t b = (blue >> 3) & 0x1f;
    uint16_t g = ((green >> 2) & 0x3f) << 5;
    uint16_t r = ((red >> 3) & 0x1f) << 11;
    return (r | g | b);
}

void SSD1131_Display::draw16BitScanLine(int16_t x, int16_t y, uint16_t width, uint16_t* colors) {
    if (x >= DISPLAY_X) return;
    if (y >= DISPLAY_Y) return ;
    if (y < 0) return ;
    if (x + width >= DISPLAY_X) width = DISPLAY_X - x;
    if (x + width < 0) return;
    if (x < 0) {
        width += x;
        colors += (x * -1);
        x = 0;
    }
    if (m_bm == BUFFER_MODE_NO_BUFFER) { 
        uint8_t* buffer = (uint8_t*)heap_caps_malloc(width*2, MALLOC_CAP_DMA | MALLOC_CAP_8BIT);
        for (int i = 0; i < width; i++) {
            uint16_t c = colors[i];
            buffer[i*2] = c;
            buffer[1 + (i*2)] = c >> 8; 

        }
    
        m_spi->sendCommand(0x15, x, (x + width) - 1);
        m_spi->sendCommand(0x75, y, y);
        m_spi->sendScanLine(buffer, width*2);
        free(buffer);
    } else {
        xSemaphoreTake(g_displaySemaphore, portMAX_DELAY);
        for (int i = 0; i < width; i++) {
            m_currentBackBuffer[(x+i) + (DISPLAY_X * y)] = colors[i];
        }
        xSemaphoreGive(g_displaySemaphore);
    }
    
}
void SSD1131_Display::drawScanLine(int16_t x, int16_t y, uint16_t width, uint32_t* colors) {
    if (x >= DISPLAY_X) return;
    if (y >= DISPLAY_Y) return ;
    if (y < 0) return ;
    if (x + width >= DISPLAY_X) width = DISPLAY_X - x;
    if (x + width < 0) return;
    if (x < 0) {
        width += x;
        colors += (x * -1);
        x = 0;
        
    }
    if (m_bm == BUFFER_MODE_NO_BUFFER) { 
        uint8_t* buffer = (uint8_t*)heap_caps_malloc(width*2, MALLOC_CAP_DMA | MALLOC_CAP_8BIT);
        for (int i = 0; i < width; i++) {
            uint16_t c = color24to16(colors[i]);
            buffer[i*2] = c;
            buffer[1 + (i*2)] = c >> 8; 

        }
    
        m_spi->sendCommand(0x15, x, (x + width) - 1);
        m_spi->sendCommand(0x75, y, y);
        m_spi->sendScanLine(buffer, width*2);
        free(buffer);
    } else {
        xSemaphoreTake(g_displaySemaphore, portMAX_DELAY);
        for (int i = 0; i < width; i++) {
            uint16_t c = color24to16(colors[i]);
            m_currentBackBuffer[(x+i) + (DISPLAY_X * y)] = c;
        }
        xSemaphoreGive(g_displaySemaphore);
    }
}

void SSD1131_Display::drawRectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color) {
    uint8_t data[3];
    if (x1 >= DISPLAY_X || y1 >= DISPLAY_Y) return;
    
    if (m_bm == BUFFER_MODE_NO_BUFFER) {
        data[0] = (color >> 16);
        data[1] = (color >> 8);
        data[2] = (color);
         
        if (x1 < 0) x1 = 0;
        if (y1 < 0) y1 = 0;
        m_spi->sendCommandList(0x22, x1, y1, x2, y2, data[0], data[1], data[2], data[0], data[1], data[2], -1);
    } else {
        uint16_t c = swap16(color24to16(color));
        xSemaphoreTake(g_displaySemaphore, portMAX_DELAY);
        for (int ix = x1; ix <= x2; ix++) {
            for (int iy = y1; iy <= y2; iy++) {
                if (iy < 0) continue;
                if (ix < 0) continue;
                if (iy >= DISPLAY_Y) continue;
                if (ix >= DISPLAY_X) continue;
                m_currentBackBuffer[ix + (DISPLAY_X * iy)] = c;
            }
        }
        xSemaphoreGive(g_displaySemaphore);
    }
}

void SSD1131_Display::getDisplaySize(uint16_t& sizeX, uint16_t& sizeY) {
    sizeX = DISPLAY_X;
    sizeY = DISPLAY_Y;
}

void SSD1131_Display::reset() {
   m_expander->setPin(OLED_SHDN_PORT, OLED_SHDN_PIN, 0);
   vTaskDelay(configTICK_RATE_HZ / 10);
   m_expander->setPin(OLED_SHDN_PORT, OLED_SHDN_PIN, 1);
   vTaskDelay(configTICK_RATE_HZ / 10);
   m_expander->setPin(OLED_RST_PORT, OLED_RST_PIN, 1);
   vTaskDelay(configTICK_RATE_HZ / 10);
   m_expander->setPin(OLED_RST_PORT, OLED_RST_PIN, 0);
   vTaskDelay(configTICK_RATE_HZ / 10);
   m_expander->setPin(OLED_RST_PORT, OLED_RST_PIN, 1);
}

void SSD1131_Display::updateTask(void *arg) {
    SSD1131_Display* myself = (SSD1131_Display*) arg;
    int i = 0;
    if (g_initializing) vTaskDelay(configTICK_RATE_HZ);
    g_initializing = false;
    myself->m_spi->sendCommand(0x15, 0, DISPLAY_X - 1);
    myself->m_spi->sendCommand(0x75, 0, DISPLAY_Y - 1);
            
    while(g_runUpdateTask) {
        myself->m_spi->sendScanLine((uint8_t*)&(myself->m_currentFrontBuffer[i*DISPLAY_X]), DISPLAY_X*UPDATE_LINES_AT_ONCE*2, true);
        i+=UPDATE_LINES_AT_ONCE;
        if (i == DISPLAY_Y) {
            // should i stop here?
            if (! g_continuouseScreenUpdate) xSemaphoreTake(g_displayPauseSemaphore, portMAX_DELAY);
            
            
            myself->m_spi->sendCommand(0x15, 0, DISPLAY_X - 1);
            myself->m_spi->sendCommand(0x75, 0, DISPLAY_Y - 1);
            i = 0;
            if (g_continuouseScreenUpdate) {
                xSemaphoreTake(myself->g_displaySemaphore, portMAX_DELAY);
                memcpy(myself->m_currentFrontBuffer, myself->m_currentBackBuffer, DISPLAY_X*DISPLAY_Y*2);
                xSemaphoreGive(myself->g_displaySemaphore);
            }
        }
        vTaskDelay(0);
    }
    vTaskDelete(NULL);
}


SSD1131_Display::~SSD1131_Display() {
    g_runUpdateTask = false;
}




#endif
