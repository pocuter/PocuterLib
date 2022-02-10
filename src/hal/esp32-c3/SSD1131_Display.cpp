#include "include/hal/esp32-c3/SSD1131_display.h"

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

using namespace PocuterLib::HAL;

SSD1131_Display::SSD1131_Display() {
   
   m_spi = new esp32_c3_SPI();
   m_expander = esp32_c3_Expander::Instance();
   
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
}
void SSD1131_Display::clearScreen() {
    clearWindow(0,0,DISPLAY_X, DISPLAY_Y);
    
}
void SSD1131_Display::clearWindow(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
    if (x > DISPLAY_X || y > DISPLAY_Y)
        return;
    uint8_t x2 = x + width - 1;
    uint8_t y2 = y + height - 1;
    if (x2 > DISPLAY_X)
        x2 = DISPLAY_X;
    if (y2 > DISPLAY_Y)
        y2 = DISPLAY_Y;
    
    m_spi->sendCommandList(COMMAND_CLEAR_WINDOW, x, y, x2, y2, -1);
    
}

void SSD1131_Display::setBrightness(uint8_t brightness) {
    if (brightness > 15)
        brightness = 15;    
    m_spi->sendCommand(COMMAND_MASTER_CURRENT, brightness);
   
    
}

void SSD1131_Display::setPixel(uint16_t x, uint16_t y, uint32_t color) {
    
    if (x >= DISPLAY_X || y >= DISPLAY_Y) return;
    drawLine(x,y,x,y,color);
}

void SSD1131_Display::drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color) {
    uint8_t data[3];
    data[0] = (color >> 16);
    data[1] = (color >> 8);
    data[2] = (color);
    
    m_spi->sendCommandList(0x21, x1, y1, x2, y2, data[0], data[1], data[2], -1);
    
}

void SSD1131_Display::drawScanLine(uint16_t x, uint16_t y, uint16_t width, uint32_t* colors) {
    if (x >= DISPLAY_X) return;
    if (y >= DISPLAY_Y) return ;
    if (x + width >= DISPLAY_X) width = DISPLAY_X - x;
    uint8_t* buffer = (uint8_t*)heap_caps_malloc(width*2, MALLOC_CAP_DMA | MALLOC_CAP_8BIT);
    for (int i = 0; i < width; i++) {
        uint8_t red = colors[i] >> 16;
        uint8_t green = colors[i] >> 8;
        uint8_t blue = colors[i];
        uint16_t b = (blue >> 3) & 0x1f;
        uint16_t g = ((green >> 2) & 0x3f) << 5;
        uint16_t r = ((red >> 3) & 0x1f) << 11;
        uint16_t c = (r | g | b);
        buffer[i*2] = c;
        buffer[1 + (i*2)] = c >> 8; 
        
    }
    
    m_spi->sendCommand(0x15, x, (x + width) - 1);
    m_spi->sendCommand(0x75, y, y);
    m_spi->sendScanLine(buffer, width*2);
    free(buffer);
}

void SSD1131_Display::drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color) {
    uint8_t data[3];
    if (x1 >= DISPLAY_X || y1 >= DISPLAY_Y) return;
    if (x2 >= DISPLAY_X || y2 >= DISPLAY_Y) return;
    data[0] = (color >> 16);
    data[1] = (color >> 8);
    data[2] = (color);
    
    m_spi->sendCommandList(0x22, x1, y1, x2, y2, data[0], data[1], data[2], data[0], data[1], data[2], -1);
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
SSD1131_Display::~SSD1131_Display() {
}

