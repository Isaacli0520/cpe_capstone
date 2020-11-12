#include <msp430.h>
#include "LED.h"
#include "timerA.h"
#include "rotaryencoder.h"
#include "debounce.h"
#include "FSM.h"
#include "InterruptServiceRoutines.h"

// Global Variables
unsigned int g1msTimer; // Global 1 ms timer used for pushbutton debounce.
int RotaryEncoderStateCount; // Indicates valid FSM sequences.
int new_pwm_level = MIN_LED_VALUE;
int LED_pwm_level = MIN_LED_VALUE;
int LED_step = 2;
int adc_div = 5;
int auto_mode = 0, night_mode = 0;
unsigned int adc;

#define FALSE 0
#define TRUE 1

// Function Prototypes
void ConfigureClockModule();
void InitializeADC();
void InitializeSwitches();
void TimeDelay(unsigned int delay, unsigned int resolution);
SwitchStatus DebounceSwitch(SwitchDefine *Switch);

int main(void)
{
	FSMType RotaryEncoder_FSM;
	SwitchDefine SwitchAutoManual;
	SwitchDefine SwitchAutoNight;
    SwitchDefine RotaryEncoderSwitchA;
    SwitchDefine RotaryEncoderSwitchB;

	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
    ConfigureClockModule();
    InitializeSwitches();
    InitializeADC();
    // Initialize hardware.
    ConfigureTimerA();
	InitializeLEDPortPins();
//	InitializePushButtonPortPin();
	InitializeRotaryEncoderPortPins();
//	TURN_ON_LED2;
    // Initialize Finite State Machine (FSM) state variables.
	InitializeFSM(&RotaryEncoder_FSM);

    // Initialize active-low switch for pushbutton.
    InitializeSwitch(&SwitchAutoManual,(char *) &P1IN,(unsigned char) BIT4,
            Active, Inactive, ACTIVE_THRESHOLD,INACTIVE_THRESHOLD);
    InitializeSwitch(&SwitchAutoNight,(char *) &P1IN,(unsigned char) BIT5,
                Active, Inactive, ACTIVE_THRESHOLD,INACTIVE_THRESHOLD);

    // Initialize active-low switches for rotary encoder.
    InitializeSwitch(&RotaryEncoderSwitchA,(char *) &P2IN,(unsigned char) BIT3,
                Active, Inactive, ACTIVE_THRESHOLD,INACTIVE_THRESHOLD);
    InitializeSwitch(&RotaryEncoderSwitchB,(char *) &P1IN,(unsigned char) BIT1,
                Active, Inactive, ACTIVE_THRESHOLD,INACTIVE_THRESHOLD);

    _enable_interrupts();

	// Loop forever
	while(TRUE) {
	    if(ReadSwitchStatus(&SwitchAutoManual) == Active)
            auto_mode = 1;
	    else // if (ReadSwitchStatus(&SwitchAutoManual) == Inactive)
            auto_mode = 0;

	    if(ReadSwitchStatus(&SwitchAutoNight) == Active){
	        if(night_mode == 0){
	            LED_pwm_level = MIN_LED_VALUE;
	        }
	        LED_step = 1;
            night_mode = 1;
	    }
        else{ // if (ReadSwitchStatus(&SwitchAutoNight) == Inactive)
            night_mode = 0;
            LED_step = 2;
        }

	    if(auto_mode && !night_mode){
            ADC10CTL0 |= ENC + ADC10SC; // Enable Conversion + Start Conversion
            adc = ADC10MEM;            // Copies data in ADC10MEM to unsigned int adc
            new_pwm_level = adc / adc_div;
            if((new_pwm_level & 1) != 0)
                new_pwm_level += 1;
            if(new_pwm_level > MAX_LED_VALUE)
                new_pwm_level = MAX_LED_VALUE;
            else
                new_pwm_level = MIN_LED_VALUE;
            if(new_pwm_level > LED_pwm_level)
                LED_pwm_level += LED_step;
            else
                LED_pwm_level -= LED_step;
            TACCR1 = TACCR0 * (1.0*LED_pwm_level / MAX_LED_VALUE);
            TimeDelay(250, 1000);
	    }
	    else{
           RotaryEncoder_FSM.SwitchA = ReadSwitchStatus(&RotaryEncoderSwitchA);
           RotaryEncoder_FSM.SwitchB = ReadSwitchStatus(&RotaryEncoderSwitchB);

           OutputFunction(&RotaryEncoder_FSM);

           RotaryEncoder_FSM.CurrentState = NextStateFunction(&RotaryEncoder_FSM);
	    }
	}

	return 0;
}

void TimeDelay(unsigned int delay, unsigned int resolution)
{
    unsigned int i;

    // Use Timer A1 Control Register

    // Stop the timer and clear register
    TA1CTL = (MC_0 | TACLR);

    // Update the value of Compare-Capture Register 0.
    TA1CCR0 = resolution;

    // Restart the timer in up mode using the SMCLK with no input clock division.
    TA1CTL |= (TASSEL_2 | MC_1);

    for (i = 0; i < delay; i++) {

        // Wait for the timer to overflow
        while( !(TA1CTL & (TAIFG)) ){}

        // Reset the TAIFG.
        TA1CTL &= ~TAIFG;
    }

    // Stop the timer and clear register
    TA1CTL = (MC_0 | TACLR);
}


SwitchStatus DebounceSwitch(SwitchDefine *Switch)
{
    unsigned char X1;
    unsigned char X0;
    SwitchStatus CurrentSwitchReading;
    unsigned int ElapsedTime = 0;

    // First, determine the current inputs, X1 and X0.
    ElapsedTime = (unsigned int) (g1msTimer-Switch->EventTime);
    X1 = FALSE;
    if (Switch->CurrentState == ValidateActive) {
        if (ElapsedTime > Switch->ActiveThreshold) {X1 = TRUE;}
    }
    else if (Switch->CurrentState == ValidateInactive){
        if (ElapsedTime > Switch->InactiveThreshold) {X1 = TRUE;}
    }

    CurrentSwitchReading = ReadSwitchStatus(Switch);
    X0 = FALSE;
    if (CurrentSwitchReading == Active) {X0 = TRUE;}

    // Perform the output function based on the inputs and current state.
    OutputFunctionDebounce(Switch,X0,X1);

    // Next, based on the input values and the current state, determine the next state.
    Switch->CurrentState = NextStateFunctionDebounce(Switch,X0,X1);

    return Switch->DebouncedSwitchStatus;
}

void InitializeADC(){
    ADC10CTL0 = ADC10SHT_0 + ADC10ON;
    ADC10CTL1 = INCH_3;
    ADC10AE0 |= BIT3;
}

void InitializeSwitches(){
    P1DIR &= ~BIT4;
    P1OUT |= BIT4;
    P1REN |= BIT4;

    P1DIR &= ~BIT5;
    P1OUT |= BIT5;
    P1REN |= BIT5;
}

void ConfigureClockModule()
{
    // Configure Digitally Controlled Oscillator (DCO) using factory calibrations
    DCOCTL  = CALDCO_16MHZ;
    BCSCTL1 = CALBC1_16MHZ;
}
