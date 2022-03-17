
#include "include/PocuterLibConfig.h"

#include <stdio.h>
#include "include/hal/esp32-c3/esp32_c3_I2C.h"
#include "include/hal/esp32-c3/Pocuter_Pins.h"

#ifndef portTICK_RATE_MS
#define portTICK_RATE_MS              portTICK_PERIOD_MS
#endif

using namespace PocuterLib::HAL;

#define WRITE_BIT                          I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT                           I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN                       0x1              /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS                      0x0              /*!< I2C master will not check ack from slave */
#define I2C_SPEED                          400000

SemaphoreHandle_t esp32_c3_I2C::m_i2cSemaphore;

esp32_c3_I2C::esp32_c3_I2C(int port) {
    m_port = port;
    m_driverInstalled = false;
    m_i2cSemaphore = xSemaphoreCreateBinary();
    if (!m_i2cSemaphore) abort();
    
    xSemaphoreGive(m_i2cSemaphore);
    
    
    i2c_config_t m_conf;
    m_conf.mode = I2C_MODE_MASTER;
#ifndef POCUTER_SWITCH_SDA_SCL
    m_conf.sda_io_num = PIN_I2C_SDA;
    m_conf.scl_io_num = PIN_I2C_SCL;
#else
    m_conf.sda_io_num = PIN_I2C_SCL;
    m_conf.scl_io_num = PIN_I2C_SDA;
#endif
    m_conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    m_conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    
    
   
    m_conf.master.clk_speed = I2C_SPEED;
    m_conf.clk_flags = 0;
    i2c_param_config(m_port, &m_conf);
    esp_err_t err = i2c_driver_install(m_port, m_conf.mode, 0, 0, ESP_INTR_FLAG_IRAM);
    if (err == ESP_OK) m_driverInstalled = true;
    
}


esp32_c3_I2C::~esp32_c3_I2C() {
    i2c_driver_delete(m_port);
}
int esp32_c3_I2C::write(uint8_t i2c_addr, uint8_t i2c_reg, uint8_t data_wr) {
    return write(i2c_addr, i2c_reg, &data_wr, 1);
}
int esp32_c3_I2C::write(uint8_t i2c_addr, uint8_t i2c_reg, uint8_t* data_wr, size_t size) {
    if (! m_driverInstalled) return 0;
    xSemaphoreTake(m_i2cSemaphore, portMAX_DELAY);
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ( i2c_addr << 1 ) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, i2c_reg, ACK_CHECK_EN);
    i2c_master_write(cmd, data_wr, size, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(m_port, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    xSemaphoreGive(m_i2cSemaphore);
    
    if (ret == ESP_OK) return 1;
    return 0;
}

int esp32_c3_I2C::read(uint8_t i2c_addr, uint8_t i2c_reg)
{
    uint8_t data_rd = 0;
    if (read(i2c_addr, i2c_reg, &data_rd, 1)) return data_rd;
    return -1;
    
}

int esp32_c3_I2C::read(uint8_t i2c_addr, uint8_t i2c_reg, uint8_t* data_rd, size_t size)
{
    if (size == 0) {
        return ESP_OK;
    }
    xSemaphoreTake(m_i2cSemaphore, portMAX_DELAY);
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
    esp_err_t ret = i2c_master_cmd_begin(m_port, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    xSemaphoreGive(m_i2cSemaphore);
    if (ret == ESP_OK) return 1;
    return 0;
}
