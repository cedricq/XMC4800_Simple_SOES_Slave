#include "imu.h"

/****************************************************************
* MACROS AND DEFINES
***************************************************************/
XMC_SPI_CH_CONFIG_t imu_spi_config =
{
  .baudrate = 10000000,
  .bus_mode = XMC_SPI_CH_BUS_MODE_MASTER,
  .selo_inversion = XMC_SPI_CH_SLAVE_SEL_INV_TO_MSLS,
  .parity_mode = XMC_USIC_CH_PARITY_MODE_NONE
};

extern uint16_t spi_rxbuf[];
extern uint16_t spi_txbuf[];

/****************************************************************
* API IMPLEMENTATION
***************************************************************/
void initSPICom(void)
{
  /*Initialize and Start SPI*/
  XMC_SPI_CH_Init(XMC_SPI2_CH0, &imu_spi_config);
  XMC_SPI_CH_SetBitOrderMsbFirst(XMC_SPI2_CH0);
  XMC_SPI_CH_SetWordLength(XMC_SPI2_CH0, (uint8_t)16);
  XMC_SPI_CH_SetFrameLength(XMC_SPI2_CH0, (uint8_t)32);
  XMC_SPI_CH_ConfigureShiftClockOutput(XMC_SPI2_CH0,XMC_SPI_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_0_DELAY_ENABLED,XMC_SPI_CH_BRG_SHIFT_CLOCK_OUTPUT_SCLK);

  /* Enable Interrupt */
  XMC_SPI_CH_EnableEvent(XMC_SPI0_CH0, XMC_SPI_CH_EVENT_STANDARD_RECEIVE);
  XMC_SPI_CH_SetInterruptNodePointer(XMC_SPI0_CH0, 0);

  NVIC_ClearPendingIRQ(USIC2_0_IRQn);
  NVIC_SetPriority(USIC2_0_IRQn, 10U);
  NVIC_EnableIRQ(USIC2_0_IRQn);

  /*Input source selected*/
  XMC_SPI_CH_SetInputSource(XMC_SPI2_CH0, XMC_SPI_CH_INPUT_DIN0, USIC2_C0_DX0_P3_7);
  XMC_SPI_CH_Start(XMC_SPI2_CH0);

  /*GPIO configuration*/
  XMC_GPIO_SetMode(IMU_SPI_MOSI, XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1);
  XMC_GPIO_SetMode(IMU_SPI_SS, XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1);
  XMC_GPIO_SetMode(IMU_SPI_SCLK, XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1);
  XMC_GPIO_SetMode(IMU_SPI_MISO, XMC_GPIO_MODE_INPUT_TRISTATE);
}

void USIC2_0_IRQHandler(void)
{
    uint32_t fifo_flags = XMC_USIC_CH_RXFIFO_GetEvent(XMC_SPI2_CH0);
    XMC_USIC_CH_RXFIFO_ClearEvent(XMC_SPI2_CH0, fifo_flags);

    // Read out data from fifo
    uint32_t rdbytes;
    int32_t rx_index = 0;
    while ((rdbytes = XMC_USIC_CH_RXFIFO_GetLevel(XMC_SPI2_CH0))) {
        spi_rxbuf[rx_index] = (uint8_t) XMC_SPI_CH_GetReceivedData(XMC_SPI2_CH0);
        rx_index++;
    }

    return;
}

void sendWord(XMC_USIC_CH_t *const channel, uint16_t* data)
{
  XMC_SPI_CH_Transmit(channel, *data, XMC_SPI_CH_MODE_STANDARD);

    /*Wait till the byte has been transmitted*/
  while((XMC_SPI_CH_GetStatusFlag(channel) & XMC_SPI_CH_STATUS_FLAG_TRANSMIT_SHIFT_INDICATION) == 0U);
  XMC_SPI_CH_ClearStatusFlag(channel, XMC_SPI_CH_STATUS_FLAG_TRANSMIT_SHIFT_INDICATION);
}

void configureIMU(void)
{
    /*Enable Slave Select line 0*/
  XMC_SPI_CH_EnableSlaveSelect(XMC_SPI2_CH0, XMC_SPI_CH_SLAVE_SELECT_0);

  uint16_t imuConfigData[8];
  imuConfigData[0] = SPI_WRITE | ADDR_CTRL1_XL | 0x00;
  imuConfigData[1] = ACC_GYRO_ODR_XL_1660Hz;
  imuConfigData[2] = SPI_WRITE | ADDR_CTRL2_G | 0x00;
  imuConfigData[3] = ACC_GYRO_ODR_G_1660Hz;
  imuConfigData[4] = SPI_WRITE | ADDR_CTRL3_C | 0x00;
  imuConfigData[5] = IF_INC | BDU;
  imuConfigData[6] = SPI_WRITE | ADDR_CTRL4_C | 0x00;
  imuConfigData[7] = I2C_DISABLE;

  /*Sending data*/
  sendWord(XMC_SPI2_CH0, &imuConfigData[0]);
  sendWord(XMC_SPI2_CH0, &imuConfigData[1]);
  sendWord(XMC_SPI2_CH0, &imuConfigData[2]);
  sendWord(XMC_SPI2_CH0, &imuConfigData[3]);
  sendWord(XMC_SPI2_CH0, &imuConfigData[4]);
  sendWord(XMC_SPI2_CH0, &imuConfigData[5]);
  sendWord(XMC_SPI2_CH0, &imuConfigData[6]);
  sendWord(XMC_SPI2_CH0, &imuConfigData[7]);

  /*Disable Slave Select line */
  XMC_SPI_CH_DisableSlaveSelect(XMC_SPI2_CH0);

  // Try a read
  uint16_t readCommand = SPI_READ | ADDR_CTRL1_XL | 0x00;
  sendWord(XMC_SPI2_CH0, &readCommand);

  while((XMC_SPI_CH_GetStatusFlag(XMC_SPI2_CH0) & XMC_SPI_CH_STATUS_FLAG_RECEIVE_INDICATION) == 0U);
  XMC_SPI_CH_ClearStatusFlag(XMC_SPI2_CH0, XMC_SPI_CH_STATUS_FLAG_RECEIVE_INDICATION);

  spi_rxbuf[0] = XMC_SPI_CH_GetReceivedData(XMC_SPI2_CH0);
}
