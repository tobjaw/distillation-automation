/**
 * @file AD7792.h
 *
 * Interface to external AD7792 AD converters.
 *
 * Requires populating the first and second hardware extension slot with a
 * AD7792. Each converter needs to be setup with two Pt1000 temperature sensors,
 * even though only the first is actually queried.
 *
 * @todo allow single temperature sensor configuration
 *
 * @todo fix temperature drop when heater is powered on
 */

#define AD7792_RDY PB3
#define AD7792_RDY_PORT PORTB
#define AD7792_RDY_PIN PINB
#define AD7792_RDY_DDR DDRB

#define AD7792_REG_COMM 0
#define AD7792_REG_STATUS 0
#define AD7792_REG_MODE 1
#define AD7792_REG_CONFIG 2
#define AD7792_REG_DATA 3
#define AD7792_REG_ID 4
#define AD7792_REG_IO 5
#define AD7792_REG_OFFSET 6
#define AD7792_REG_FULL_SCALE 7

#define AD7792_COMM_WEN (1 << 7)
#define AD7792_COMM_WRITE (0 << 6)
#define AD7792_COMM_READ (1 << 6)
#define AD7792_COMM_ADDR(x) (((x)&0x7) << 3)
#define AD7792_COMM_CREAD (1 << 2)

#define AD7792_MODEL_ID 0xA
#define AD7792_MODEL_MASK 0xF

#define AD7792_CONFIG_DEFAULT 0x0710

#define AD7792_CONFIG_BIAS_SEL(x) (x << 14)
#define AD7792_CONFIG_BIAS_0 0 // disabled
#define AD7792_CONFIG_BIAS_1 1 // connected to AIN1(-)
#define AD7792_CONFIG_BIAS_2 2 // connected to AIN2(-)

#define AD7792_CONFIG_BURNOUT_DISABLE (0 << 13)
#define AD7792_CONFIG_BURNOUT_ENABLE (1 << 13)

#define AD7792_CONFIG_POL_BIPOLAR (0 << 12)
#define AD7792_CONFIG_POL_UNIPOLAR (1 << 12)

#define AD7792_CONFIG_BOOST_DISABLE (0 << 11)
#define AD7792_CONFIG_BOOST_ENABLE (1 << 11)

#define AD7792_CONFIG_GAIN_SEL(x) (((x)&0x7) << 8)
#define AD7792_CONFIG_GAIN_LEVEL_1 0
#define AD7792_CONFIG_GAIN_LEVEL_2 1
#define AD7792_CONFIG_GAIN_LEVEL_4 2
#define AD7792_CONFIG_GAIN_LEVEL_8 3
#define AD7792_CONFIG_GAIN_LEVEL_16 4
#define AD7792_CONFIG_GAIN_LEVEL_32 5
#define AD7792_CONFIG_GAIN_LEVEL_64 6
#define AD7792_CONFIG_GAIN_LEVEL_128 7

#define AD7792_CONFIG_REF_EXTERNAL (0 << 7)
#define AD7792_CONFIG_REF_INTERNAL (1 << 7)

#define AD7792_CONFIG_BUFFER_DISABLE (0 << 4)
#define AD7792_CONFIG_BUFFER_ENABLE (1 << 4)

#define AD7792_CONFIG_CHANNEL_SEL(x) (x << 0)
#define AD7792_CONFIG_CHANNEL_0 0
#define AD7792_CONFIG_CHANNEL_1 1
#define AD7792_CONFIG_CHANNEL_2 2
#define AD7792_CONFIG_CHANNEL_3 3
#define AD7792_CONFIG_CHANNEL_4 4
#define AD7792_CONFIG_CHANNEL_6 6
#define AD7792_CONFIG_CHANNEL_7 7

#define AD7792_IO_CURRENT_SRC_SEL(x) (x << 2)
#define AD7792_IO_CURRENT_SRC_0 0
#define AD7792_IO_CURRENT_SRC_1 1
#define AD7792_IO_CURRENT_SRC_2 2
#define AD7792_IO_CURRENT_SRC_3 3
#define AD7792_IO_EXCITATION_SEL(x) (x << 0)
#define AD7792_IO_EXCITATION_0 0 // disable
#define AD7792_IO_EXCITATION_1 1 // 10 μA
#define AD7792_IO_EXCITATION_2 2 // 210 μA
#define AD7792_IO_EXCITATION_3 3 // 1 mA

#define AD7792_MODE_DEFAULT 0x000A
#define AD7792_MODE_SEL(x) (x << 13)
#define AD7792_MODE_CC 0
#define AD7792_MODE_SC 1
#define AD7792_MODE_IDLE 2
#define AD7792_MODE_POWER_DOWN 3
#define AD7792_MODE_CALIBRATION_INTERNAL_ZERO 4
#define AD7792_MODE_CALIBRATION_INTERNAL_FULL 5
#define AD7792_MODE_CALIBRATION_SYSTEM_ZERO 6
#define AD7792_MODE_CALIBRATION_SYSTEM_FULL 7
#define AD7792_MODE_CLOCK_SEL(x) (x << 6)
#define AD7792_MODE_CLOCK_0 0
#define AD7792_MODE_CLOCK_1 1
#define AD7792_MODE_CLOCK_2 2
#define AD7792_MODE_CLOCK_3 3
#define AD7792_MODE_FILTER_SEL(x) (x << 0)
#define AD7792_MODE_FILTER_0 0
#define AD7792_MODE_FILTER_1 1
#define AD7792_MODE_FILTER_2 2
#define AD7792_MODE_FILTER_3 3
#define AD7792_MODE_FILTER_4 4
#define AD7792_MODE_FILTER_5 5
#define AD7792_MODE_FILTER_6 6
#define AD7792_MODE_FILTER_7 7
#define AD7792_MODE_FILTER_8 8
#define AD7792_MODE_FILTER_9 9
#define AD7792_MODE_FILTER_10 10
#define AD7792_MODE_FILTER_11 11
#define AD7792_MODE_FILTER_12 12
#define AD7792_MODE_FILTER_13 13
#define AD7792_MODE_FILTER_14 14
#define AD7792_MODE_FILTER_15 15

#define AD7792_STATUS_RDY (1 << 7)

#include "api.h"

/**
 * Select a specific SPI slave
 *
 * @param  slot AD7729 controller slot
 */
void selectSlave(TempSlot slot);

/**
 * Deselect a specific SPI slave
 *
 * @param  slot AD7729 controller slot
 */
void deselectSlave(TempSlot slot);

/**
 * Read bytes from a AD7792
 *
 * @param  addr   register address to read from
 * @param  length amount of bytes to read
 * @param  slot   AD7792 slot
 */
unsigned short AD7792_read(unsigned char addr, unsigned char length,
                           TempSlot slot);

/**
 * Write bytes to a AD7792
 *
 * @param  addr   register address to write to
 * @param  data   data to write
 * @param  length amount of bytes to write
 * @param  slot   AD7792 slot
 */
void AD7792_write(unsigned char addr, unsigned short data, unsigned int length,
                  TempSlot slot);

/**
 * Write bytes to a AD7792 and verify
 *
 * After writing, read back register data and confirm it matches @a data.
 * @warning this will only work for idempotent registers
 *
 * @param  addr   register address to write to
 * @param  data   data to write
 * @param  length amount of bytes to write
 * @param  slot   AD7792 slot
 *
 * @return 1 if data matches, 0 else
 */
int AD7792_write_verify(unsigned char addr, unsigned short data,
                        unsigned int length, TempSlot slot);

/**
 * Wait until AD7792 is ready
 *
 * The AD needs time for conversions; busy wait until it is ready.
 *
 * @param  slot AD7792 slot
 */
void AD7792_waitReady(TempSlot slot);

/**
 * Check connection to AD7792
 *
 * Read out model register and match.
 *
 * @param   slot AD7792 slot
 *
 * @return  1 if AD7792 connected, 0 else
 */
int AD7792_isConnected(TempSlot slot);

/**
 * Reset AD7792
 *
 * @param   slot AD7792 slot
 */
void AD7792_reset(TempSlot slot);

/**
 * Init AD7792
 *
 * @param   slot AD7792 slot
 *
 * @return  0 on success, positive int error code on error.
 */
unsigned char AD7792_init(TempSlot slot);

/**
 * Get temperature in degrees Celisus
 *
 * @param   slot AD7792 slot
 *
 * @return  temperature in degrees Celisus
 */
float AD7792_getTemperature(TempSlot slot);
