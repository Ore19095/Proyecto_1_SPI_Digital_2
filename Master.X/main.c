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

#define GIVE_ADC 1
#define GIVE_COUNTER 2
#define GIVE_TEMP 3
//-----------prototipos de funciones--------------------------- 
char* uint8ToChar(uint8_t);


int main(){
  //como no se utilizara el ADC en el master se desactivan
  ANSEL = 0;
  ANSELH = 0;
  // PUERTOS PARA LA LCD
  TRISD = 0;
  TRISE = 0;
  //PUERTOS PARA SPI
  TRISA = 0; // PARA LA ACTIVACION DE nSS DE CADA PIC 
  // INICIALIZACION DE LOS MODULOS A USAR
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE,
                SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
  LcdInit();
  UARTInit(9600,1);
  
  // varibles para el adc 
  uint8_t adc;
  uint8_t counter;
  uint8_t temp;
  
  char* adcChar;
  LcdSetCursor(1,1);
  LcdWriteString("ADC: CONT: TEMP:");
  while(1){
      LcdSetCursor(2,1); //inicio de segunda linea
      PORTA = ~ 1; //RA0 en 1 
      
      spiWrite(GIVE_ADC);
      adc = spiRead(); // se lee el valor enviado
      adcChar = uint8ToChar(adc);
      LcdWriteString(adcChar);
  }

  
  return 0;
}

char* uint8ToChar(uint8_t value){
    char salida[4]; //cadena que sera retornada
    uint8_t temp;
    
    temp = value/100;
    
    salida[0] = temp + 48;// se obtiene el valor del 3er digito, y se 
            //pasa a su equivalente en ASCII
    
    value -= temp*100;
    
    salida[1] = value/10 +48 ;
    salida[2] = value % 10 + 48;
    
    salida[3] = '\0';
    
    return salida;
}

