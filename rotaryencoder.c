#include "rotaryencoder.h"

void InitializeRotaryEncoderPortPins(void)
{
	P2DIR &= ~BIT3;
	P1DIR &= ~BIT1;

	P2REN |= BIT3;
	P1REN |= BIT1;

	P2OUT |= BIT3;
	P1OUT |= BIT1;
}
