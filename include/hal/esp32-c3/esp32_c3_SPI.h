#ifndef ESP32_C3_SPI_H
#define ESP32_C3_SPI_H

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include "driver/spi_master.h"


#define SPI_TRANSACTION_COUNT (4)


namespace PocuterLib {
    namespace HAL {
        class esp32_c3_SPI {
        public:
           
            
            esp32_c3_SPI();
            virtual ~esp32_c3_SPI();
            
            void sendCommand(const uint8_t cmd);
            void sendCommand(const uint8_t cmd, const uint8_t arg);
            void sendCommand(const uint8_t* cmd, uint8_t size, bool doCopy = false);
            void sendCommandList(const uint8_t cmd, ...);
        private:
            
             struct TRANS_USER{
                uint8_t* transBuffer;
                uint8_t dc;
                
            };
            
            static QueueHandle_t m_spiQueueFree;
            static QueueHandle_t m_spiQueueToDo;
            
            
            static spi_transaction_t m_trans[SPI_TRANSACTION_COUNT];
            static TRANS_USER m_trasUserData[SPI_TRANSACTION_COUNT];
            static spi_device_handle_t m_spi;
            
           
            void init();
            static void spiTask(void *arg);
            static void eventTask(void *arg);
            
            static void spiPreTransfer(spi_transaction_t *t);
            static void spiAfterTransfer(spi_transaction_t *t);
            spi_transaction_t* spiGetTransaction();
            void spiPutTransaction(spi_transaction_t* t);
            
        };
    }
}

#endif /* ESP32_C3_SPI_H */

