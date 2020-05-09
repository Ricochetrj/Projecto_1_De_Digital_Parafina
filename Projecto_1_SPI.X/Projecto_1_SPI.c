/****************************************
    * File:    Projecto_1_Master.c                 *
     * Date:    04/03/2020                   *
     * Author:  Rodrigo Figueroa             *
     * Prof:     Pablo Mazariegos            *
     * Seccion:     20                       *
     * Clase:   Digital 2                    *
     * Compiler:    MPLAB XC8 v2.10          *
     * Arquitectura:    PIC16F887            *
     * Descripcion:  
     * 
     * 
     * Asignacion de Pins:
     * Puerto B = Nada
     *          
     * Puerto A = Salida de Pines D0-D7 de la LCD
     * 
     * Puerto C = Salida de Clock y entrada de datos de los Slaves
     * 
     * Puerto D = Pines RS y E de la LCD
     *                      
     * 
     * 
     *
     * Link al Github: https://github.com/Ricochetrj/Projecto_1_De_Digital_Parafina.git
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
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)


#include <xc.h>
#include <stdint.h>
#include "ProjectoSPI.h"
uint8_t Temp;
uint8_t Luz;
uint8_t Prof;
uint8_t Fuego;
uint8_t Dist;
void main(void) {
    UARTInit(9600,1); //Iniciar UART en baudrate de 9600, en HS mode
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE); //Modo Spi Maestro
    TRISA = 0;
    ANSEL = 0;
    PORTA = 0;
    ///////Profundidad
    while(1){
        if(UART_Data_Ready()){
        Prof =UART_Read_Text();
        PORTA = 0b00000001;
        
    }
        spiWrite(Prof);
        PORTB = spiRead();
    //////Temperatura
        if(UART_Data_Ready()){
        Temp =UART_Read_Text();
        PORTA = 0b00000010;
        
    }
        spiWrite(Temp);
        PORTB = spiRead();
    /////Incendio
        if(UART_Data_Ready()){
        Fuego =UART_Read_Text();
        PORTA = 0b00000100;
    }
        spiWrite(Fuego);
        PORTB = spiRead();
    ////////Distancia    
        if(UART_Data_Ready()){
        Dist =UART_Read_Text();
        PORTA = 0b00001000;
    }
        spiWrite(Dist);
        PORTB = spiRead();
    ///////Infrarojo    
        if(UART_Data_Ready()){
        Luz =UART_Read_Text();
        PORTA = 0b00010000;
        
    }
        spiWrite(Luz);
        PORTB = spiRead();
    }
}
