#include "include/PocuterLibConfig.h"
#ifndef POCUTER_DISABLE_BATTERY
#include "include/hal/esp32-c3/esp32_c3_Battery.h"
#include "include/hal/esp32-c3/esp32_c3_Expander.h"
#include "include/hal/esp32-c3/esp32_c3_ADC.h"

#define EXP_PORT_CHST 	0
#define EXP_PIN_CHST  	3
#define PIN_BTLV		2
#define ACD_CHANNEL_BTLV	ADC1_CHANNEL_2

using namespace PocuterLib::HAL;

esp32_c3_Battery::esp32_c3_Battery() {
	esp32_c3_Expander* expander = esp32_c3_Expander::Instance();
	expander->pinMode(EXP_PORT_CHST, EXP_PIN_CHST, EXPANDER_IN);
}

esp32_c3_Battery::~esp32_c3_Battery() {
}

uint16_t esp32_c3_Battery::getBatteryLevel() {
	uint16_t value = esp32_c3_ADC::singleRead(ACD_CHANNEL_BTLV);
	return value;
}

bool esp32_c3_Battery::isCharging() {
	esp32_c3_Expander* expander = esp32_c3_Expander::Instance();
	if (expander->readPin(EXP_PORT_CHST, EXP_PIN_CHST))
		return true;
	else
		return false;
}
            
            
#endif
