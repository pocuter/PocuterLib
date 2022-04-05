
#ifndef ESP32_C3_HTTP_H
#define ESP32_C3_HTTP_H

#include "include/hal/PocuterHTTP.h"
#include <esp_http_client.h>
namespace PocuterLib {
    namespace HAL {
        class esp32_c3_HTTP : public PocuterHTTP {
        public:
            esp32_c3_HTTP();
            virtual ~esp32_c3_HTTP();
            
            HTTPERROR downloadFile(const uint8_t* url, const uint8_t* destination, bool stepwise, uint8_t *percent, const uint8_t* PEMcert = NULL);
            
        private:
            static esp_err_t fileDownloadEventHandler(esp_http_client_event_t *evt) ;
            char *m_downloadBuffer;
            int m_downloadContentLength;
            int m_downloadTotalReadLen;
            FILE * m_downloadFp;
            esp_http_client_handle_t m_client;
        };
    }
}

#endif /* ESP32_C3_HTTP_H */

