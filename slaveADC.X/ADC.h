/* 
 * File:   
 * Author: Angel Orellana 
 * Comments:
 * Revision history: primera version para pic 16F887
 
 */

#ifndef ADC_H 
#define	ADC_H 

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000 //valor por defecto
#endif

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
void initADC(void);
/**
 * es necesario configurar previamente el pin que se va a utilizar como entrada
 * analogica.
 * @param pin pin adc que se desea leer, 
 * @return valor leido 8bits
 */
uint8_t readADC(uint8_t);


#endif	/* XC_HEADER_TEMPLATE_H */

