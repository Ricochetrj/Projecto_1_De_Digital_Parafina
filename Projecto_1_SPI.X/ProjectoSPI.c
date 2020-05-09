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
unsigned char *Temp[3];
unsigned char *Luz[5];
unsigned char *Prof[2];
unsigned char *Fuego[5];
unsigned char *Dist[3];



void main(void) {
    UARTInit(9600,1); //Iniciar UART en baudrate de 9600, en HS mode
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE); //Modo Spi Maestro
    //spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE); //Modo Spi MaestrospiInit(SPI_SLAVE_SS_DIS, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    TRISA = 0;
    ANSEL = 0;
    PORTA = 0;
    TRISB = 0;
    while(1){
    ///////Profundidad
        if(UART_Data_Ready()){
        UART_Read_Text(Prof,2);
        PORTA = 0b00000001;
        
    }
        PORTB = spiRead();
        spiWrite(Prof);
        
        
    //////Temperatura
        if(UART_Data_Ready()){
        UART_Read_Text(Temp,3);
        PORTA = 0b00000010;
        
    }
        PORTB = spiRead();
        spiWrite(Temp);
        
        
    /////Incendio
        if(UART_Data_Ready()){
        UART_Read_Text(Fuego,5);
        PORTA = 0b00000100;
    }
        PORTB = spiRead();
        spiWrite(Fuego);
        
       
    ////////Distancia    
        if(UART_Data_Ready()){
        UART_Read_Text(Dist,3);
        PORTA = 0b00001000;
    }
        PORTB = spiRead();
        spiWrite(Dist);
        
        
    ///////Infrarojo    
        if(UART_Data_Ready()){
        UART_Read_Text(Luz,5);
        PORTA = 0b00010000;
        
    }
        PORTB = spiRead();
        spiWrite(Luz);
        

    }
}

