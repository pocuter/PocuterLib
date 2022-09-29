#include "include/PocuterLibConfig.h"
#include "include/hal/esp32-c3/esp32_c3_SPI.h"
#include "include/hal/esp32-c3/esp32_c3_Expander.h"
#include <freertos/task.h>
#include <string.h>
#include <driver/spi_common.h>
#include <stdarg.h>


using namespace PocuterLib::HAL;

#define SPI_PIN_NUM_MISO    10
#define SPI_PIN_NUM_MOSI    6
#define SPI_PIN_NUM_CLK     5

#define LCD_SPI_CLOCK_RATE SPI_MASTER_FREQ_80M


QueueHandle_t esp32_c3_SPI::m_spiQueueFree;
QueueHandle_t esp32_c3_SPI::m_spiQueueToDo;

spi_transaction_t esp32_c3_SPI::m_trans[SPI_TRANSACTION_COUNT];
spi_device_handle_t esp32_c3_SPI::m_spi;
esp32_c3_SPI::TRANS_USER esp32_c3_SPI::m_trasUserData[SPI_TRANSACTION_COUNT];

esp32_c3_SPI::esp32_c3_SPI() {
    init();
}


esp32_c3_SPI::~esp32_c3_SPI() {
}
void esp32_c3_SPI::init() {
    
     m_spiQueueFree = xQueueCreate(SPI_TRANSACTION_COUNT, sizeof(void*));
    if(!m_spiQueueFree) abort();
     
    m_spiQueueToDo = xQueueCreate(SPI_TRANSACTION_COUNT, sizeof(void*));
    if(!m_spiQueueToDo) abort();
    
    
   
    
    esp_err_t ret;
    spi_bus_config_t buscfg;
    memset(&buscfg, 0, sizeof(buscfg));
    
    spi_device_interface_config_t devcfg;
    memset(&devcfg, 0, sizeof(devcfg));
    
    
    buscfg.miso_io_num = SPI_PIN_NUM_MISO;
    buscfg.mosi_io_num = SPI_PIN_NUM_MOSI;
    buscfg.sclk_io_num = SPI_PIN_NUM_CLK;
    buscfg.quadwp_io_num=-1;
    buscfg.quadhd_io_num=-1;
    buscfg.max_transfer_sz = 4000;
    
    devcfg.clock_speed_hz = LCD_SPI_CLOCK_RATE;
    devcfg.mode = 0;                                //SPI mode 0
    devcfg.spics_io_num = -1;                       //CS pin not in use (on expander)
    devcfg.queue_size = 7;                          //We want to be able to queue 7 transactions at a time

    
    devcfg.flags = 0; 

    //Initialize the SPI bus
    ret=spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    assert(ret==ESP_OK);
    
    
    //Attach the LCD to the SPI bus
    ret=spi_bus_add_device(SPI2_HOST, &devcfg, &m_spi);
    assert(ret==ESP_OK);
    
   
    // Initialize transactions
    for (int x = 0; x < SPI_TRANSACTION_COUNT; x++)
    {
        void* param = &m_trans[x];
        m_trasUserData[x].transBuffer = (uint8_t*)heap_caps_malloc(96*2, MALLOC_CAP_DMA | MALLOC_CAP_8BIT);
        m_trans[x].user = &m_trasUserData[x];
        xQueueSend(m_spiQueueFree, &param, portMAX_DELAY);
    }
   
    xTaskCreate(&spiTask, "spiTask", 4000, this, 5, NULL);
    
}

void esp32_c3_SPI::spiTask(void *arg)
{
    esp32_c3_Expander* expander = NULL;  
    uint8_t lastTranactionDC = 0;
    while(1)
      {
        spi_transaction_t* t;
        
        if(xQueueReceive(m_spiQueueToDo, &t, portMAX_DELAY) == pdTRUE )
        {
            //TODO: I Really don't know why, but I have to create the first instance of the expander exactly here, have to invesigate, why
            if (expander == NULL) expander = esp32_c3_Expander::Instance();  
            
            TRANS_USER* user = ( TRANS_USER*) t->user;
            spi_device_acquire_bus(m_spi, portMAX_DELAY);
            expander->setPin(0,7,0);
            if (lastTranactionDC != user->dc) {
                expander->setPin(1,5,user->dc);
                lastTranactionDC = user->dc;
            }
            
            spi_device_transmit(m_spi, t);
            expander->setPin(0,7,1);
            spi_device_release_bus(m_spi);
            if(xQueueSend(m_spiQueueFree, &t, portMAX_DELAY) != pdPASS)
            {
                abort();
            }
        }
      
     
    }

    
   vTaskDelete(NULL);
}
spi_transaction_t* esp32_c3_SPI::spiGetTransaction()
{
    spi_transaction_t* t;
    xQueueReceive(m_spiQueueFree, &t, portMAX_DELAY);
    void* userData = t->user;
    memset(t, 0, sizeof(spi_transaction_t));
    t->user = userData;
    return t;
}
void esp32_c3_SPI::waitEmptyMessageQueue()
{
    while(uxQueueMessagesWaiting(m_spiQueueFree)){
        vTaskDelay(0);
    }
}

void esp32_c3_SPI::spiPutTransaction(spi_transaction_t* t)
{
    t->rx_buffer = NULL;
    t->rxlength = 0;
    xQueueSend(m_spiQueueToDo, &t, portMAX_DELAY);
}
void esp32_c3_SPI::sendCommandList(const uint8_t cmd, ...)
{
    va_list arguments;
    va_start ( arguments, cmd ); 
    int arg;
    uint8_t buf[32];
    buf[0] = cmd;
    int i = 1;
    do {
        arg = va_arg ( arguments, int ); 
        if (arg > -1) {
            buf[i++] = (uint8_t)arg;
            if (i == 32) break;
        }
    } while(arg > -1);
    va_end ( arguments );
    sendCommand(buf, i);
    
    
}
void esp32_c3_SPI::sendCommand(const uint8_t cmd, const uint8_t arg, const uint8_t arg2)
{
    uint8_t buf[3];
    buf[0] = cmd;
    buf[1] = arg;
    buf[2] = arg2;
    sendCommand(buf, 3);
}
void esp32_c3_SPI::sendCommand(const uint8_t cmd, const uint8_t arg)
{
    uint8_t buf[2];
    buf[0] = cmd;
    buf[1] = arg;
    sendCommand(buf, 2);
}
void esp32_c3_SPI::sendCommand(const uint8_t cmd)
{
   sendCommand(&cmd, 1);
}
void esp32_c3_SPI::sendScanLine(const uint8_t* line, uint32_t size, bool noCopy)
{
    if (! noCopy && size > 96*2) return;
    spi_transaction_t* t = spiGetTransaction();
    TRANS_USER* user = ( TRANS_USER*) t->user;
    t->addr = 0;    
    t->length = size*8;                     //Command is 8 bits
    if (! noCopy) {
        memcpy(user->transBuffer, line, size);
        t->tx_buffer = user->transBuffer;
    } else {
        t->tx_buffer = line;
    }
    
    t->flags = 0;
   
    user->dc = 1;
      
    spiPutTransaction(t);
}
void esp32_c3_SPI::sendCommand(const uint8_t* cmd, uint8_t size)
{
    
    spi_transaction_t* t = spiGetTransaction();
    TRANS_USER* user = ( TRANS_USER*) t->user;
    t->addr = 0;    
    t->length = size*8;                     //Command is 8 bits
    if (size < 5) {
       for (int i = 0; i < size; i++) t->tx_data[i] = cmd[i];
       t->flags = SPI_TRANS_USE_TXDATA;
    } else {
        
        memcpy(user->transBuffer, cmd, size);
        t->tx_buffer = user->transBuffer;
        
        t->flags = 0;
    }
    user->dc = 0;
      
    spiPutTransaction(t);
    
}
