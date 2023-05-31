#ifndef ESP32_C3_BUTTONS_H
#define ESP32_C3_BUTTONS_H
#include "include/hal/esp32-c3/esp32_c3_Expander.h"
#include "include/hal/PocuterButtons.h"
#include "include/PocuterLibConfig.h"
#define BUTTON_NUM 10

namespace PocuterLib {
    namespace HAL {
        class esp32_c3_Buttons : public PocuterButtons {
        public:
            
            esp32_c3_Buttons();
            virtual ~esp32_c3_Buttons();
            PBUTTONS getButtonState();

            //void expEvent(uint8_t, uint8_t, void*);
            
            void updateButtons();
            PBACTION getButtonStatus(PBUTTON button);
            bool isButtonPressedDown(PBUTTON button);
            
            
            bool isButtonPressedDownInternal(PBUTTON button);
            void stopForwardingKeys();
            void continueForwardingKeys();
            
        private:
        
            struct BUTTON_INFO {
                bool isExpander;
                uint8_t port;
                uint8_t pin;
                int8_t mirror; /*mirror =! -1 : This is only another Name for an button, the number is the element in this array of the other button. Has to be smaller than this element*/
            };
            struct BUTTON_HELPER {
                PBACTION status;
                bool wasPressed; // whether it was pressed on last check
                uint64_t pressTime; // time, the button was pressed used for long hold
                uint64_t lastTimeActivated; // used to time repeated activations
                bool firstActivated; // used to apply a bigger delay after the first activation
            };
            
            static const BUTTON_INFO s_buttonInfo_pocuter_ext[BUTTON_NUM];
            static const BUTTON_INFO s_buttonInfo_pocket[10];
            
            BUTTON_HELPER m_buttonHelper[BUTTON_NUM];
            
            esp32_c3_Expander* m_expander;
            
            void* m_eventHandlerUserData;
            uint8_t m_currentButtonStates;
            
            int m_buttonCount;
            const BUTTON_INFO* m_binfo;
            bool m_stopForwardingKeys;
        };
    }
}

#endif /* ESP32_C3_BUTTONS_H */

