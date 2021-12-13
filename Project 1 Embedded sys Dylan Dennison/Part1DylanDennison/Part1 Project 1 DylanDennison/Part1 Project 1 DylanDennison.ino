//Dylan Dennison
//Project 1 part 1
//10/02/21
// I only used the external LED for this part. In his lectures he said this would be okay


#define BIT3_MASK 0x08 // 0000 1000 PB3
#define BIT5_MASK 0x20 // 0010 0000 PD5

#define PORTB_MASK 0x25 // 0010 0101
#define DDRB_MASK 0x24  // 0010 0100
#define PINB_MASK 0x23  // 0010 0011

#define PORTD_MASK 0x2B // 0010 1011
#define DDRD_MASK 0x2A  // 0010 1010
#define PIND_MASK 0x29  // 0010 1001

void MyDelay(unsigned long mSec); // declaring MyDelay function

// The setup function runs once when you press reset or power the board
void setup() {
  // put your setup code here, to run once:
  unsigned char *portDDRB; //creating DDRB varibles 
  unsigned char *portDDRD;

  portDDRB = (unsigned char *) DDRB_MASK; //Assigning them to The MASKs defined above
  portDDRD = (unsigned char *) DDRD_MASK;

  *portDDRB &= ~BIT3_MASK;  // Configure port B bit 3 as an input with logical AND with bit 3
  *portDDRD |= BIT5_MASK;   // Configure port D bit 5 as an output with logical OR with bit 5
  
  unsigned char *portD;
  portD = (unsigned char *) PORTD_MASK;  
  *portD |= 0x20; //setting the external LED on. This means the external LED will be on when board is first turned on. 
  //used a logic or to make sure the fith bit of 5 is on(1)
}

void loop() {
  unsigned char *portPinB; // creating instance of portPinB the input
  portPinB = (unsigned char *) PINB_MASK; // for pin B

  unsigned char *portD;// creating instance of portD for the output LED
  portD = (unsigned char *) PORTD_MASK; // for port D
 
  if ((*portPinB & BIT3_MASK) == 0x08) { // if the button is pressed. (*portB & BIT3_MASK) == 0x08) would mean the input has been pressed because the 3 bit would be(1)
    
     if ((*portD & BIT5_MASK) == 0x20){ // if the PortD 5th bit is already on 
        *portD &= 0xDF; // turns off with logical AND of Port D and hex value DF so the 5th bit of portD will be off(0)
        MyDelay(500);// I found 500 is a second on my board. I used a stop watch to conclude this
     } 
     else { // if PortD 5th bit is off
       *portD |= 0x20; //this will turn on the LED with a logical or with portD & hex value 20 and the 5th bit will be turned on(1)
       MyDelay(500);// Delay for the button to no be able to switch again
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
