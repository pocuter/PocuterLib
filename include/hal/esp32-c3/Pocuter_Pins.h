#ifndef Pocuter_Pins_h
#define Pocuter_Pins_h

#define PIN_MASK_TYPE       (1 << 7)
#define PIN_MASK_NATIVE     (0 << 7)
#define PIN_MASK_EXPANDER   (1 << 7)
#define PIN_MASK_EXP_PORT   (1 << 6)
#define PIN_MASK_EXP_P0     (0 << 6)
#define PIN_MASK_EXP_P1     (1 << 6)
#define PIN_MASK_NR         (0b00111111)

#define EXPANDER_I2C_ADDRESS    0x5B

#define EXP_REG_INPUT_P0        0x00
#define EXP_REG_INPUT_P1        0x01
#define EXP_REG_OUTPUT_P0       0x02
#define EXP_REG_OUTPUT_P1       0x03
#define EXP_REG_CONFIG_P0       0x04
#define EXP_REG_CONFIG_P1       0x05
#define EXP_REG_INT_CONFIG_P0   0x06
#define EXP_REG_INT_CONFIG_P1   0x07
#define EXP_REG_GLOBAL_CONTROL  0x11


/* OLC DEFINITIONS */
/*
#define PIN_EXPANDER_INTERRUPT  (PIN_MASK_NATIVE   | 10)
#define PIN_EXPANDER_RESET      (PIN_MASK_NATIVE   | 20)
#define PIN_BUTTON_0            (PIN_MASK_EXPANDER | PIN_MASK_EXP_P1 | 4)
#define PIN_BUTTON_1            (PIN_MASK_EXPANDER | PIN_MASK_EXP_P1 | 5)
#define PIN_BUTTON_2            (PIN_MASK_EXPANDER | PIN_MASK_EXP_P1 | 6)
#define PIN_OLED_DC             (PIN_MASK_EXPANDER | PIN_MASK_EXP_P0 | 1)
#define PIN_OLED_CS             (PIN_MASK_EXPANDER | PIN_MASK_EXP_P0 | 2)
#define PIN_OLED_RESET          (PIN_MASK_EXPANDER | PIN_MASK_EXP_P0 | 0)
#define PIN_OLED_SHUTDOWN       (PIN_MASK_EXPANDER | PIN_MASK_EXP_P1 | 7)
#define PIN_SD_CS               (PIN_MASK_NATIVE   |  5)
#define PIN_SD_CHECK            (PIN_MASK_EXPANDER | PIN_MASK_EXP_P0 | 7)
#define PIN_LED_RED             (PIN_MASK_EXPANDER | PIN_MASK_EXP_P1 | 3)
#define PIN_LED_GREEN           (PIN_MASK_EXPANDER | PIN_MASK_EXP_P1 | 4)
#define PIN_LED_BLUE            (PIN_MASK_EXPANDER | PIN_MASK_EXP_P1 | 5)
#define PIN_BATTERY_LEVEL       (PIN_MASK_NATIVE   |  0)
#define PIN_BATTERY_CHARGING    (PIN_MASK_EXPANDER | PIN_MASK_EXP_P0 | 6)
#define PIN_LIGHT_SENSOR        (PIN_MASK_NATIVE   |  4)
#define PIN_MICROPHONE          (PIN_MASK_NATIVE   |  5)
#define PIN_ACCEL_INTERRUPT     (PIN_MASK_NATIVE   | 19)
#define PIN_EXPOSED_0           (PIN_MASK_EXPANDER | PIN_MASK_EXP_P1 | 0)
#define PIN_EXPOSED_1           (PIN_MASK_EXPANDER | PIN_MASK_EXP_P1 | 1)
#define PIN_EXPOSED_2           (PIN_MASK_EXPANDER | PIN_MASK_EXP_P1 | 2)
#define PIN_EXPOSED_3           (PIN_MASK_EXPANDER | PIN_MASK_EXP_P1 | 3)
#define PIN_EXPOSED_4           (PIN_MASK_NATIVE   |  3)
#define PIN_EXPOSED_5           (PIN_MASK_NATIVE   |  2)
*/

/* NEW DEFINITIONS */

static const uint8_t PIN_SPI_SCK            = (PIN_MASK_NATIVE   |  5);
static const uint8_t PIN_SPI_MOSI           = (PIN_MASK_NATIVE   |  6);
static const uint8_t PIN_SPI_MISO           = (PIN_MASK_NATIVE   | 10);
static const uint8_t PIN_I2C_SDA            = (PIN_MASK_NATIVE   |  8);
static const uint8_t PIN_I2C_SCL            = (PIN_MASK_NATIVE   |  9);
static const uint8_t PIN_EXPANDER_INTERRUPT = (PIN_MASK_NATIVE   | 20);
static const uint8_t PIN_ACCEL_INTERRUPT    = (PIN_MASK_NATIVE   |  7);
static const uint8_t PIN_BUTTON_0           = (PIN_MASK_EXPANDER | PIN_MASK_EXP_P0 | 6);
static const uint8_t PIN_BUTTON_1           = (PIN_MASK_EXPANDER | PIN_MASK_EXP_P0 | 5);
static const uint8_t PIN_BUTTON_2           = (PIN_MASK_EXPANDER | PIN_MASK_EXP_P0 | 1);
static const uint8_t PIN_OLED_DC            = (PIN_MASK_EXPANDER | PIN_MASK_EXP_P1 | 5);
static const uint8_t PIN_OLED_CS            = (PIN_MASK_EXPANDER | PIN_MASK_EXP_P0 | 7);
static const uint8_t PIN_OLED_RESET         = (PIN_MASK_EXPANDER | PIN_MASK_EXP_P1 | 4);
static const uint8_t PIN_OLED_SHUTDOWN      = (PIN_MASK_EXPANDER | PIN_MASK_EXP_P0 | 2);
static const uint8_t PIN_SD_CS              = (PIN_MASK_NATIVE   | 21);
static const uint8_t PIN_SD_CHECK           = (PIN_MASK_EXPANDER | PIN_MASK_EXP_P0 | 4);
static const uint8_t PIN_LED_RED            = (PIN_MASK_EXPANDER | PIN_MASK_EXP_P1 | 2);
static const uint8_t PIN_LED_GREEN          = (PIN_MASK_EXPANDER | PIN_MASK_EXP_P1 | 3);
static const uint8_t PIN_LED_BLUE           = (PIN_MASK_EXPANDER | PIN_MASK_EXP_P0 | 0);
static const uint8_t PIN_BATTERY_LEVEL      = (PIN_MASK_NATIVE   |  2);
static const uint8_t PIN_BATTERY_CHARGING   = (PIN_MASK_EXPANDER | PIN_MASK_EXP_P0 | 3);
static const uint8_t PIN_LIGHT_SENSOR       = (PIN_MASK_NATIVE   |  1);
static const uint8_t PIN_MICROPHONE         = (PIN_MASK_NATIVE   |  0);
static const uint8_t PIN_EXPOSED_0          = (PIN_MASK_NATIVE   |  4);
static const uint8_t PIN_EXPOSED_1          = (PIN_MASK_EXPANDER | PIN_MASK_EXP_P1 | 1);
static const uint8_t PIN_EXPOSED_2          = (PIN_MASK_EXPANDER | PIN_MASK_EXP_P1 | 0);
static const uint8_t PIN_EXPOSED_3          = (PIN_MASK_EXPANDER | PIN_MASK_EXP_P1 | 7);
static const uint8_t PIN_EXPOSED_4          = (PIN_MASK_EXPANDER | PIN_MASK_EXP_P1 | 6);
static const uint8_t PIN_EXPOSED_5          = (PIN_MASK_NATIVE   |  3);


static const uint8_t PIN_I2C_SDA_POCKET            = (7);
static const uint8_t PIN_I2C_SCL_POCKET           = (8);


#endif // Pocuter_Pins_h