#ifndef SPI_H
#define	SPI_H
#define _XTAL_FREQ 4000000
#include <xc.h> // include processor files - each processor file is guarded.  

//////////////// Tomado de Circuit Digest, Por Pablo Pereira (2014)///////////////
typedef enum
{
  SPI_MASTER_OSC_DIV4  = 0b00100000,
  SPI_MASTER_OSC_DIV16 = 0b00100001,
  SPI_MASTER_OSC_DIV64 = 0b00100010,
  SPI_MASTER_TMR2      = 0b00100011,
  SPI_SLAVE_SS_EN      = 0b00100100,
  SPI_SLAVE_SS_DIS     = 0b00100101
}Spi_Type;

typedef enum
{
  SPI_DATA_SAMPLE_MIDDLE = 0b00000000,
  SPI_DATA_SAMPLE_END    = 0b10000000
}Spi_Data_Sample;

typedef enum
{
  SPI_CLOCK_IDLE_HIGH = 0b00001000,
  SPI_CLOCK_IDLE_LOW  = 0b00000000
}Spi_Clock_Idle;

typedef enum
{
  SPI_IDLE_2_ACTIVE = 0b00000000,
  SPI_ACTIVE_2_IDLE = 0b01000000
}Spi_Transmit_Edge;

void spiInit(Spi_Type sType, Spi_Data_Sample sDataSample, Spi_Clock_Idle sClockIdle, Spi_Transmit_Edge sTransmitEdge)
{
  TRISC5 = 0;
  if(sType & 0b00000100) //If Slave Mode
  {
    SSPSTAT = sTransmitEdge;
    TRISC3 = 1;
  }
  else //If Master Mode
  {
    SSPSTAT = sDataSample | sTransmitEdge;
    TRISC3 = 0;
  }
  SSPCON = sType | sClockIdle;
}

static void spiReceiveWait()
{
  while ( !SSPSTATbits.BF ); // Wait for Data Receipt complete
}

void spiWrite(char dat) //Write data to SPI bus
{
  SSPBUF = dat;
}

unsigned spiDataReady() //Check whether the data is ready to read
{
  if(SSPSTATbits.BF)
    return 1;
  else
    return 0;
}

char spiRead()    // Read the received data
{
  spiReceiveWait();      // Wait until all bits receive
  return(SSPBUF); // Read the received data from the buffer
}
//UART

/////Tomado de embeded Circuits///////////////////
void UARTInit(const uint32_t baud_rate, const uint8_t BRGH) {
    // Calculate BRG
    if (BRGH == 0) {
        SPBRG = _XTAL_FREQ/(64*baud_rate) - 1;
        TXSTAbits.BRGH = 0;
    } else {
        SPBRG = _XTAL_FREQ/(16*baud_rate) - 1;
        TXSTAbits.BRGH = 1;
    }
    // TXSTA register
    TXSTAbits.TX9 = 0;      // 8-bit transmission
    TXSTAbits.TXEN = 1;     // Enable transmission
    TXSTAbits.SYNC = 0;     // Asynchronous mode
    
    // RCSTA register
    RCSTAbits.SPEN = 1;     // Enable serial port
    RCSTAbits.RX9 = 0;      // 8-bit reception
    RCSTAbits.CREN = 1;     // Enable continuous reception
    RCSTAbits.FERR = 0;     // Disable framing error
    RCSTAbits.OERR = 0;     // Disable overrun error
    
    // Set up direction of RX/TX pins
    //UART_TRIS_RX = 1;
    //UART_TRIS_TX = 0;
    TRISCbits.TRISC7 =0; //RX
    TRISCbits.TRISC6 =1;//TX
}
char UART_TX_Empty()
{
  return TRMT;
}

char UART_Data_Ready()
{
   return RCIF;
}
char UART_Read()
{
 
  while(!RCIF);
  return RCREG;
}

void UART_Read_Text(char *Output, unsigned int length)
{
	int i;
	for(int i=0;i<length;i++)
		Output[i] = UART_Read();
}

void UART_Write(char data)
{
  while(!TRMT);
  TXREG = data;
}

void UART_Write_int(int data){
    while(!TRMT);
    TXREG = data;
}

void UART_Write_Text(char *text)
{
  int i;
  for(i=0;text[i]!='\0';i++)
	  UART_Write(text[i]);
}

#endif	/* SPI_H */



