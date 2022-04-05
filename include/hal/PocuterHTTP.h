
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
           HTTPERROR_OTAERROR_MORE_STEPS,
           HTTPERROR_UNKNOWN
           
          
       };
        
        virtual HTTPERROR downloadFile(const uint8_t* url, const uint8_t* destination, bool stepwise, uint8_t *percent, const uint8_t* PEMcert = NULL) = 0;
        
    private:

};

#endif /* POCUTERHTTP_H */

