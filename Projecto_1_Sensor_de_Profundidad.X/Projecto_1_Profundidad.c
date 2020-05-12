/********************************************************************************************
    * File:    Projecto_1_Profundidad.c                                                        * 
     * Date:    23/02/2020                                                                  *
     * Autores:  Rodrigo Figueroa, Gonzalo Palarea, Stefan Schwendenner                     *
     * Prof:     Pablo Mazariegos                                                           *
     * Seccion:     20                                                                      *
     * Clase:   Digital 2                                                                   *
     * Compiler:    MPLAB XC8 v2.10                                                         *
     * Arquitectura:    PIC16F887                                                           *
     * Descripcion:  Projecto 1 de Digital, en donde este PIC atravez de I2C recibe         *
     * Datos de 5 sensores, los despliega en una LCD, y los Manda atravez de UART a otro PIC*  
     *                                                                                      *
     *                                                                                      *
     * Asignacion de Pins:                                                                  *
     * Puerto B = Entrada del ADC RB5                                                       *
     *                                                                                      *
     *                                                                                      *
     *                                                                                      *
     * Puerto C = Salida de Clock y Salida de datos de los Datos  RC4,RC3                   *
     *                                                                                      *
     *                                                                                      *
     *                                                                                      *
     *                                                                                      *
     *                                                                                      *
     *                                                                                      *
     *                                                                                      *
     * Link al Github: https://github.com/Ricochetrj/Projecto_1_De_Digital_Parafina.git     *
     *                                                                                      *
     ****************************************************************************************/

//*****************************************************************************
// Bits de Configuracion
//*****************************************************************************

#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
//*****************************************************************************
// Librerias
//*****************************************************************************
#include <xc.h>
#include <stdint.h>
#include "ProjectoProfundidad.h"
//*****************************************************************************
// Variables
//*****************************************************************************
short z;
uint8_t adcval;
uint8_t masterval;
uint8_t deep;
int adcsend;
//*****************************************************************************
// Interrupcion de Esclavo- Por Ligo George, Interrupcion de esclavo que activa el buffer para  recibir la senal de reloj del master
// Luego de recbir la senal de reloj del master espera para revisar si el master lo esta llamando por nombre
//Una vez que el maestro lo llama, le escribe al maestro con la informacion almacenada el la variable 
//*****************************************************************************
void __interrupt() isr(void)
{
  if(SSPIF == 1)
  {
    SSPCONbits.CKP = 0;

    if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)) //If overflow or collision
    {
      z = SSPBUF; // Read the previous value to clear the buffer
      SSPCONbits.SSPOV = 0; // Clear the overflow flag
      SSPCONbits.WCOL = 0; // Clear the collision bit
      SSPCONbits.CKP = 1;
    }

  if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) //If last byte was Address + Write
  {
    z = SSPBUF;
    PIR1bits.SSPIF = 0;
    SSPCONbits.CKP = 1;
    while(!BF);
    masterval = SSPBUF;
     __delay_us(250);
    
  }
  else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW) //If last byte was Address + Read
  {
    z = SSPBUF;
    BF = 0;
    SSPBUF = deep;
    SSPCONbits.CKP = 1;
     __delay_us(250);
    while(SSPSTATbits.BF);
  }

  PIR1bits.SSPIF = 0;
  }
  return;
}


//*****************************************************************************
// Funcion Principal- Leemos el valor del ADC y utilizando 5 rangos definimos
// la profundidad del sensor, luego mandamos el valor de profundidad en cm al
// master en la interrupcion
//*****************************************************************************

void main(void) {
    ADCinit(); //Iniciar ADC
    I2C_Slave_Init(0x10); //Iniciar PIC como Esclavo
    TRISB = 0b00110000;
    TRISD = 0;
    ANSELH = 0b00100000;
    PORTB = 0;
    ADCON0bits.CHS = 13;
    while(1){
        ADCread(); //leer valor de ADC y mandarlo en la  interrupcion
        adcsend = voltaje;
        if(adcsend<100){
            deep = 0;
        }
        if(adcsend<110 && adcsend>100){
            deep = 1;
        }
        if(adcsend<120 && adcsend>110){
            deep = 2;
        }
        if(adcsend<130 && adcsend>120){
            deep = 3;
        }
        if(adcsend<140 && adcsend>130){
            deep = 4;
        }
        if(adcsend>140){
            deep = 5;
        }
    }
    
}
