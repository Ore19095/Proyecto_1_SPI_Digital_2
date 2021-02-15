/*
 * File:   MAIN.c
 * Author: AngelOrellana
 *
 * Created on 14 de febrero de 2021, 04:11 PM
 */
// CONFIG1
#pragma config FOSC = EXTRC_CLKOUT// Oscillator Selection bits (RC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
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
#include <stdint.h>
#include "spi.h"
#include "ADC.h"


uint8_t adcValue;

void main(void) {
    ANSEL = 1; //ANS0 como entrada analogica
    ANSELH= 0;
    INTCONbits.GIE =1;
    INTCONbits.PEIE =1; //perifericos activados
    PIE1bits.SSPIE = 1; //interrupcion en MSSP activada
    TRISB = 0;
    
    spiInit(SPI_SLAVE_SS_EN,SPI_DATA_SAMPLE_MIDDLE,
            SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    initADC();
    
    while(1){
        adcValue = readADC(0);
        PORTB = adcValue;
    }
    
    return;
}

void __interrupt() isr(){
    if(PIR1bits.SSPIF ==1){ // si se recibio un dato
        int valor = spiRead();
        if(valor == 1){
            spiWrite(adcValue);
        }
        PIR1bits.SSPIF = 0;
    }
}


