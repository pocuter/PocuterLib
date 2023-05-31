
#include "include/hal/esp32-c3/esp32_c3_DeviceTypeDetection.h"
#include <driver/i2c.h>

#define I2C_SPEED                          400000
#define WRITE_BIT                          I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT                           I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN                       0x1              /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS                      0x0              /*!< I2C master will not check ack from slave */

#define EXPANDER_I2C_ADDRESS_POCUTER1    0x5B
#define EXPANDER_ID_POCUTER1           0x10
esp32_c3_DeviceTypeDetection::esp32_c3_DeviceTypeDetection() {
    
    // we will try the pocuter 1. If we don't find the expander this should be a PocketStar
    int port = 0;
    i2c_config_t m_conf;
    m_conf.mode = I2C_MODE_MASTER;

    m_conf.sda_io_num = PIN_I2C_SDA;
    m_conf.scl_io_num = PIN_I2C_SCL;
    m_conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    m_conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    
    m_conf.master.clk_speed = I2C_SPEED;
    m_conf.clk_flags = 0;
    i2c_param_config(port, &m_conf);
    esp_err_t err = i2c_driver_install(port, m_conf.mode, 0, 0, ESP_INTR_FLAG_IRAM);
    uint8_t data_rd = 0;
    read(EXPANDER_I2C_ADDRESS_POCUTER1, EXPANDER_ID_POCUTER1, &data_rd, 1);
    if (data_rd == 0x23) {
        m_dt = DEVICE_TYPE_POCUTER_1;
    } else {
        m_dt = DEVICE_TYPE_POCKETSTAR_2;
    }
    
    i2c_driver_delete(0);
    
}
PocuterDeviceType::DEVICE_TYPES esp32_c3_DeviceTypeDetection::getDeviceType() {
    return m_dt;
}
int esp32_c3_DeviceTypeDetection::read(uint8_t i2c_addr, uint8_t i2c_reg, uint8_t* data_rd, size_t size)
{
    if (size == 0) {
        return ESP_OK;
    }
    
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ( i2c_addr << 1 ), ACK_CHECK_EN);
    i2c_master_write_byte(cmd, i2c_reg, ACK_CHECK_EN);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ( i2c_addr << 1 ) | READ_BIT, ACK_CHECK_EN);
    if (size > 1) {
        i2c_master_read(cmd, data_rd, size - 1, I2C_MASTER_ACK);
    }
    i2c_master_read_byte(cmd, data_rd + size - 1, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(0, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    
    if (ret == ESP_OK) return 1;
    return 0;
}

esp32_c3_DeviceTypeDetection::~esp32_c3_DeviceTypeDetection() {
}

