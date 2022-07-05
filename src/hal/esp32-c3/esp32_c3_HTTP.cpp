#include "include/PocuterLibConfig.h"
#ifndef POCUTER_DISABLE_HTTP
#include <esp_http_client.h>

#include "include/hal/esp32-c3/esp32_c3_HTTP.h"

#define MAX_HTTP_RECV_BUFFER 2048*10


using namespace PocuterLib::HAL;

#pragma GCC diagnostic push     
#pragma GCC diagnostic ignored "-Wmissing-field-initializers" 

esp32_c3_HTTP::esp32_c3_HTTP() {
    m_downloadBuffer = NULL;
}



esp32_c3_HTTP::~esp32_c3_HTTP() {
}

esp_err_t esp32_c3_HTTP::fileDownloadEventHandler(esp_http_client_event_t *evt) {
    
    return ESP_OK;
}
PocuterHTTP::HTTPERROR esp32_c3_HTTP::getResponse(const uint8_t* url, uint8_t* response, size_t maxSize, const uint8_t* PEMcert) {
    esp_err_t err;
    if (maxSize < 2) return HTTPERROR_NO_MEMORY;
    
    esp_http_client_config_t config = {
            .url = (const char*) url,
            .cert_pem = (const char*)PEMcert,
            .cert_len = 0,
            .disable_auto_redirect = true,
            .event_handler = fileDownloadEventHandler,
           
    };
    response[0] = 0;
    PocuterHTTP::HTTPERROR res = HTTPERROR_DOWNLOAD_FAILED;
    bool opened = false;
    esp_http_client_handle_t client = esp_http_client_init(&config);
    if ((err = esp_http_client_open(client, 0)) == ESP_OK){
        if (esp_http_client_fetch_headers(client) >= 0) {
            opened = true;
        } 
    }
    if (opened) {
        int len = esp_http_client_read_response(client, (char*)response, maxSize - 1);
        if (len > 0) {
            response[len] = 0;
            res = HTTPERROR_OK;
        }
        if (len == maxSize - 1) res = HTTPERROR_NO_MEMORY;
        
    }
    esp_http_client_close(client);
    esp_http_client_cleanup(client);
    return res;
        
    
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
            .cert_len = 0,
            .disable_auto_redirect = true,
            .event_handler = fileDownloadEventHandler,
                  
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
        if (esp_http_client_get_status_code(m_client) == 302){
            esp_http_client_set_redirection(m_client);
             if ((err = esp_http_client_open(m_client, 0)) != ESP_OK) {
                esp_http_client_close(m_client);
                esp_http_client_cleanup(m_client);
                free(m_downloadBuffer);
                m_downloadBuffer = NULL;
                fclose(m_downloadFp);
                return PocuterHTTP::HTTPERROR_CONNECT_FAILED;
            }
            m_downloadContentLength = esp_http_client_fetch_headers(m_client);
            m_downloadTotalReadLen = 0;

        }
        
    }
    
    int read_len = -1;
    bool complete = false;
    while (m_downloadTotalReadLen < m_downloadContentLength || m_downloadContentLength == 0) {
        read_len = esp_http_client_read(m_client, m_downloadBuffer, MAX_HTTP_RECV_BUFFER);
        if (read_len <= 0) {
            break;
        }
        fwrite(m_downloadBuffer, read_len, 1, m_downloadFp);
        m_downloadTotalReadLen += read_len;
        if (percent != NULL && m_downloadContentLength != 0) *percent = (100 * m_downloadTotalReadLen) / m_downloadContentLength;
        
        complete = esp_http_client_is_complete_data_received(m_client);
        if (complete) {
            if (percent != NULL) *percent = 100;
            break;
        }
        if (stepwise) break;
    }
    if (complete || read_len <= 0) {
        
        esp_http_client_close(m_client);
        esp_http_client_cleanup(m_client);
        free(m_downloadBuffer);
        m_downloadBuffer = NULL;
        fclose(m_downloadFp);
        if (! complete) return PocuterHTTP::HTTPERROR_DOWNLOAD_FAILED;
        
        return PocuterHTTP::HTTPERROR_OK;
    }
    return PocuterHTTP::HTTPERROR_MORE_STEPS;
}

#pragma GCC diagnostic pop 

#endif
