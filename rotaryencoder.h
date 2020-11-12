#ifndef ROTARYENCODER_H_
#define ROTARYENCODER_H_

#include <msp430.h>

#define ROTARY_ENCODER_BOUNCE_TIME_MAX 5


#define ROTARY_SWITCH_A_BIT                         BIT4    // Port pin bit location for Switch A
#define ROTARY_SWITCH_A_REN                         P2REN   // Register to enable resistors for Switch A
#define ROTARY_SWITCH_A_PORT_IN                     P2IN    // Register to read port pin input
#define ROTARY_SWITCH_A_PORT                        P2OUT   // Register to select pull-up/pull-down
#define ROTARY_SWITCH_A_DDR                         P2DIR   // Data Direction Register (DDR) for Switch A
#define SET_ROTARY_SWITCH_A_TO_AN_INPUT             ROTARY_SWITCH_A_DDR &= ~ROTARY_SWITCH_A_BIT
#define ENABLE_ROTARY_SWITCH_A_PULL_UP_RESISTORS    ROTARY_SWITCH_A_REN |= ROTARY_SWITCH_A_BIT
#define SELECT_ROTARY_SWITCH_A_PULL_UP_RESISTORS    ROTARY_SWITCH_A_PORT |= ROTARY_SWITCH_A_BIT

#define ROTARY_SWITCH_B_BIT                         BIT3
#define ROTARY_SWITCH_B_REN                         P2REN   // Register to enable resistors for Switch B
#define ROTARY_SWITCH_B_PORT_IN                     P2IN    // Register to read port pin input
#define ROTARY_SWITCH_B_PORT                        P2OUT   // Register to select pull-up/pull-down
#define ROTARY_SWITCH_B_DDR                         P2DIR   // Data Direction Register (DDR) for Switch B
#define SET_ROTARY_SWITCH_B_TO_AN_INPUT             ROTARY_SWITCH_B_DDR &= ~ROTARY_SWITCH_B_BIT
#define ENABLE_ROTARY_SWITCH_B_PULL_UP_RESISTORS    ROTARY_SWITCH_B_REN |= ROTARY_SWITCH_B_BIT
#define SELECT_ROTARY_SWITCH_B_PULL_UP_RESISTORS    ROTARY_SWITCH_B_PORT |= ROTARY_SWITCH_B_BIT

// Function Prototypes
void InitializeRotaryEncoderPortPins(void);
/*
 *  * This function configures and initializes all of the pins associated with the rotary encoder.
 */

#endif /* ROTARYENCODER_H_ */
