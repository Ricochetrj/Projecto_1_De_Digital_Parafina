/********************************************************************************************
    * File:    ProjectoPeso.h                                                               *
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
// Definicion de Cabecera
//*****************************************************************************
#ifndef PROJECTOPESO_H
#define	PROJECTOPESO_H
//*****************************************************************************
// Librerias
//*****************************************************************************
#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#define _XTAL_FREQ 4000000
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//*****************************************************************************
// Definir Frecuencia
//*****************************************************************************
#define _XTAL_FREQ 4000000
//*****************************************************************************
// Libreria I2C por George Ligo
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


#endif	/* PROJECTOPESO_H */