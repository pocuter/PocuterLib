#ifndef POCUTEROTA_H
#define POCUTEROTA_H
#include <stdint.h>
#include <stddef.h>
#include <vector>

class PocuterOTA {
    public:
        enum POCUTER_PARTITION {
            PART_APPLOADER,
            PART_APP1,
            
            PART_UNKNOWN
        };
        enum OTAERROR {
            OTAERROR_OK = 0,
            OTAERROR_PATITION_NOT_AVAILABLE,
            OTAERROR_PATITION_IN_USE,
            OTAERROR_FILE_NOT_FOUND,
            OTAERROR_NO_SD_CARD,
            OTAERROR_FLASHING_FAILED,
            OTAERROR_MORE_STEPS,
            OTAERROR_APP_READ_ERROR,
            
            OTAERROR_UNKNOWN
        };
        virtual OTAERROR setNextAppID(uint64_t appID) = 0;
        virtual OTAERROR getApps(std::vector<uint64_t>* apps, int maxLength, int offset) = 0;
        virtual uint32_t getAppsCount() = 0;
        virtual OTAERROR getAppVersion(uint64_t appID, uint8_t* major, uint8_t* minor, uint8_t* patch) = 0;
        virtual OTAERROR flashFromSDCard(uint64_t appID, POCUTER_PARTITION partition, bool checkSigning = false, bool stepwise = false, uint8_t *percent = NULL) = 0;
        virtual OTAERROR bootPartition(POCUTER_PARTITION partition) = 0;
        virtual POCUTER_PARTITION getCurrentPartition() = 0;
        virtual OTAERROR restart() = 0;

        virtual OTAERROR verifyPartition(POCUTER_PARTITION partition) = 0; 
        virtual OTAERROR forceBootloaderToReflashApp() = 0; 
        

    private:

};


#endif /* POCUTEROTA_H */

