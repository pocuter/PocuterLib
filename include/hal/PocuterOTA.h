/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   PocuterOTA.h
 * Author: Test
 *
 * Created on 12. Februar 2022, 12:01
 */

#ifndef POCUTEROTA_H
#define POCUTEROTA_H
#include <stdint.h>
#include <stddef.h>
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
            
            
            OTAERROR_UNKNOWN
        };
        virtual OTAERROR setNextAppID(uint64_t appID);
        virtual OTAERROR flashFromSDCard(uint64_t appID, POCUTER_PARTITION partition, bool checkSigning = false, bool stepwise = false, uint8_t *percent = NULL) = 0;
        virtual OTAERROR bootPartition(POCUTER_PARTITION partition) = 0;
        virtual POCUTER_PARTITION getCurrentPartition() = 0;
        virtual OTAERROR restart() = 0;
    private:

};


#endif /* POCUTEROTA_H */

