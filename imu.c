#include "imu.h"
#include "utypes.h"
#include "time.h"

/****************************************************************
* MACROS AND DEFINES
***************************************************************/
XMC_SPI_CH_CONFIG_t imu_spi_config =
{
  .baudrate = 4000000,
  .bus_mode = XMC_SPI_CH_BUS_MODE_MASTER,
  .selo_inversion = XMC_SPI_CH_SLAVE_SEL_INV_TO_MSLS,
  .parity_mode = XMC_USIC_CH_PARITY_MODE_NONE
};


extern _Rbuffer    Rb;
extern _Wbuffer    Wb;
extern _Cbuffer    Cb;

#define READ_BUFFER_SIZE    14 
extern IMURawData_s lastRawIMU;
extern IMURawData_s currentRawIMU;

uint16_t count = 0;

/****************************************************************
* API IMPLEMENTATION
***************************************************************/
void initSPICom(void)
{
  /*Initialize and Start SPI*/
  XMC_SPI_CH_Init(XMC_SPI2_CH0, &imu_spi_config);
  XMC_SPI_CH_SetBitOrderMsbFirst(XMC_SPI2_CH0);
  XMC_SPI_CH_SetWordLength(XMC_SPI2_CH0, (uint8_t)8);
  XMC_SPI_CH_ConfigureShiftClockOutput(XMC_SPI2_CH0, XMC_SPI_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_0_DELAY_ENABLED, XMC_SPI_CH_BRG_SHIFT_CLOCK_OUTPUT_SCLK);
   
  XMC_USIC_CH_RXFIFO_Configure(XMC_SPI2_CH0, 0, XMC_USIC_CH_FIFO_SIZE_64WORDS, READ_BUFFER_SIZE);
  XMC_USIC_CH_RXFIFO_Flush(XMC_SPI2_CH0);
  
  /* Enable Interrupt */
  XMC_USIC_CH_RXFIFO_EnableEvent(XMC_SPI2_CH0, XMC_USIC_CH_RXFIFO_EVENT_CONF_STANDARD | XMC_USIC_CH_RXFIFO_EVENT_CONF_ALTERNATE);
  XMC_USIC_CH_EnableEvent(XMC_SPI2_CH0, XMC_USIC_CH_RXFIFO_EVENT_STANDARD | XMC_USIC_CH_RXFIFO_EVENT_ALTERNATE);
  XMC_SPI_CH_SetInterruptNodePointer(XMC_SPI2_CH0, 0);

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

    // Remove dummy bytes 0x00 (this byte correspond to read command)
    (void) XMC_SPI_CH_GetReceivedData(XMC_SPI2_CH0);

    // Read out data from FIFO
    int32_t rx_index = 0;
    uint8_t spi_rxbuf[READ_BUFFER_SIZE];
    while (rx_index < READ_BUFFER_SIZE) 
    { 
      spi_rxbuf[rx_index] = (uint8_t) XMC_SPI_CH_GetReceivedData(XMC_SPI2_CH0);
      rx_index++;
    }

    // TODO: remove 1st frame, seems to b 

    // populate IMU data
    lastRawIMU     = currentRawIMU;
    currentRawIMU  = fetchRawIMU(spi_rxbuf);
    
    XMC_SPI_CH_DisableSlaveSelect(XMC_SPI2_CH0);

    return;
}

IMURawData_s fetchRawIMU(uint8_t* buffer)
{  
  IMURawData_s imuPDOObject;

  int16_t temperature_raw = ((int16_t)buffer[1] << 8) + (int16_t)buffer[0];

  imuPDOObject.gyroscope[0] = ((((int16_t)buffer[3]) << 8) + (int16_t)buffer[2]);
  imuPDOObject.gyroscope[1] = ((((int16_t)buffer[5]) << 8) + (int16_t)buffer[4]);
  imuPDOObject.gyroscope[2] = ((((int16_t)buffer[7]) << 8) + (int16_t)buffer[6]);

  imuPDOObject.accelerometer[0] = ((((int16_t)buffer[9]) << 8) + (int16_t)buffer[8]);
  imuPDOObject.accelerometer[1] = ((((int16_t)buffer[11]) << 8) + (int16_t)buffer[10]);
  imuPDOObject.accelerometer[2] = ((((int16_t)buffer[13]) << 8) + (int16_t)buffer[12]);

  imuPDOObject.temperatureSensor = (temperature_raw / 256) + 25;
  imuPDOObject.timeStamp = (uint16_t) count;

  count++;

  return imuPDOObject;
}


void sendWord(XMC_USIC_CH_t *const channel, uint16_t data)
{
  XMC_SPI_CH_Transmit(channel, data, XMC_SPI_CH_MODE_STANDARD);

  /*Wait till the byte has been transmitted*/
  while((XMC_SPI_CH_GetStatusFlag(channel) & XMC_SPI_CH_STATUS_FLAG_TRANSMIT_SHIFT_INDICATION) == 0U);
  XMC_SPI_CH_ClearStatusFlag(channel, XMC_SPI_CH_STATUS_FLAG_TRANSMIT_SHIFT_INDICATION);
}

void configureIMU(void)
{
    // disable interrupt when transmit configuration
    NVIC_DisableIRQ(USIC2_0_IRQn);

    /*Enable Slave Select line 0*/
    XMC_SPI_CH_EnableSlaveSelect(XMC_SPI2_CH0, XMC_SPI_CH_SLAVE_SELECT_0);

    uint16_t imuConfigData[5];
    imuConfigData[0] = SPI_WRITE | ADDR_CTRL1_XL | 0x00; 
    imuConfigData[1] = ACC_GYRO_ODR_XL_1660Hz;
    imuConfigData[2] = ACC_GYRO_ODR_G_1660Hz;
    imuConfigData[3] = IF_INC | BDU;
    imuConfigData[4] = I2C_DISABLE;

    /*Sending data*/
    sendWord(XMC_SPI2_CH0, imuConfigData[0]);
    sendWord(XMC_SPI2_CH0, imuConfigData[1]);
    sendWord(XMC_SPI2_CH0, imuConfigData[2]);
    sendWord(XMC_SPI2_CH0, imuConfigData[3]);
    sendWord(XMC_SPI2_CH0, imuConfigData[4]);

    /*Disable Slave Select line */
    XMC_SPI_CH_DisableSlaveSelect(XMC_SPI2_CH0);

    // re-enable interrupt, start data receive
    XMC_USIC_CH_RXFIFO_Flush(XMC_SPI2_CH0);
    NVIC_EnableIRQ(USIC2_0_IRQn);
}

void readIMU(void)
{
    XMC_SPI_CH_EnableSlaveSelect(XMC_SPI2_CH0, XMC_SPI_CH_SLAVE_SELECT_0);
  
    uint16_t readCommand = SPI_READ | ADDR_TEMP | 0x00;
    sendWord(XMC_SPI2_CH0, readCommand);

    // Write dummy bytes just to keep the comm continue
    for (int32_t i = 0; i < READ_BUFFER_SIZE; i++)
    {
      XMC_SPI_CH_Receive(XMC_SPI2_CH0, XMC_SPI_CH_MODE_STANDARD);
    }
}
