#include <msp430.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my430lib.h"
#include "LCD1602.h"

unsigned long _mseed = 1;
unsigned long _mchaeq = 1;
unsigned long _mlength = 1;

void smseq(unsigned long seed, unsigned long chaeq, unsigned char length)
{
    _mseed = seed;
    _mchaeq = chaeq >> 1;
    _mlength = (1 << length) - 1;       // length = 2, _mlength = 0x03
}

unsigned long msequ(void)
{
    unsigned long temp;
    unsigned char c =0 ;
    temp = _mseed & _mchaeq;

    for (c =0; temp; c++)
    {
        temp &= (temp -1) ; // Çå³ý×îµÍÎ»µÄ1
    }

    c &= 1;

    _mseed = (_mseed << 1) + c;

    return (_mseed & _mlength);
}

void Sys_init(void);
void Port_init(void);
void Timer_init(void);
void USCI_init(void);
void Clock_init(void);

int main(void)
{
	char adcdata[17];
	unsigned long tmseed,tmchaeq;
	unsigned int tmlength;
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	Sys_init();

	LCD_init();                         //液晶参数初始化设置
	LCD_write_str(0, 0, "ADC10 + rand");

	// ADC10CTL0 = SREF_1 + REFON + ADC10ON; // ADC10ON, interrupt enabled
	// ADC10AE0 |= 0x01;                         // P7.4 ADC option select

	// P5DIR |= 0x02;                            // Set P5.1 to output direction

	// ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
	// while(ADC10CTL0 & ADC10IFG == 0);
	// srand(ADC10MEM & 0x0f);

	tmseed = 0x01;
	tmchaeq = 0x8003;
	tmlength = 15;
	smseq(tmseed,tmchaeq,tmlength);

	for (;;)
	{
		__bis_SR_register(LPM0_bits + GIE);        // LPM0, ADC10_ISR will force exit

		// adcdata = (signed char)ADC10MEM;

		sprintf(adcdata, "%6d", msequ());
		// sprintf(adcdata, "%4x", ADC10MEM);

		LCD_write_str(0, 1, adcdata);

	}
}

// ADC10 interrupt service routine
/* #pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
	__bic_SR_register_on_exit(LPM0_bits);        // Clear CPUOFF bit from 0(SR)
} */

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)
{
	// P5OUT ^= 0x02;                            // Toggle P5.1 using exclusive-OR
	CCR0 += 32768;
	// FLAG |= F_TX;							// tx data
	_bic_SR_register_on_exit(LPM0_bits);     // Clear LPM3 bits from 0(SR)
}

// Timer_A3 Interrupt Vector (TAIV) handler
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1(void)
{
	switch( TAIV )
	{
		case  2:     	                      // CCR1
					// CCR1 += 32767;
					// _bic_SR_register_on_exit(LPM0_bits);     // Clear LPM3 bits from 0(SR)
					break;
		case  4: 	CCR2 += 32768;					// led
					break;                          // CCR2
		case 10: 	break;                 			// overflow not used

	}
}

/* #pragma vector=USCIAB0RX_VECTOR
__interrupt void USCIA0RX_ISR (void)
{
	// while(!(IFG2&UCA0TXIFG));
	uart_rx_buf = UCA0RXBUF;                    // TX -> RXed character
	_bic_SR_register_on_exit(LPM0_bits);
} */

void Sys_init(void)
{
	Clock_init();
	Port_init();
	USCI_init();
	Timer_init();
}

void Port_init(void)
{
/*
	P1.0 nRF24L01P_IRQ
	P1.1 & P1.6 to output MCLK & ACLK
*/
	P1DIR = 0xFE;								//  P1.1 ~ P1.7 outputs, P1.0 input
	P1OUT = 0;									// All P1.x reset
	P1SEL = BIT1 + BIT6;						// P1.1 & P1.6 to output MCLK & ACLK
/* 	P1IE |= BIT0;                            	// P1.0 interrupt enabled
	P1IES |= BIT0;                           	// P1.0 Hi/Lo edge
	P1IFG &= ~BIT0;                          	// P1.0 IFG cleared
	 */
/*
	P2 LCD1602 dataport, DIR: output
 */
	P2DIR = 0xFF;								// All P2.x outputs
	P2OUT = 0;									// All P2.x reset

/*
	P3.5,6,7 LCD1602 ctrl port, DIR: output
*/
	P3DIR = 0xFF;								// All P3.x outputs
	P3OUT = 0;									// All P3.x reset

	/* not used */
	P4DIR = 0xFF;								// All P4.x outputs
	P4OUT = 0;									// All P4.x reset

	/* not used */
	P5DIR = 0xFF;								// All P5.x outputs
	P5OUT = 0;									// All P5.x reset

/*
	P6.0 nRF24L01P CE, GPIO
	P6.1 nRF24L01P MISO, USCI_B0
	P6.2 nRF24L01P MOSI, USCI_B0
	P6.3 nRF24L01P CSN, GPIO
	P6.4 nRF24L01P SCK, USCI_B0
	P6.5 GPS Rx, USCI_A0
	P6.6 GPS Tx, USCI_A0
 */
	P6DIR = 0xFF;								// All P6.x outputs
	P6OUT = 0;									// All P6.x reset
	// P6SEL |= BIT1 + BIT2 + BIT4;              // P6.1,2,4 option select(USCI_B0, SPI), DIR don't care
	// P6SEL |= BIT5 + BIT6;						// P6.5,6 option select(USCI_A0, UART), DIR don't care

	/* P7.6 TA2 output */
	P7DIR = 0xFF;								// All P7.x outputs
	P7OUT = 0;									// All P7.x reset
	P7SEL |= 0x40;                            	// P7.6 TA2 option

}

void Timer_init(void)
{
	// CCR0 = 32768-1;                             // PWM Period
	CCTL0 = CCIE;                             // CCR0 interrupt enabled
	CCR0 = 32768;
	// CCTL1 = OUTMOD_7;                         // CCR1 reset/set
	// CCTL1 = CCIE;
	// CCR1 = 32767;                               // CCR1 PWM duty cycle
	CCTL2 = CCIE + OUTMOD_7;                         // CCR2 reset/set
	CCR2 = 4096;                               // CCR2 PWM duty cycle
	TACTL = TASSEL_1 + MC_2 + TAIE;           // ACLK, cont. mode, interrupt
}

void USCI_init(void)
{
	UCA0CTL1 |= UCSSEL_2;                     // CLK = SMCLK
	UCA0BR0 = 0x03;                           // 7995392/9600 = 832.8533
	UCA0BR1 = 0x00;                           //
	UCA0MCTL = 0x06;                          // Modulation
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
	IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt

	UCB0CTL0 |= UCMST + UCSYNC + UCCKPH + UCMSB;    //3-pin, 8-bit SPI master
	UCB0CTL1 |= UCSSEL_2;                     // SMCLK
	UCB0BR0 = 0x01;                           // /1
	UCB0BR1 = 0;                              //
	// UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
}

void Clock_init(void)
{
	// SCFI0 |= FN_2;                            // x2 DCO, 4MHz nominal DCO
	// SCFQCTL = 30;                             // (74+1) x 32768 = 2.45Mhz

	// FLL_CTL0 |= DCOPLUS;

	SCFI0 |= FN_4;                            // x2 DCO freq, 8MHz nominal DCO
	SCFQCTL = 121;                            // (121+1) x 32768 x 2 = 7.99 Mhz(7995392)
	FLL_CTL0 |= DCOPLUS;           				// DCO+ set so freq= xtal x D x N+1
}
