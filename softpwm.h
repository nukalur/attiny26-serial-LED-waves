// Pin mappings
#define CHMAX       3    // maximum number of PWM channels
#define PWMDEFAULT  0x08  // default PWM value at start up for all channels

#define R0_CLEAR (pinlevelB &= ~(1 << PB4)) // map R0 to PB4
#define G0_CLEAR (pinlevelB &= ~(1 << PB5)) // map G0 to PB5
#define B0_CLEAR (pinlevelB &= ~(1 << PB6)) // map B0 to PB6

//#define R1_CLEAR (pinlevelA &= ~(1 << PA3)) // map R1 to PA3
//#define G1_CLEAR (pinlevelA &= ~(1 << PA4)) // map G1 to PA4
//#define B1_CLEAR (pinlevelA &= ~(1 << PA5)) // map B1 to PA5

// ROOM FOR A THIRD OR 4TH ON REST OF PORT A AND SOME OF PORT B ********

// Set bits corresponding to pin usage above
#define PORTB_MASK  (1 << PB4)|(1 << PB5)|(1 << PB6)

// prototypes
void PWMInit(void);
void setPWM(unsigned char,unsigned char);

