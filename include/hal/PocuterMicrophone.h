
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
        enum NOICE_REDUCTION_LEVEL {
            REDUCTION_LEVEL_NONE,
            REDUCTION_LEVEL_SMALL,
            REDUCTION_LEVEL_MEDIUM,
            REDUCTION_LEVEL_HIGHT,
            REDUCTION_LEVEL_RAW,
           
        };  
        
        enum SAMLE_RATE_HZ {
            SAMLE_RATE_8000 = 8000,
            SAMLE_RATE_11025 = 11025,
            SAMLE_RATE_16000 = 16000,
            SAMLE_RATE_22050 = 22050,
            SAMLE_RATE_32000 = 32000,
            SAMLE_RATE_44100 = 44100,
            SAMLE_RATE_48000 = 48000
          
        };  
        
        typedef void (micEventHandler)(MICEVENT, void* data, size_t dataSize, void* userData);
        virtual MICERROR registerEventHandler(micEventHandler*, void*) = 0;
        virtual void unregisterEventHandler() = 0;
        virtual MICERROR startRecording(SAMLE_RATE_HZ, NOICE_REDUCTION_LEVEL) = 0;
        virtual MICERROR stopRecording() = 0;
        
    private:
        
};





#endif /* POCUTERMICROPHONE_H */

