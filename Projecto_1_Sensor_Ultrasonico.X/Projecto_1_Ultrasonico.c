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
#include "ProjectoUltrasonico.h"
//*****************************************************************************
// Variables
//*****************************************************************************
#define Trigger RB1 //34 is Trigger
#define Echo RB2//35 is Echo 
short z;
uint8_t distancia;
uint8_t masterval;
uint8_t tiempo;
int adcsend;
char d1,d2,d3;
//*****************************************************************************
// Interrupcion de Esclavo: Interrupcion de esclavo que activa el buffer para  recibir la senal de reloj del master
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
    SSPBUF = distancia;
    SSPCONbits.CKP = 1;
     __delay_us(250);
    while(SSPSTATbits.BF);
  }

  PIR1bits.SSPIF = 0;
  }
  return;
}


//*****************************************************************************
// Funcion Principal: Se manda un pulso al pin de trigger, luego de que se manda
//el pulso al trigger, este emite un un pulso de sonido de alta frecuecia que es
//inaudible. Luego el sensor espera a que regrese el echo de ese pulso, y cuando 
//Recibe el echo y activa una senal en alto. Usando una funcion matematica que transforma
//El tiempo transcurrido (Cuyo valor esta guardado en el Timer1)
//a una distancia en cm, mandamos dicha informacion por I2C al Master.
//*****************************************************************************
void main(void) {
    I2C_Slave_Init(0x40); //Iniciar PIC como Esclavo
    TRISB = 0b00000101;
    ANSELH = 0;
    PORTB = 0;
    T1CON=0x20;
    while(1){
        TMR1H =0; TMR1L =0; //clear the timer bits
        Trigger = 1; 
        __delay_us(10);           
        Trigger = 0;  
        while (Echo==0){
            
        }
            TMR1ON = 1;
        while (Echo==1){
            
        }
            TMR1ON = 0;
            
        tiempo = (TMR1L | (TMR1H<<8)); 
        distancia= (0.136*tiempo)/2;
    }
}

