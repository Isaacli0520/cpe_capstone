# include "FSM.h"

extern int RotaryEncoderStateCount;
extern int LED_pwm_level;
extern int LED_step;

void InitializeFSM(FSMType *FSM)
{
    FSM->CurrentState = ResetState;
}

FSMState NextStateFunction(FSMType *FSM)
{
    FSMState NextState;

    switch (FSM->CurrentState) {
    case ResetState:
        if(FSM->SwitchA == Active && FSM->SwitchB == Inactive){
            NextState = AActiveBInactive;
        }
        else if(FSM->SwitchA == Active && FSM->SwitchB == Active){
            NextState = AActiveBActive;
        }
        else if(FSM->SwitchA == Inactive && FSM->SwitchB == Active){
            NextState = AInactiveBActive;
        }
        else if(FSM->SwitchA == Inactive && FSM->SwitchB == Inactive){
            NextState = AInactiveBInactive;
        }
        else{
            NextState = ResetState;
        }
        break;

    case AInactiveBInactive:
        if(FSM->SwitchA == Active && FSM->SwitchB == Inactive){
            NextState = AActiveBInactive;
        }
        else if(FSM->SwitchA == Inactive && FSM->SwitchB == Active){
            NextState = AInactiveBActive;
        }
        else{
            NextState = AInactiveBInactive;
        }
        break;

    case AInactiveBActive:
        if(FSM->SwitchA == Inactive && FSM->SwitchB == Inactive){
            NextState = AInactiveBInactive;
        }
        else if(FSM->SwitchA == Active && FSM->SwitchB == Active){
            NextState = AActiveBActive;
        }
        else{
            NextState = AInactiveBActive;
        }
        break;

    case AActiveBInactive:
        if(FSM->SwitchA == Active && FSM->SwitchB == Active){
            NextState = AActiveBActive;
        }
        else if(FSM->SwitchA == Inactive && FSM->SwitchB == Inactive){
            NextState = AInactiveBInactive;
        }
        else{
            NextState = AActiveBInactive;
        }
        break;

    case AActiveBActive:
        if(FSM->SwitchA == Active && FSM->SwitchB == Inactive){
            NextState = AActiveBInactive;
        }
        else if(FSM->SwitchA == Inactive && FSM->SwitchB == Active){
            NextState = AInactiveBActive;
        }
        else{
            NextState = AActiveBActive;
        }
        break;

    default:
        NextState = ResetState;
    }

    return NextState;
}

void OutputFunction(FSMType *FSM)
{
    // Update RotaryEncoderStateCount
    switch (FSM->CurrentState) {
    case ResetState:
        RotaryEncoderStateCount = 0;
        break;

    case AInactiveBInactive:
        if(FSM->SwitchA == Active && FSM->SwitchB == Inactive){
            RotaryEncoderStateCount += 1;
        }
        else if(FSM->SwitchA == Inactive && FSM->SwitchB == Active){
            RotaryEncoderStateCount -= 1;
        }
        break;

    case AInactiveBActive:
        if(FSM->SwitchA == Inactive && FSM->SwitchB == Inactive){
            RotaryEncoderStateCount += 1;
        }
        else if(FSM->SwitchA == Active && FSM->SwitchB == Active){
            RotaryEncoderStateCount -= 1;
        }
        break;

    case AActiveBInactive:
        if(FSM->SwitchA == Active && FSM->SwitchB == Active){
            RotaryEncoderStateCount += 1;
        }
        else if(FSM->SwitchA == Inactive && FSM->SwitchB == Inactive){
            RotaryEncoderStateCount -= 1;
        }
        break;

    case AActiveBActive:
        if(FSM->SwitchA == Active && FSM->SwitchB == Inactive){
            RotaryEncoderStateCount -= 1;
        }
        else if(FSM->SwitchA == Inactive && FSM->SwitchB == Active){
            RotaryEncoderStateCount += 1;
        }
        break;

    default:
        RotaryEncoderStateCount = 0;
    }
    if (FSM->CurrentState == ResetState) {
        // Insert code to initialize TA0CCR1 and the LED display value.
        RotaryEncoderStateCount = 0;
        LED_pwm_level = MAX_LED_VALUE / 2;
        TACCR1 = TACCR0 * (1.0*LED_pwm_level / MAX_LED_VALUE);
    }
    else{
        // Now, use RotaryEncoderStateCount to update the value of TA0CCR1 to modify the PWM duty cycle
        // and update the corresponding value displayed in the two 7-segment displays.
        if (RotaryEncoderStateCount == 4) { // clockwise
            // Insert code action(s) when rotary encoder has been rotated clockwise.

            RotaryEncoderStateCount = 0;
            if(LED_pwm_level < MAX_LED_VALUE){
                LED_pwm_level += LED_step;
                TACCR1 = TACCR0 * (1.0*LED_pwm_level / MAX_LED_VALUE);
            }
        }

        if (RotaryEncoderStateCount == -4) { // counter-clockwise
            // Insert code for action(s) when rotary encoder has been rotated counter-clockwise.

            RotaryEncoderStateCount = 0;
            if(LED_pwm_level > MIN_LED_VALUE){
                LED_pwm_level -= LED_step;
                TACCR1 = TACCR0 * (1.0*LED_pwm_level / MAX_LED_VALUE);
            }
        }
    }
}
