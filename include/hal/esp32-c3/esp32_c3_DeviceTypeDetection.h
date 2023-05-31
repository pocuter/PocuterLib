#ifndef ESP32_C3_DEVICETYPEDETECTION_H
#define ESP32_C3_DEVICETYPEDETECTION_H
#include "include/hal/PocuterDeviceType.h"

class esp32_c3_DeviceTypeDetection : public PocuterDeviceType {
public:
    esp32_c3_DeviceTypeDetection();
    DEVICE_TYPES getDeviceType();
    virtual ~esp32_c3_DeviceTypeDetection();
private:
    DEVICE_TYPES m_dt;
    int read(uint8_t i2c_addr, uint8_t i2c_reg, uint8_t* data_rd, size_t size);
};

#endif /* ESP32_C3_DEVICETYPEDETECTION_H */

