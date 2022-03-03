
#ifndef POCUTERMICROPHONE_H
#define POCUTERMICROPHONE_H
#include <stddef.h>

class PocuterMicrophone {
    public:
        enum MICERROR {
            MICERROR_OK,
            MICERROR_RESOURCE_NOT_AVAILABLE,
            
            MICERROR_UNKNOWN,
        };
        enum MICEVENT {
            MIC_DATA_PACKAGE
           
        };   
        typedef void (micEventHandler)(MICEVENT, void* data, size_t dataSize, void* userData);
        virtual MICERROR registerEventHandler(micEventHandler*, void*) = 0;
        virtual void unregisterEventHandler() = 0;
        virtual MICERROR startRecording() = 0;
        virtual MICERROR stopRecording() = 0;
        
    private:
        
};





#endif /* POCUTERMICROPHONE_H */

