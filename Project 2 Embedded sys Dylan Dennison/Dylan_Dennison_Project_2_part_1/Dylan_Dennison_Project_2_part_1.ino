//Dylan Dennison
//Project 2 part 1
//10/10/21

#define BIT5_MASK 0x20; // b 0010 0000
#define BIT0_MASK 0x01; // b 0000 0001
#define BIT3_MASK 0x08; // b 0000 1000
#define BIT2_MASK 0x04; // b 0000 0100
#define BIT7_MASK 0x80; // b 1000 0000

/////!!! I used both the internal LED and the external LED. I did this because the last project he wanted both. I hope this is not a problem
/////!!! I just wanted to show that it is super simple to do both at the same time.

int main(void)
{
 unsigned char *portD; // for input
 portD = (unsigned char *) 0x2B;
 // Enable the pull-up resistor on PD2 using the Port D
 // Data Register (PORTD)
 *portD |= BIT2_MASK; // b 0000 0100
 
 // Configure external interrupt 0 to generate an interrupt request on any
 // logical change using External Interrupt Control Register A (EICRA)
 unsigned char *AT328_EICRA;
 AT328_EICRA = (unsigned char *) 0x69;
 *AT328_EICRA |= BIT0_MASK; // b 0000 0001 --> b xxxx xx11 --> Any change for INT 0
 
 // Enable external interrupt 0 using the External Interrupt Mask Register
 // (EIMSK)
 unsigned char *AT328_EIMSK;
 AT328_EIMSK = (unsigned char *) 0x3D;
 *AT328_EIMSK |= BIT0_MASK; // b 0000 0001 --> b xxxx xxx1 --> INT0 enable

 // Configure PB5 as an output using the Port B Data Direction Register
 // (DDRB) // for internal light. 
 unsigned char *portDDRB;
 portDDRB = (unsigned char *) 0x24;
 *portDDRB |= BIT5_MASK; // PORTB bit 5 as output (bit --> 1) --> b 0010 0000 --> b xx1x xxxx

//Configure PC3 as an output as well using the Port C Data Direction Register
// for external lED
unsigned char *portDDRC;
portDDRC = (unsigned char *) 0x27; // for DDRC
*portDDRC |= BIT3_MASK; //PORTC bit 3 as output (bit -->1) --> b 0000 1000 --> xxxx 1xxx
 
 // Enable interrupts using SREG
 unsigned char *ptrSREG;
 ptrSREG = (unsigned char *) 0x5F;
 *ptrSREG |= BIT7_MASK; // b 1000 0000 == 0x80 --> b 1xxx xxxx

 // Loop forever
 while (1)
 {
 // Nothing to do here
 // All work is done in the ISR
 }
}

//interupt function
ISR(INT0_vect)
{
 unsigned char *portPinD; // for input pin
 portPinD = (unsigned char *) 0x29; //assigning pin value
 
 unsigned char *portB; // for internal LED
 portB = (unsigned char *) 0x25; // assigning internal LED value

 unsigned char *portC; // for external LED
 portC = (unsigned char *)0x28;


 if (*portPinD & 0x04){ // I had to use portPinD becauaw just using the interrupt seemed to give me mulitple problems. 
  if ((*portB & 0x20) == 0x20){ // if LEDS are on 
  // PD2 is high, so button is released
  // Set PB5 low using the Port B Data Register (PORTB)
  *portB &= ~0x20; // turn off internal 
  *portC &= ~0x08; // turns off external 
  MyDelay(1000); //delay for 1 second
  }
  else{ // if LEDS are off 
  // PD2 is low, so button is pressed
  // Set PB5 high using the Port B Data Register (PORTB)
  *portB |= 0x20; // turns on internal 
  *portC |= 0x08; // turns on external 
  MyDelay(1000);  // delay for 1 second 
  }
 }


}


// MyDelay function given by instructor 
void MyDelay(unsigned long mSec) // takes an extended size varible for number storage 
{
  volatile unsigned long i; //volatile unsigned long makes sure the state change is immediately visible in the loop function 
  unsigned long endT = 1000 * mSec; // * the input varible by 1000

  for (i = 0; i < endT; i++); // the loop running is what actually creates a delay 
}
