/****************************************
    * File:    Projecto_1_Master.c                 *
     * Date:    23/02/2020                   *
     * Autores:  Rodrigo Figueroa, Gonzalo Palarea, Stefan Schwendenner             *
     * Prof:     Pablo Mazariegos            *
     * Seccion:     20                       *
     * Clase:   Digital 2                    *
     * Compiler:    MPLAB XC8 v2.10          *
     * Arquitectura:    PIC16F887            *
     * Descripcion:  Projecto 1 de Digital, en donde este PIC atravez de I2C recibe
     * Datos de 5 sensores, los despliega en una LCD, y los Manda atravez de UART a otro PIC  
     * 
     * 
     * Asignacion de Pins:
     * Puerto B = Pines de Control para la banda, RB7,RB6,RB5
     *          
     * Puerto A = Salida de Pines D0-D7 de la LCD
     * 
     * Puerto C = Salida de Clock y entrada de datos de los Slaves, Salida de UART
     *RC7,RC6,RC4,RC3  
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

//*****************************************************************************
// Declaracion de Librerias
//*****************************************************************************
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
  TRISA = 0;//PORTA as output
  TRISD = 0;//PORTD as output
  PORTA = 0;//Inicializar Puerto A 
  PORTD = 0;      
  TRISB =  0;
  PORTB = 0;
  I2C_Master_Init(100000);      //Initialize I2C Master with 100KHz clock
  UARTInit(9600,1); //Iniciar UART en baudrate de 9600, en HS mode
  lcd_init();
  lcd_clear();
  
  __delay_ms(250);
}




void main(){
    Start();
    lcd_clear();
    while(1){
        
//*****************************************************************************
// 1-Declaramos Las ubicaciones para todas Las variables y nombres de los Sensores 
//*****************************************************************************
      lcd_cursor(1,1);// Poner texto de cursor en posicion 1
      lcd_palabra("Agua");
      __delay_ms(10);
      lcd_cursor(1,8);// Poner texto de cursor en posicion 2
      lcd_palabra("Temp");
      __delay_ms(10);
      lcd_cursor(1,13);// Poner texto de cursor en posicion 3
      lcd_palabra("Fire");
      __delay_ms(10);
      lcd_cursor(1,19);// Poner texto de cursor en posicion 3
      lcd_palabra("Cera");
      __delay_ms(10);
      lcd_cursor(1,25);// Poner texto de cursor en posicion 3
      lcd_palabra("Banda");
      __delay_ms(10);
   
    
//*****************************************************************************
// 2-Sensor de Profundidad-Mandamos a Llamar por I2C al sensor de profundidad de
// Agua y recibimos el valor que manda el sensor, si el valor esta por debajo de
//un limite, le  pedimos al usuario que rellene la cubeta con agua. Desplegamos
//El estado del agua de la cubeta en la LCD. Por ultimo mandamos el dato por UART
//*****************************************************************************  
    I2C_Master_Start();         //Condicion de inicio
    I2C_Master_Write(0x11);     //Address
    Profundidad = I2C_Master_Read(0); //Mandar valor leido a variable
    I2C_Master_Stop();          //Condicion de Fin
    itoa(buffer,Profundidad,10);     //Convertir variable en String
    if(Profundidad < 4){
    lcd_cursor(2,1);            //Desplegar en LCD
    lcd_palabra("Refill"); 
    }
    if(Profundidad >= 4){
    lcd_cursor(2,1);            //Desplegar en LCD
    lcd_palabra("OK"); 
    }
    UART_Write_Text(buffer);
//*****************************************************************************
// 3-Sensor de Temperatura-Mandamos a llamar por I2C al sensor de Temperatura y 
//Recibimos el Valor analogico de Temperatura. Usando un factor de conversion,
//convertimos el valor a la temperatura actual. Desplegamos la Temperatura en Pantalla
//y luego revisamos una condicion de temperatura que controla el relay que enciende y apaga
//la resistencia termica.Luego Mandamos el Valor por UART de temperatura
//*****************************************************************************
    I2C_Master_Start();         //Condicion de inicio
    I2C_Master_Write(0x21);     //Address
    Temperatura = I2C_Master_Read(0); //Mandar valor leido a variable
    I2C_Master_Stop();          //Condicion de Fin
    Rtemp =Temperatura*150/255;
    itoa(buffer,Rtemp,10);      //Convertir en String
    lcd_cursor(2,9);            //Desplegar en LCD
    lcd_palabra(buffer); 
    lcd_cursor(2,11);
    lcd_palabra("C");
    if(Rtemp< 45){
        PORTBbits.RB7= 1;
    }
    if(Rtemp>= 45){
        PORTBbits.RB7= 0;
        //__delay_ms(10000);//delay de 10 segundos 
    }
    UART_Write_Text(buffer);
    
//*****************************************************************************
// 4-Sensor de Incendios- Mandamos a llamar por I2C al sensor de Incendios y 
//Recibimos el Valor Digital. Si el sensor detecta un incendio, desplegarlo en la
//Pantalla y encender una alarma. Luego mandar el estado del sistema por UART
//***************************************************************************** 
    I2C_Master_Start();         //Condicion de inicio
    I2C_Master_Write(0x31);     //Address
    Fuego = I2C_Master_Read(0); //Mandar valor leido a variable
    I2C_Master_Stop();          //Condicion de Fin
    if(Fuego == 100){
        PORTBbits.RB6 = 0;      //Apagar la Alarma
        lcd_cursor(2,13);            //Desplegar en LCD
        lcd_palabra("Safe");
        UART_Write_Text("Safe");
    }
    else if(Fuego == 0){
        PORTBbits.RB6 = 1;      //Prender la Alarma
        lcd_cursor(2,13);            //Desplegar en LCD
        lcd_palabra("Fuego");
        UART_Write_Text("Fuego");
    }
    
    
//*****************************************************************************
// 5-Sensor Ultrasonico- Mandamos a llamar el valor de distancia por I2C. Recibimos
//el valor de 3 digitos y lo separamos en unidades y deceneas. Lo desplegamos en la
//LCD. Luego mandamos el valor de distancia por UART
//*****************************************************************************  
    I2C_Master_Start();         //Condicion de inicio
    I2C_Master_Write(0x41);     //Address
    Distancia = I2C_Master_Read(0); //Mandar valor leido a variable
    I2C_Master_Stop();          //Condicion de Fin
     d1 = (Distancia/100)%10;
     d2 = (Distancia/10)%10;
     d3 = (Distancia/1)%10;
    itoa(buffer,Distancia,10);     //Convertir variable en String
    lcd_cursor(2,19);            //Desplegar en LCD
    lcd_char(d1+'0');
    lcd_char(d2+'0');
    lcd_char(d3+'0');   
    lcd_cursor(2,22);            //Desplegar en LCD
    lcd_palabra("Cm");
    UART_Write_Text(buffer);
    
//*****************************************************************************
// 6-Sensor Infrarojo-Mandamos a llamar el estado del sensor por I2C y desplegamos
//En la LCD si la banda se esta moviendo o si esta detenida, dependiendo de el estado 
//del sensor. Luego mandamos el estado de la banda por UART
//*****************************************************************************
    I2C_Master_Start();         //Condicion de inicio
    I2C_Master_Write(0x51);     //Address
    Luz = I2C_Master_Read(0); //Mandar valor leido a variable
    I2C_Master_Stop();          //Condicion de Fin
    if(Luz == 1){
    lcd_cursor(2,25);            //Desplegar en LCD
    lcd_palabra("Detener");
    UART_Write_Text("Detener");   
    }
    if(Luz == 0){
    lcd_cursor(2,25);            //Desplegar en LCD
    lcd_palabra("Mover");
    UART_Write_Text("Mover");
        
    }
    
    
//*****************************************************************************
// Movemos la pantalla a la izquierda en cada ciclo
//*****************************************************************************
    LCD_SCREEN_SHIFT(1);//mover a la izquierda
    __delay_ms(20);
    }
    }
  
