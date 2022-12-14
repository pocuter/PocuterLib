#ifndef ESP32_C3_BUTTONS_H
#define ESP32_C3_BUTTONS_H
#include "include/hal/esp32-c3/esp32_c3_Expander.h"
#include "include/hal/PocuterButtons.h"
namespace PocuterLib {
    namespace HAL {
        class esp32_c3_Buttons : public PocuterButtons {
        public:
            
            esp32_c3_Buttons();
            virtual ~esp32_c3_Buttons();
            PBUTTONS getButtonState();
            void registerEventHandler(buttonEventHandler*, void*);
            void unregisterEventHandler();
            void expEvent(uint8_t, uint8_t, void*);
            
            
        private:
            esp32_c3_Expander* m_expander;
            buttonEventHandler* m_eventHandler;
            void* m_eventHandlerUserData;
            uint8_t m_currentButtonStates;
            
            static void expEventHandler(uint8_t, uint8_t, void*);
            void checkCeatCodes(uint8_t buttons);
        };
    }
}

#endif /* ESP32_C3_BUTTONS_H */

