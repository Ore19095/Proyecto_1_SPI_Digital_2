/*
 * File:   main.c
 * Author: AngelOrellana
 *
 * Created on 15 de febrero de 2021, 05:08 PM
 */


// CONFIG1
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

#include <xc.h>
#include "ADC.h"
#include "spi.h"

int adcValue;
uint8_t temperature;
uint8_t valor;
void main(void) {
    ANSEL = 1; //ANS0 como entrada analogica
    TRISD = 248;// pines de salida para los leds
    TRISB = 0;
    
    INTCONbits.GIE = 1; 
    INTCONbits.PEIE = 1;
    PIE1bits.SSPIE = 1;// interrupcion mssp activada
    
     spiInit(SPI_SLAVE_SS_EN,SPI_DATA_SAMPLE_MIDDLE,
            SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    initADC();
    
    while(1){
        /* se lee el valor del adc y conociendo que por 10mV se tiene 1°C y que 
         cada valor en el ADC equivale a 4.88mV se hace la conversion,
         unicamente se usara el valor entero*/
        adcValue = readADC(0);
        /* temperatura = adc * ADCstep * 100 °C/V*/
        temperature = adcValue*0.4888; // se truncan los decimales
        PORTB = temperature;
        if(temperature < 25) PORTD = 1;
        else if(temperature < 36 ) PORTD = 2;
        else PORTD = 4;
    }
    return;
}

void __interrupt() isr(){
       if(PIR1bits.SSPIF){
        PIR1bits.SSPIF = 0;
        valor = spiRead();
        if(valor == 'T') spiWrite(temperature);
    }
       return;
}
