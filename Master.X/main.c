/*
 * File:   main.c
 * Author: Angel Orellana
 *
 * Created on 14 de febrero de 2021, 09:40 AM
 */

#// CONFIG1
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

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include<stdint.h>
#include "spi.h"
#include "LCD8bits.h"
#include "UART.h"
#define _XTAL_FREQ 4000000


// valor de los "comandos que se enviaran a cada pic slave

#define GIVE_ADC    'A'
#define GIVE_COUNTER 'C'
#define GIVE_TEMP 'T'


// lectura del adc
uint8_t valueADC;
char* cadenaADC;
// lectura del contador
uint8_t valueCounter;
char* cadenaCounter;
// lectura de temperatura
char* cadenaTemp;
uint8_t valorTemp;
//-----------prototipos de funciones--------------------------- 
char* adcToString(uint16_t);
char* int2String(uint8_t);


int main(){
  //como no se utilizara el ADC en el master se desactivan
  ANSEL = 0;
  ANSELH = 0;
  // PUERTOS PARA LA LCD
  TRISD = 0;
  TRISE = 0;
  //PUERTOS PARA SPI
  TRISA = 248; // PARA LA ACTIVACION DE nSS DE CADA PIC 
  // INICIALIZACION DE LOS MODULOS A USAR
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE,
                SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
  LcdInit();
  UARTInit(9600,1);
  
  LcdSetCursor(1,1);
  LcdWriteString("ADC: CONT: TEMP:");
  while(1){
      LcdSetCursor(2,1); //inicio de segunda linea
      //--------- comunicacion con el slave que tiene el adc
      PORTA = 6; //RA0 en 1 
      //se recibe el valor del ADC y se convierte a voltaje;
      spiWrite(GIVE_ADC);
      valueADC = spiRead();
      
      cadenaADC = adcToString(valueADC);
      LcdWriteString(cadenaADC);
      LcdWriteString("V ");
      //-----comunicacion con el slave de contador
      PORTA = 5;
      spiWrite(GIVE_COUNTER);
      valueCounter = spiRead();
      cadenaCounter = int2String(valueCounter);
      LcdWriteString(cadenaCounter) ;
      LcdWriteChar(' ');
      // ------ comunicacion con el slave de temperatura
      PORTA = 3;
      spiWrite(GIVE_TEMP);
      valorTemp = spiRead();
      cadenaTemp = int2String(valorTemp);
      LcdWriteString(cadenaTemp);
      LcdWriteChar(0xDF); // simbolo de grado
      LcdWriteChar('C');
  }

  
  return 0;
}

char* adcToString(uint16_t value){
    char salida[5]; //cadena que sera retornada
    uint16_t digito;
    
    value = value*1.96; // se cambia de 0-255  a 0-500
    digito = value/100 ; // unidad de voltaje
    
    salida[0] = digito + 48;
    salida[1] = '.';
    value -= digito*100;
    
    digito = value/10; // decimas
    salida[2] = digito +48;
    digito = value % 10;
    salida[3] = digito +48;
    salida[4] = '\0';
    return salida;
}

char* int2String(uint8_t value){
    char cadena[4]; //cadena en donde se almacenan los ASCCI de los digitos
    uint8_t digito;
    // se obtiene el valor de las decenas y s e convierte a ASCII
    digito = value/100;
    cadena[0] = digito +48;
    value-= digito*100;
    // se hace lo mismo con las decenas y centenas
    cadena[1] = value/10 + 48;
    cadena[2] = value%10 + 48;
    cadena[3] = '\0';
    return cadena;
}