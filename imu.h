#ifndef IMU_H_
#define IMU_H_

#include "xmc_spi.h"
#include "xmc_gpio.h"

/****************************************************************
* MACROS AND DEFINES
***************************************************************/
#ifdef XMC4800_F144x2048
#define IMU_SPI_MISO 	P3_7
#define IMU_SPI_MOSI 	P3_8
#define IMU_SPI_SCLK   	P3_9
#define IMU_SPI_SS 		P3_10
#endif

enum SPIAction
{
    SPI_READ    = 0b10000000,
    SPI_WRITE   = 0b00000000
};

enum IMUregisteraddresses
{
    ADDR_ACCEL_ANTI_ALIASING    = 0x0U,     /**< Anti aliasing register address [0-1] */
    ADDR_ACCEL_SCALE            = 0x2U,     /**< Accelerometer scale register address [2-3] */
    ADDR_ACCEL_MODE             = 0x4U,     /**< Accelerometer mode register address [4-7] */
    ADDR_GYRO_SCALE             = 0x2U,     /**< Gyroscope scale register address [2-3] */
    ADDR_GYRO_MODE              = 0x4U,     /**< Gyroscope mode register address [4-7] */
    ADDR_HPCF_G                 = 0x5U,     /**< HPCF G register address */
    ADDR_HP_G_EN                = 0x7U,     /**< HP G register address */
    ADDR_CTRL1_XL               = 0x10U,    /**< CTRL1_XL register address (accelerometer configuration) */
    ADDR_CTRL2_G                = 0x11U,    /**< CTRL2_G register address (gyroscope configuration) */
    ADDR_CTRL3_C                = 0x12U,    /**< CTRL3_C register address (general configuration) */
    ADDR_CTRL4_C                = 0x13U,    /**< CTRL4_C register address (general configuration) */
    ADDR_CTRL5_C                = 0x14U,    /**< CTRL5_C register address (general configuration) */
    ADDR_CTRL6_C                = 0x15U,    /**< CTRL6_C register address (general configuration) */
    ADDR_CTRL7_G                = 0x16U,    /**< CTRL7_C register address (gyroscope configuration) */
    ADDR_CTRL8_XL               = 0x17U,    /**< CTRL8_XL register address (accelerometer configuration) */
    ADDR_TEMP                   = 0x20U,    /**< Temperature register address */
    ADDR_GYRO_X                 = 0x22U,    /**< Gyroscope first byte axis X register address */
    ADDR_GYRO_Y                 = 0x24U,    /**< Gyroscope first byte axis Y register address */
    ADDR_GYRO_Z                 = 0x26U,    /**< Gyroscope first byte axis Z register address */
    ADDR_ACCEL_X                = 0x28U,    /**< Accelerometer first byte axis X register address */
    ADDR_ACCEL_Y                = 0x2AU,    /**< Accelerometer first byte axis Y register address */
    ADDR_ACCEL_Z                = 0x2CU,    /**< Accelerometer first byte axis Z register address */
    ADDR_ORIENT                 = 0x0U,     /**< Orientation register address [0-2] */
    ADDR_REG_IF_INC             = 0x2U,     /**< I2C slave address register address */
    ADDR_REG_I2C_DISABLE        = 0x2U      /**< I2C disable mode register address */
};

enum IMUConfiguration
{
    /* Register CTRL1_XL : 0x10 bits masks */
    ACC_GYRO_ODR_XL_POWER_DOWN  = 0b00000000,
    ACC_GYRO_ODR_XL_1660Hz      = 0b10000000,

    ACC_GYRO_ODR_G_POWER_DOWN   = 0b00000000,
    ACC_GYRO_ODR_G_1660Hz       = 0b10000000,

    LPF1_XL_BW_SEL      = 0b00000010,   /**< Accelerometer low pass filter 1 selection (logic or) */
    ACCEL_SELECT_RESET  = 0b11110011,   /**< Accelerometer sensitivity reset mask (logic and) */
    ACCEL_SELECT_2G     = 0b00000000,   /**< Accelerometer sensitivity of 2G (logic or) */
    ACCEL_SELECT_4G     = 0b00001000,   /**< Accelerometer sensitivity of 4G (logic or) */
    ACCEL_SELECT_8G     = 0b00001100,   /**< Accelerometer sensitivity of 8G (logic or) */
    ACCEL_SELECT_16G    = 0b00000100,   /**< Accelerometer sensitivity of 16G (logic or) */

    ACCEL_MODE          = 0b10000000,   /**< Run the Accelerometer 1666 Hz (logic or) */
    BW0_XL              = 0b00000001,   /**< Accelerometer analog chain bandwidth selection. I is 400Hz and
                                             0 is 1.5kHz (logic or) */

    /* Register CTRL2_G : 0x11 bits masks */
    GYRO_SELECT_RESET   = 0b11110001,   /**< Gyroscope sensitivity reset mask (logic and) */
    GYRO_SELECT_125DPS  = 0b00000010,   /**< Gyroscope sensitivity of 125 dps (logic or) */
    GYRO_SELECT_250DPS  = 0b00000000,   /**< Gyroscope sensitivity of 250 dps (logic or) */
    GYRO_SELECT_500DPS  = 0b00000100,   /**< Gyroscope sensitivity of 500 dps (logic or) */
    GYRO_SELECT_1000DPS = 0b00001000,   /**< Gyroscope sensitivity of 1000 dps (logic or) */
    GYRO_SELECT_2000DPS = 0b00001100,   /**< Gyroscope sensitivity of 2000 dps (logic or) */

    GYRO_MODE           = 0b10000000U,  /**< Run the Gyroscope at 1666 Hz (logic or) */

    /* Register CTRL_3C : 0x12 bits mask */
    IF_INC              = 0b00000100U,  /**< Automatic address increment in multiple bytes read (logic or) */
    BDU                 = 0b01000000U,  /**< Block data update mechanism(logic or) */

    /* Register CTRL4_C : 0x13 bits mask */
    LPF1_SEL_G          = 0b00000010,   /**< Gyroscope low pass filter activation bit (logic or) */
    I2C_DISABLE         = 0b00000100,   /**< Automatic address increment in multiple bytes read (logic or) */

    /* Register CTRL6_C : 0x15 bits mask */
    GYRO_FTYPE_RESET    = 0b11111100,   /**< Gyroscope low pass filter reset mask (logic and) */
    GYRO_FTYPE_00       = 0b00000000,   /**< Gyroscope low pass filter type 00 (logic or) */
    GYRO_FTYPE_01       = 0b00000001,   /**< Gyroscope low pass filter type 01 (logic or) */
    GYRO_FTYPE_10       = 0b00000010,   /**< Gyroscope low pass filter type 10 (logic or) */
    GYRO_FTYPE_11       = 0b00000011,   /**< Gyroscope low pass filter type 11 (logic or) */

    /* Register CTRL7_G : 0x16 bits masks */
    HP_EN_G_RESET       = 0b10001111,   /**< Gyroscope high pass filter reset mask (logic and) */
    HP_EN_G             = 0b01000000,   /**< Gyroscope high pass filter activation (logic or) */
    HPM_G_16mHZ         = 0b01000000,   /**< Gyroscope high pass filter of 16 mHz (logic or) */
    HPM_G_65mHZ         = 0b01010000,   /**< Gyroscope high pass filter of 65 mHz (logic or) */
    HPM_G_260mHZ        = 0b01100000,   /**< Gyroscope high pass filter of 260 mHz (logic or) */
    HPM_G_1040mHZ       = 0b01110000,   /**< Gyroscope high pass filter of 1040 mHz (logic or) */

    /* Register CTRL8_XL : 0x17 bits masks */
    LPF2_XL_EN          = 0b10000000,   /**< Accelerometer low pass filter 2 enable (logic or) */
    HPCF_XL             = 0b01100000,   /**< Accelerometer high pass filter enable (logic or) */
    INPUT_COMPOSITE     = 0b00001000,   /**< Input composite selection [0 = ODR/2 LPF ; 1 = ODR / 4 LPF]
                                             (logic or) */
    HP_SLOPE_XL_EN      = 0b00000100,   /**< Accelerometer slope filter (logic or) */
};


/****************************************************************
* API PROTOTYPES
***************************************************************/
void initSPICom(void);
void configureIMU(void);

void sendWord(XMC_USIC_CH_t *const channel, uint16_t* data);

#endif