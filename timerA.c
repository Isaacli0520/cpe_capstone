#include "timerA.h"

extern unsigned int g1msTimer;

void ConfigureTimerA(void)
{
    // Stop and clear the timer.
    TA0CTL = (MC_0 | TACLR);

    // Assign a value to TA0CCR0 and TA0CCR1.
    TA0CCR0 = 16000;
    TA0CCR1 = TA0CCR0 / 2;

    // Choose Reset/Set mode so that increase in intensity corresponds to
    // clockwise rotation of rotary encoder, and decrease corresponds to
    // counter-clockwise rotation.
    TA0CCTL1 |= OUTMOD_7;

    // Also, configure GPIO P1.6 so that the timer output is connected to the port pin.
    P1DIR |= BIT2;
    P1SEL |= BIT2;
    P1SEL2 &= ~BIT2;

    // Enable TACCR0 Compare/Capture Interrupt Flag (CCIFG0)
    TA0CCTL0 |= CCIE;

    // Finally, start the timer in up mode using the SMCLK and an appropriate
    // input divide for the Timer A clock.
    TA0CTL |= (TASSEL_2 | MC_1 | ID_0);

}
