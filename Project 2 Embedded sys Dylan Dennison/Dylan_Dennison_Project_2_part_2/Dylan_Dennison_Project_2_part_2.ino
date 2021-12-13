//Dylan Dennison
//Project 2 part 1
//10/10/21

#define BIT0_MASK 0x01; // b 0000 0001
#define BIT1_MASK 0x02; // b 0000 0010
#define BIT2_MASK 0x04; // b 0000 0100
#define BIT3_MASK 0x08; // b 0000 1000
#define BIT5_MASK 0x20; // b 0010 0000
#define BIT7_MASK 0x80; // b 1000 0000

 long timer = 500000; //timer for button delay
 
// Configure external interrupt 0 to generate an interrupt request
 unsigned char *AT328_EICRA; // we make this global for we can change the interrupt status each time a button is pressed 
 
 bool interrupt = false;// for use case switch
 bool internal = true; // for alternating between lights
 bool external = false;
 
 int i = 1; // counter for different types of interrupts
 
int main(void)
{
 Serial.begin(9600);
 unsigned char *portD; // for input
 portD = (unsigned char *) 0x2B;
 // Enable the pull-up resistor on PD2 using the Port D
 // Data Register (PORTD)
 *portD |= BIT2_MASK; // b 0000 0100
  
 //setting interrupt 0
  AT328_EICRA = (unsigned char *) 0x69;
  *AT328_EICRA |= BIT0_MASK; // b 0000 0001 --> b xxxx xxx1 
  *AT328_EICRA |= BIT1_MASK; // b 0000 0010 --> b xxxx xx1x 
  
 // Enable external interrupt 0 using the External Interrupt Mask Register
 // (EIMSK)
 unsigned char *AT328_EIMSK;
 AT328_EIMSK = (unsigned char *) 0x3D;
 *AT328_EIMSK |= BIT0_MASK; // b 0000 0001 --> b xxxx xxx1 --> INT0 enable

 // Configure PB5 as an output using the Port B Data Direction Register
 // (DDRB)
 unsigned char *portDDRB;
 portDDRB = (unsigned char *) 0x24;
 *portDDRB |= BIT5_MASK; // PORTB bit 5 as output (bit --> 1) --> b 0010 0000 --> b xx1x xxxx

//Configure PC3 as an output as well using the Port C Data Direction Register
unsigned char *portDDRC;
portDDRC = (unsigned char *) 0x27; // for DDRC
*portDDRC |= BIT3_MASK; //PORTC bit 3 as output (bit -->1) --> b 0000 1000 --> xxxx 1xxx
 
 // Enable interrupts using SREG
 unsigned char *ptrSREG;
 ptrSREG = (unsigned char *) 0x5F;
 *ptrSREG |= BIT7_MASK; // b 1000 0000 == 0x80 --> b 1xxx xxxx
 
Serial.println("In MAIN");
 // Loop forever
 while (1)
 {
  unsigned char *portB; // for internal LED
 portB = (unsigned char *) 0x25; // assigning internal LED value

 unsigned char *portC; // for external LED
 portC = (unsigned char *)0x28;

 bool check = true; // button register delayer bool

    if(internal) { // if internal is true
      check = false;
      if(!check) {
      //internal flashing loop
      *portC &= ~0x08; // turns off external // make sure external is OFF by logical AND portD with 0xDF hex value
      *portB |= 0x20; // internal ON with logical OR of portb & 0x20
      MyDelay(5); // optimal delay value
      *portB &= ~0x20; // intenal OFF
      MyDelay(5);// optimal delay value
      timer = timer + 5000;// adds 5000 nano seconds to the I value. I tried to line it up with the MyDelay logic
      //Serial.println("in intenal on");
      } 
    }
   
    if(external){ // if internal is true
      check = false;
      if(!check){
      //external flashing loop
      *portB &= ~0x20;// make sure internal is OFF by logical and portD with 0xDF hex value
      *portC |= 0x08; // external ON with logical OR of portD & 0x08
      MyDelay(5); //optimal delay value
      *portC &= ~0x08;// external OFF
      MyDelay(5); //optimal delay
      timer = timer + 5000; // adds 5000 nano seconds to the I value. I tried to line it up with the MyDelay logic.
      //Serial.println("in external on");
      }
    }
  
}

}

//interupt function
ISR(INT0_vect){
  Serial.println("Test1");
  
if(interrupt){

  // each interrupt I wil change the inturrupt method falling edge, low status... ect
    interrupt = false;// change it false for it only runs once
    switch(i){
      case 0 : // rising edge
        if(timer >= 500000){
          AT328_EICRA = (unsigned char *) 0x69;
          *AT328_EICRA |= BIT0_MASK; // b 0000 0001 --> b xxxx xxx1 
          *AT328_EICRA |= BIT1_MASK; // b 0000 0010 --> b xxxx xx1x 
          internal = true; // turn on internal led
          external = false; // turn off external led
          Serial.println("in rising edge"); // for testing 
          i++;
          timer = 0;
        }
        break;
      case 1 : // falling edge
        if(timer >= 500000) {
          AT328_EICRA = (unsigned char *) 0x69;
          *AT328_EICRA |= BIT1_MASK; // b 0000 0010 --> b xxxx xx1x
          internal = false; // turn off internal led
          external = true; // turn on external 
          Serial.println("in falling edge"); // for testing
          i++;
          timer = 0;
        }
        break;
      case 2 : // low status 
        if(timer >= 500000) {
          AT328_EICRA = (unsigned char *) 0x69;
          internal = true; // turn on internal 
          external = false;// turn off external
          Serial.println("in low status"); // for testing
          i++;
          timer = 0; // set timer back to zero
        }
        break;
      case 3 : // any logical change 
        if(timer >= 500000) {
          AT328_EICRA = (unsigned char *) 0x69;
          *AT328_EICRA |= BIT0_MASK; // b 0000 0001 --> b xxxx xxx1
          internal = false; // turn off internal 
          external = true; // turn on external
          Serial.println("in any logical change"); // for testing
          i = 0;
          timer = 0; 
        }
        break;
    } 
  
  }
 interrupt = true; // set interrupt back to true
 
 
 
//Serial.println(interrupt); //for testing 


}
// MyDelay function given by instructor 
void MyDelay(unsigned long mSec) // takes an extended size varible for number storage 
{
  volatile unsigned long i; //volatile unsigned long makes sure the state change is immediately visible in the loop function 
  unsigned long endT = 1000 * mSec; // * the input varible by 1000

  for (i = 0; i < endT; i++); // the loop running is what actually creates a delay 
}
