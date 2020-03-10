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
#include "ProjectoMaster.h"
//*****************************************************************************
// Declaracion de Variables
//*****************************************************************************
uint8_t Profundidad; // Address 0x10
uint8_t Temperatura; // Address 0x20
uint8_t Fuego;        // Address 0x30
uint8_t Distancia;   // Address 0x40
uint8_t Luz;         // Address 0x50
char d1,d2,d3;
double Rtemp;
//*****************************************************************************
// Subrutina de Inicio
//*****************************************************************************
void Start(){
  TRISA = 0;            //PORTA as output
  TRISD = 0;            //PORTD as output
  PORTA = 0;
  PORTD = 0;      
  TRISB =  0;
  PORTB = 0;
  I2C_Master_Init(100000);      //Initialize I2C Master with 100KHz clock
  lcd_init();
  lcd_clear();
  
  __delay_ms(250);
}

//*****************************************************************************
// Ecuacion Steinheart-Hart Para conversion de la temperatura
//*****************************************************************************
double Thermistor(int temperatura){
    double tempura;
    tempura = log(((10240000/temperatura)-1000));
    tempura = 1 / (0.001129148 + (0.000234125 * tempura) + (0.0000000876741 * tempura * tempura * tempura));
    tempura = tempura - 273.15;           // Convert Kelvin to Celcius
    Rtemp =  abs(tempura);
}


void main(){
    Start();
    lcd_clear();
    while(1){
        
    //Funcion Para declarar Nombres de Variables en la LCD
      lcd_cursor(1,1);// Poner texto de cursor en posicion 1
      lcd_palabra("Agua");
      __delay_ms(10);
      lcd_cursor(1,6);// Poner texto de cursor en posicion 2
      lcd_palabra("Temperatura");
      __delay_ms(10);
      lcd_cursor(1,18);// Poner texto de cursor en posicion 3
      lcd_palabra("Incendio");
      __delay_ms(10);
      lcd_cursor(1,27);// Poner texto de cursor en posicion 3
      lcd_palabra("Distancia");
      __delay_ms(10);
      lcd_cursor(1,36);// Poner texto de cursor en posicion 3
      lcd_palabra("Luz");
      __delay_ms(10);
   
    
    //Funcion para llamar Valor de Profundidad del array de I2C  
    I2C_Master_Start();         //Condicion de inicio
    I2C_Master_Write(0x11);     //Address
    Profundidad = I2C_Master_Read(0); //Mandar valor leido a variable
    I2C_Master_Stop();          //Condicion de Fin
    itoa(buffer,Profundidad,10);     //Convertir variable en String
    lcd_cursor(2,1);            //Desplegar en LCD
    lcd_palabra(buffer); 
    lcd_cursor(2,2);
    lcd_palabra("Cm");
    
    //Funcion para llamar Valor de Temperatura del array de I2C  
    I2C_Master_Start();         //Condicion de inicio
    I2C_Master_Write(0x21);     //Address
    Temperatura = I2C_Master_Read(0); //Mandar valor leido a variable
    I2C_Master_Stop();          //Condicion de Fin
    //Thermistor(Temperatura);           //Transformar variable en Temperatura
    Rtemp =Temperatura*150/255;
    itoa(buffer,Rtemp,10);      //Convertir en String
    lcd_cursor(2,9);            //Desplegar en LCD
    lcd_palabra(buffer); 
    lcd_cursor(2,11);
    lcd_palabra("C");
    if(Rtemp< 35){
        PORTBbits.RB7= 1;
    }
    if(Rtemp>= 35){
        PORTBbits.RB7= 0;
    }
    
    //Funcion para llamar Valor de el Sensor de Incendios del array de I2C  
    I2C_Master_Start();         //Condicion de inicio
    I2C_Master_Write(0x31);     //Address
    Fuego = I2C_Master_Read(0); //Mandar valor leido a variable
    I2C_Master_Stop();          //Condicion de Fin
    if(Fuego == 100){
        lcd_cursor(2,20);            //Desplegar en LCD
    lcd_palabra("Fuego");
    }
    else if(Fuego == 0){
        lcd_cursor(2,20);            //Desplegar en LCD
        lcd_palabra("Safe");
    }
    
    //Funcion para llamar Valor de Distancia del array de I2C  
    I2C_Master_Start();         //Condicion de inicio
    I2C_Master_Write(0x41);     //Address
    Distancia = I2C_Master_Read(0); //Mandar valor leido a variable
    I2C_Master_Stop();          //Condicion de Fin
     d1 = (Distancia/100)%10;
     d2 = (Distancia/10)%10;
     d3 = (Distancia/1)%10;
    //itoa(buffer,Distancia,10);     //Convertir variable en String
    lcd_cursor(2,30);            //Desplegar en LCD
    lcd_char(d1+'0');
    lcd_char(d2+'0');
    lcd_char(d3+'0');     
    
    //Funcion para llamar Valor de el Sensor de Luz del array de I2C  
    I2C_Master_Start();         //Condicion de inicio
    I2C_Master_Write(0x51);     //Address
    Luz = I2C_Master_Read(0); //Mandar valor leido a variable
    I2C_Master_Stop();          //Condicion de Fin
    if(Luz == 1){
        lcd_cursor(2,39);            //Desplegar en LCD
    lcd_palabra("Si");
    PORTBbits.RB6 = 1;
    __delay_ms(10);
    PORTBbits.RB6 = 0;
        
    }
    if(Luz == 0){
        lcd_cursor(2,37);            //Desplegar en LCD
    lcd_palabra("No");
    PORTBbits.RB6 = 0;
        
    }
    
    
//    lcd_clear();
    LCD_SCREEN_SHIFT(1);//mover a la izqueierda
    __delay_ms(200);
    }
    }
  
