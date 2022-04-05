
#include <esp_http_client.h>

#include "include/hal/esp32-c3/esp32_c3_HTTP.h"

#define MAX_HTTP_RECV_BUFFER 2048*10


using namespace PocuterLib::HAL;

esp32_c3_HTTP::esp32_c3_HTTP() {
    m_downloadBuffer = NULL;
}



esp32_c3_HTTP::~esp32_c3_HTTP() {
}

esp_err_t esp32_c3_HTTP::fileDownloadEventHandler(esp_http_client_event_t *evt) {
    printf("http event: %d\n", evt->event_id);
    
    return ESP_OK;
}


PocuterHTTP::HTTPERROR esp32_c3_HTTP::downloadFile(const uint8_t* url, const uint8_t* destination, bool stepwise, uint8_t *percent, const uint8_t* PEMcert) {
    
    if (m_downloadBuffer == NULL) {
        m_downloadBuffer = (char*)malloc(MAX_HTTP_RECV_BUFFER);
    
        if (m_downloadBuffer == NULL) {
            return PocuterHTTP::HTTPERROR_NO_MEMORY;
        }
        
        m_downloadFp = fopen ((const char*)destination, "wb");
        if (! m_downloadFp){
            free(m_downloadBuffer);
            m_downloadBuffer = NULL;
            return PocuterHTTP::HTTPERROR_FILE_OPEN_FAILED;
        }

        esp_http_client_config_t config = {
            .url = (const char*) url,
            .cert_pem = (const char*)PEMcert,
            .cert_len = 0
        };
        m_client = esp_http_client_init(&config);
        esp_err_t err;
        if ((err = esp_http_client_open(m_client, 0)) != ESP_OK) {
            free(m_downloadBuffer);
            m_downloadBuffer = NULL;
            fclose(m_downloadFp);
            return PocuterHTTP::HTTPERROR_CONNECT_FAILED;
        }
        m_downloadContentLength = esp_http_client_fetch_headers(m_client);
        m_downloadTotalReadLen = 0;
    }
    
    int read_len = -1;
    while (m_downloadTotalReadLen < m_downloadContentLength) {
        read_len = esp_http_client_read(m_client, m_downloadBuffer, MAX_HTTP_RECV_BUFFER);
        if (read_len <= 0) {
            break;
        }
        
        if (percent != NULL) *percent = (100 * m_downloadTotalReadLen) / m_downloadContentLength;
        fwrite(m_downloadBuffer, read_len, 1, m_downloadFp);
        m_downloadTotalReadLen += read_len;
        if (stepwise) break;
    }
    if (read_len <= 0) {
        esp_http_client_close(m_client);
        esp_http_client_cleanup(m_client);
        free(m_downloadBuffer);
        m_downloadBuffer = NULL;
        fclose(m_downloadFp);
        if (m_downloadTotalReadLen != m_downloadContentLength) return PocuterHTTP::HTTPERROR_DOWNLOAD_FAILED;
        return PocuterHTTP::HTTPERROR_OK;
    }
    return PocuterHTTP::HTTPERROR_OTAERROR_MORE_STEPS;
}