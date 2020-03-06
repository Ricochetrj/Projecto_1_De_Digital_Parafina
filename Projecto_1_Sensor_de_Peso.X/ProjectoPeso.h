/****************************************
    * File:    Master_main.c                 *
     * Date:    23/02/2020                   *
     * Author:  Rodrigo Figueroa             *
     * Prof:     Pablo Mazariegos            *
     * Seccion:     20                       *
     * Clase:   Digital 2                    *
     * Compiler:    MPLAB XC8 v2.10          *
     * Arquitectura:    PIC16F887            *
     * Descripcion:  Projecto en donde un PIC maestro controla a otros 3 PICs
 *  Utilizando Comunicacion I2C
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
#ifndef PROJECTOPESO_H
#define	PROJECTOPESO_H

#include <xc.h> // include processor files - each processor file is guarded.  
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
// Asignacion de Pines para la Pantalla
//*****************************************************************************
#define RS PORTDbits.RD0
#define EN PORTDbits.RD1
#define _XTAL_FREQ 4000000
uint8_t valadc;
float voltaje;
unsigned char *buffer[10];
uint8_t i;
//*****************************************************************************
// Libreria I2C por George Ligo
//*****************************************************************************
void I2C_Master_Init(const unsigned long transmissionspeed)
{
  SSPCON = 0b00101000;            //SSP Module as Master
  SSPCON2 = 0;
  SSPADD = (_XTAL_FREQ/(4*transmissionspeed))-1; //Setting Clock Speed
  SSPSTAT = 0;
  TRISC3 = 1;                   //Setting as input as given in datasheet
  TRISC4 = 1;                   //Setting as input as given in datasheet
}

void I2C_Master_Wait()
{
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F)); //Transmit is in progress
}

void I2C_Master_Start()
{
  I2C_Master_Wait();    
  SSPCON2bits.SEN = 1;             //Initiate start condition
}

void I2C_Master_RepeatedStart()
{
  I2C_Master_Wait();
  SSPCON2bits.RSEN = 1;           //Initiate repeated start condition
}

void I2C_Master_Stop()
{
  I2C_Master_Wait();
  SSPCON2bits.PEN = 1;           //Initiate stop condition
}

void I2C_Master_Write(unsigned d)
{
  I2C_Master_Wait();
  SSPBUF = d;         //Write data to SSPBUF
}

unsigned short I2C_Master_Read(unsigned short a)
{
  unsigned short temp;
  I2C_Master_Wait();
  RCEN = 1;
  I2C_Master_Wait();
  temp = SSPBUF;      //Read data from SSPBUF
  I2C_Master_Wait();
  ACKDT = (a)?0:1;    //Acknowledge bit
  ACKEN = 1;          //Acknowledge sequence
  return temp;
}
//*****************************************************************************
// Libreria LCD
//*****************************************************************************
void lcd_puerto(char a){ // Mandar valores al puerto donde esta conectado la LCD
    PORTA = a;
}

void lcd_comando(char a){ // Apagar RS para que la LCD sepa que se le esta mandando un comando y no datos
    RS = 0;
    lcd_puerto(a);
    EN = 1;
    __delay_us(5);
    EN = 0;
}

void lcd_clear(void){ // Limpiar pantalla de la LCD
	lcd_comando(0);
	lcd_comando(1);
}
void lcd_cursor(char a, char b){ // Poner el cursor de la LCD en una posicion para que imprima caracteres ahi
	char temp;
	if(a == 1)
	{
	  temp = 0x80 + b - 1;
      lcd_comando(temp);
	}
	else if(a == 2)
	{
		temp = 0xC0 + b - 1;
		
        lcd_comando(temp);
	}
    __delay_ms(4);
}

void lcd_init() // Mandar los comandos segun la guia, para inicializar la LCD
{
 
   __delay_ms(20);
  lcd_comando(0x30);
	__delay_ms(10);
  lcd_comando(0x30);
	__delay_us(110);
  lcd_comando(0x30);
	__delay_ms(11);
 
  /////////////////////////////////////////////////////
  lcd_comando(0x38);
  __delay_ms(4);
  lcd_comando(0x08);
  __delay_ms(4);
  lcd_comando(0x01);
  __delay_ms(4);
  lcd_comando(0x06);
  __delay_ms(4);
  lcd_comando(0x0C);
  __delay_ms(4);
}

void lcd_char(char a) // Mandar un solo character a la LCD
{
   RS = 1;             // => RS = 1
   lcd_puerto(a);             //Data transfer
   EN = 1;
   __delay_us(40);
   EN = 0;
}



void lcd_palabra(char *a) // Mandar string de caracteres y leerlos 1 por 1
{
	int i;
	for(i=0;a[i]!='\0';i++)
	   lcd_char(a[i]);
}

void Lcd_Shift_Right()// Correr la pantalla a la Derecha
{
	lcd_comando(0x01);
	lcd_comando(0x0C);
}

void Lcd_Shift_Left() // Correr la Pantalla a la izquierda
{
	lcd_comando(0x01);
	lcd_comando(0x08);
}
//*****************************************************************************
// Funciones Adicionales
//*****************************************************************************
////Tomado de Geeks- Fuuncion para convertir Floats y Doubles en Strings
void reverse(char* str, int len){
    int i = 0, j= len - 1, temp;
    while (i<j){
        temp = str[  i ];
        str[i] = str[j];
        str[j] = temp ;
        i++;
        j--;
    }
}

int intToStr (int x, char str[], int d){
    int i = 0;
    while(x){
        str[i++] = (x % 10) + '0';
        x =x/10;
    }
    while(i<d){
        str[i++] = '0';
        reverse(str,i);
        str[i] = '\0';
        return i;
    }
}

void ftoa2(float n, char* res, int afterpoint){
    int ipart = (int)n;
    float fpart = n - (float)ipart;
    int i = intToStr(ipart, res, 0);
    if(afterpoint != 0){
        res[i] = '.';
        fpart = fpart* pow(10,afterpoint);
        intToStr((int)fpart, res +1,afterpoint);
                
    }
}

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
#endif	/* PROJECTOPESO_H */