//Dylan Dennison
//Project 1 part 2
//10/02/21

#define BIT3_MASK 0x08 // 0000 1000 PB3
#define BIT5_MASK 0x20 // 0010 0000 PD5

#define PORTB_MASK 0x25 // 0010 0101
#define DDRB_MASK 0x24  // 0010 0100
#define PINB_MASK 0x23  // 0010 0011

#define PORTD_MASK 0x2B // 0010 1011
#define DDRD_MASK 0x2A  // 0010 1010
#define PIND_MASK 0x29  // 0010 1001

bool flag = true; // boolean for loop 
void MyDelay(unsigned long mSec); // declaring MyDelay function

// The setup function runs once when you press reset or power the board
void setup() {
  // put your setup code here, to run once:
  unsigned char *portDDRB; //creating DDRB varibles 
  unsigned char *portDDRD;

  portDDRB = (unsigned char *) DDRB_MASK; //Assigning them to The MASKs defined above
  portDDRD = (unsigned char *) DDRD_MASK;

  *portDDRB &= ~BIT3_MASK;  // Configure port B bit 3 as an input
  *portDDRD |= BIT5_MASK;   // Configure port D bit 5 as an output
  *portDDRB |= BIT5_MASK;   // Configure Port B bit 5 as an output
  
  //unsigned char *portB;
  //portB = (unsigned char *) PORTB_MASK;  
  //*portB |= 0x20; //setting the internal LED on. This means the internal LED on the board will be on when board is first turned on.
}

void loop() {

  unsigned char *portPinB; // initializing varibles 
  portPinB = (unsigned char *) PINB_MASK; // for pin B

  unsigned char *portD;
  portD = (unsigned char *) PORTD_MASK; // for port D

  unsigned char *portB;
  portB = (unsigned char *) PORTB_MASK; // for port B
  
  *portB |= 0x20; // internal on
  MyDelay(7);
  *portB &= 0xDF;
  MyDelay(7);

  if ((*portPinB & BIT3_MASK) == 0x08){
     while((*portPinB & 0x08) != 0x08) {
        *portD |= 0x20; // external on
        MyDelay(7);
        *portD &= 0xDF;
        MyDelay(7);
      } 
      MyDelay(500);
    }
}

// MyDelay function given by instructor 
void MyDelay(unsigned long mSec) // takes an extended size varible for number storage 
{
  volatile unsigned long i; //volatile unsigned long makes sure the state change is immediately visible in the loop function 
  unsigned long endT = 1000 * mSec; // * the input varible by 1000

  for (i = 0; i < endT; i++); // the loop running is what actually creates a delay 
}
