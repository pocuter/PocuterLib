/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   esp32_c3_SDCard.h
 * Author: Test
 *
 * Created on 9. Februar 2022, 12:02
 */

#ifndef ESP32_C3_SDCARD_H
#define ESP32_C3_SDCARD_H
#include "include/hal/PocuterSDCard.h"
#include "sdmmc_cmd.h"
namespace PocuterLib {
    namespace HAL {
        class esp32_c3_SDCard : public PocuterSDCard {
        public:
            esp32_c3_SDCard();
            virtual ~esp32_c3_SDCard();
            bool cardInSlot();
            bool cardIsMounted();
            const char* getMountPoint();
        private:
            sdmmc_card_t *m_card;
            bool m_sdcardMounted;
        };
    }
}

#endif /* ESP32_C3_SDCARD_H */

