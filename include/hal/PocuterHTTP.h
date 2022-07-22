
#ifndef POCUTERHTTP_H
#define POCUTERHTTP_H

class PocuterHTTP {
    public:
        enum HTTPERROR {
           HTTPERROR_OK = 0,
           HTTPERROR_CONNECT_FAILED,
           HTTPERROR_NO_MEMORY,
           HTTPERROR_FILE_OPEN_FAILED,
           HTTPERROR_DOWNLOAD_FAILED,
           HTTPERROR_MORE_STEPS,
           HTTPERROR_UNKNOWN
           
          
       };
      
        /**
        * @brief download a file from web
        * 
        * @param url the url to the file
        * @param destination the destination on the sd card (beginning with the value you get from PocuterSDCard::getMountPoint() )
        * @param stepwise If true download a file step by step instead of downloading everything at once. This way you can realize a progress bar for example.
        * @param percent (out) Shows how much percent of the file has been downloaded so far. Is always 100 if stepwise=false. This is only available if the http header contains "content-length"
        * @param PEMcert (optional) if it is an https address, you need to add the root certificate to trust the server.
        */
        virtual HTTPERROR downloadFile(const uint8_t* url, const uint8_t* destination, bool stepwise, uint8_t *percent, const uint8_t* PEMcert = NULL) = 0;
        
        /**
        * @brief sends a get request to a server
        * 
        * @param url the url for the get request
        * @param response the response text. You must allocate enough memory for the response.
        * @param maxSize the max size you can hold in the response
        * @param PEMcert (optional) if it is an https address, you need to add the root certificate to trust the server.
        */
        virtual HTTPERROR getResponse(const uint8_t* url, uint8_t* response, size_t maxSize, const uint8_t* PEMcert = NULL) = 0;
    private:

};

#endif /* POCUTERHTTP_H */

