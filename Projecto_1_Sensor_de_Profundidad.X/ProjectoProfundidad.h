/********************************************************************************************
    * File:    ProjectoProfundidad.h                                                        * 
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
#ifndef PROJECTOPROFUNDIDAD_H
#define	PROJECTOPROFUNDIDAD_H 
//*****************************************************************************
// Librerias
//*****************************************************************************
#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//*****************************************************************************
// Definir Frecuencia
//*****************************************************************************
#define _XTAL_FREQ 4000000
//*****************************************************************************
// Variables
//*****************************************************************************
uint8_t valadc;
float voltaje;
unsigned char *buffer[10];
uint8_t i;
//*****************************************************************************
// Funcion de I2C por Ligo George
//*****************************************************************************
void I2C_Slave_Init(short address) 
{
  SSPSTAT = 0x80;    
  SSPADD = address; //Setting address
  SSPCON = 0x36;    //As a slave device
  SSPCON2 = 0x01;
  TRISC3 = 1;       //Setting as input as given in datasheet
  TRISC4 = 1;       //Setting as input as given in datasheet
  GIE = 1;          //Global interrupt enable
  PEIE = 1;         //Peripheral interrupt enable
  SSPIF = 0;        //Clear interrupt flag
  SSPIE = 1;        //Synchronous serial port interrupt enable
}

//*****************************************************************************
// Funciones del ADC
//*****************************************************************************
void ADCinit(void){
    ADCON1bits.ADFM =0;// Justificacion izquierda y voltajes de referencia VDD y VSS
    ADCON1bits.VCFG0 =0;
    ADCON1bits.VCFG1 =0;
    ADCON0bits.ADCS0 =1;// Iniciar ADC y determinar como va a leer
    ADCON0bits.ADCS1 =0;
    ADCON0bits.ADON = 1;
                   }


void ADCread(){ // Iniciar lectura de ADC, Esperar a que termine de leer, convertir dato de 0-1024 a 0-5 y desplegar los decimales en la LCD 
    ADCON0bits.GO_DONE=1;
    while(ADCON0bits.GO_DONE);
    valadc= ADRESH;
    voltaje=valadc;
    //itoa(buffer,voltaje,10);
    __delay_ms(50);
}
#endif	/* PROJECTOPROFUNDIDAD_H */