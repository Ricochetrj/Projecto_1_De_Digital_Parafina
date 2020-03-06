/****************************************
    * File:    ADC.c                *
     * Date:    23/02/2020                   *
     * Author:  Rodrigo Figueroa             *
     * Prof:     Pablo Mazariegos            *
     * Seccion:     20                       *
     * Clase:   Digital 2                    *
     * Compiler:    MPLAB XC8 v2.10          *
     * Arquitectura:    PIC16F887            *
     * Descripcion:  Projecto en donde un PIC maestro controla a otros 3 PICs
     * Utilizando Comunicacion I2C
     * 
     * 
     * Asignacion de Pins:
     * Puerto B = Entrada del Potenciometro
     *          
     * Puerto A = Nada
     * 
     * Puerto C = Salida de Clock y entrada de datos de los Slaves
     * 
     * Puerto D = Nada
     *                      
     * 
     * 
     *
     * Link al Github: https://github.com/Ricochetrj/Lab_5_I2C.git
     * 
     ****************************************/ 

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

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
//*****************************************************************************
// Librerias
//*****************************************************************************
#include <xc.h>
#include <stdint.h>
#include "ProjectoLaser.h"
//*****************************************************************************
// Variables
//*****************************************************************************
short z;
uint8_t adcval;
uint8_t masterval;
int adcsend;
uint8_t confirm;
#define Laser RB7
//*****************************************************************************
// Interrupcion de Esclavo
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
    SSPBUF = confirm;
    SSPCONbits.CKP = 1;
     __delay_us(250);
    while(SSPSTATbits.BF);
  }

  PIR1bits.SSPIF = 0;
  }
  return;
}


//*****************************************************************************
// Funcion Principal
//*****************************************************************************
void main(void) {
    I2C_Slave_Init(0x50); //Iniciar PIC como Esclavo
    TRISB = 0b10000000;
    ANSELH = 0;
    PORTB = 0;
    while(1){
        if(Laser == 1){
            confirm = 0;
        }
        if(Laser == 0){
            confirm = 1;
        }
    }
}

