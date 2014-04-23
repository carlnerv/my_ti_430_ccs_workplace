/*
 * my430lib.h
 *
 *  Created on: 2014-4-4
 *      Author: CarlNERV
 */

#ifndef MY430LIB_H_
#define MY430LIB_H_

#ifndef DELAYS
#define DELAYS

#define CPU_F ((double)7995392)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

#endif /* DEALYS */

#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long

#endif /* MY430LIB_H_ */
