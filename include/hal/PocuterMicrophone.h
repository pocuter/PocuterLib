
#ifndef POCUTERMICROPHONE_H
#define POCUTERMICROPHONE_H
#include <stddef.h>

/**
* @brief options to access the Pocuter microphone
*  
*/
class PocuterMicrophone {
    public:
        enum MICERROR {
            MICERROR_OK,                        /*!< everything went well */
            MICERROR_RESOURCE_NOT_AVAILABLE,    /*!< the given recource in not available now */
            
            MICERROR_UNKNOWN,                   /*!< an unusual error */
        };
        enum MICEVENT {
            MIC_DATA_PACKAGE    /*!< a data package is ready */
           
        };   
        enum NOICE_REDUCTION_LEVEL {
            REDUCTION_LEVEL_NONE,   /*!< No noice reduction */
            REDUCTION_LEVEL_SMALL,  /*!< A little reduction */
            REDUCTION_LEVEL_MEDIUM, /*!< Medium reduction */
            REDUCTION_LEVEL_HIGHT,  /*!< hight noice reduction */
            REDUCTION_LEVEL_RAW,    /*!< send raw data only */
           
        };  
        
        enum SAMLE_RATE_HZ {
            SAMLE_RATE_8000 = 8000,     /*!< Sample Rate 8 khz */
            SAMLE_RATE_11025 = 11025,   /*!< Sample Rate 11 khz */
            SAMLE_RATE_16000 = 16000,   /*!< Sample Rate 16 khz */
            SAMLE_RATE_22050 = 22050,   /*!< Sample Rate 22 khz */
            SAMLE_RATE_32000 = 32000,   /*!< Sample Rate 32 khz */
            SAMLE_RATE_44100 = 44100,   /*!< Sample Rate 44 khz */
            SAMLE_RATE_48000 = 48000    /*!< Sample Rate 48 khz */
          
        };  
       
         
        typedef void (micEventHandler)(MICEVENT, void* data, size_t dataSize, void* userData);  /* Callback type definition */
        
        /**
        * @brief register an event callback
        * 
        * @note the event handler is called every time a new data package is ready. You can only have one event handler at a time.
        * 
        * @param h a pointer to the callback function
        * @param u a pointer to user data sent to the event handler on each event
        * 
        * @return 
        *     - MICERROR_OK everything works well
        *     - MICERROR_RESOURCE_NOT_AVAILABLE can not add an event handler now
        */
        virtual MICERROR registerEventHandler(micEventHandler* h, void* u) = 0;

        /**
        * @brief unregister the event handler
        * 
        */
        virtual void unregisterEventHandler() = 0;
        
        /**
        * @brief start the recording
        * 
        * @note after starting the event handler will be calls periodically, when a data package is ready.
        * 
        * @param s the sample rate
        * @param n the noice reduction level, or raw data
        * 
        * @return 
        *     - MICERROR_OK everything works well
        *     - MICERROR_RESOURCE_NOT_AVAILABLE cannot start recording
        */
        virtual MICERROR startRecording(SAMLE_RATE_HZ s, NOICE_REDUCTION_LEVEL n) = 0;
        
        /**
        * @brief stop the recording
        * 
        * @note after stopping the event handler will not be called anymore
 
        */
        virtual MICERROR stopRecording() = 0;
        
        
        
        
    private:
        
};





#endif /* POCUTERMICROPHONE_H */

