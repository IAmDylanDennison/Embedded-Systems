//Dylan Dennison
//Project 3 part 2
// 11/1/21


#define BIT0_MASK 0x01; // b 0000 0001
#define BIT2_MASK 0x04; // b 0000 0100
#define BIT7_MASK 0x80; // b 1000 0000

int counter = 0; // for timer counter
int i = 0; // keeps track of 1 second iterations
bool flag = false; // flag to switch between on and off
unsigned char *portB;// for internal LED

void setup() {
Serial.begin(9600);
  
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
  
  unsigned char *portDDRB; // for enternal LED
  portDDRB = (unsigned char *) 0x24;

  portB = (unsigned char *) 0x25;

  unsigned char *TCCR1A_ESP;
  TCCR1A_ESP = (unsigned char *) 0x80;
  unsigned char *TCCR1B_ESP;
  TCCR1B_ESP = (unsigned char *) 0x81;
  unsigned char *TCNT1H_ESP;
  TCNT1H_ESP = (unsigned char *) 0x85;
  unsigned char *TCNT1L_ESP;
  TCNT1L_ESP = (unsigned char *) 0x84;
  unsigned char *OCR1AH_ESP;
  OCR1AH_ESP = (unsigned char *) 0x89;
  unsigned char *OCR1AL_ESP;
  OCR1AL_ESP = (unsigned char *) 0x88;
  unsigned char *TIMSK1_ESP;
  TIMSK1_ESP = (unsigned char *) 0x6F;
  
  *portDDRB = 0x20;

  *TCCR1A_ESP = 0x00;
  *TCCR1B_ESP = 0x00;
  *TCNT1H_ESP = 0x00;
  *TCNT1L_ESP = 0x00;
  
  *OCR1AH_ESP = 0xF4; // setting top to 1 second real time
  *OCR1AL_ESP = 0x00;
  
  *TCCR1B_ESP |= 0x0C; // setting to ctc
  *TIMSK1_ESP |= 0x02;  // enable timer compare interrupt

// Enable interrupts using SREG
 unsigned char *ptrSREG;
 ptrSREG = (unsigned char *) 0x5F;
 *ptrSREG |= BIT7_MASK; // b 1000 0000 == 0x80 --> b 1xxx xxxx

}

ISR(TIMER1_COMPA_vect) { // timer interrupt funciton
  
if(flag == true) { 
  *portB |= 0x20; // turn LED on 
  //Serial.println("on");
  i++;
  //Serial.println("increment i on ON");
  if(i == counter){
  flag = false; //switch back to off
  i = 0; // reset time increment 
  Serial.println("ddenni19@kent.edu");
  }
  Serial.print("i: "); Serial.print(i); Serial.print(" Counter: "); Serial.println(counter);
}
else {
  *portB &= ~0x20; // turn LED off
  //Serial.println("off");
  i++;
  //Serial.println("increment i on OFF");
  if(i == counter) {
  flag = true; // go back to on
  i = 0; // reset time increment
  Serial.println("ddenni19@kent.edu");
  
  }
 Serial.print("i: "); Serial.print(i); Serial.print(" Counter: "); Serial.println(counter);
}

  
}

ISR(INT0_vect){ //button interrupt function
    unsigned char *portPinD; // for input pin
 portPinD = (unsigned char *) 0x29; //assigning pin value
 
if (*portPinD & 0x04){ // if button press is dectected. Helps with not having multiple button hits with one press
    counter++; // increment counter ex: 1 sec -> 2 sec delay
    i = 0; // reset time increment
    //Serial.println("increment timer");
        
    if(counter == 5){
      //Serial.println("timer overflow");
      counter = 1; // resent timer
      i = 0; //reset time increment
    }

}

 
}

void loop() {
  // put your main code here, to run repeatedly:

}
