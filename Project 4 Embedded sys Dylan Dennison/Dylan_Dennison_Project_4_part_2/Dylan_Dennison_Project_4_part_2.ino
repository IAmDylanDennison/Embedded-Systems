//Dylan Dennison
//Project 4 part 2
//11/19/21


#define BIT0_MASK 0x01; // b 0000 0001
#define BIT2_MASK 0x04; // b 0000 0100
#define BIT7_MASK 0x80; // b 1000 0000

// High when a value is ready to be read
volatile int readFlag;

// Value to store analog result
volatile int analogVal;

bool flag = true; //flag for switching between on and off
unsigned char *portB; // internal LED instance

int counter = 0; // global counter

unsigned char *ADMUX_ESP;
unsigned char *ADCSRA_ESP;
unsigned char *ADCSRB_ESP;
unsigned char *ADCL_ESP;
unsigned char *ADCH_ESP;

// Initialization
void setup(){
  // Serial Communication
   Serial.begin(9600);


//ADC configuration----------------------
  // Registers' Address Setting
  ADMUX_ESP = (unsigned char*) 0x7c;
  ADCSRA_ESP = (unsigned char*) 0x7a;
  ADCSRB_ESP = (unsigned char*) 0x7b;
  ADCL_ESP = (unsigned char*) 0x78;
  ADCH_ESP = (unsigned char*) 0x79;

  // clear ADLAR in ADMUX (0x7C) to right-adjust the result
  // ADCL will contain lower 8 bits, ADCH upper 2 (in last two bits)
  *ADMUX_ESP &= B11011111; // 0x7c
 
  // Set REFS1..0 in ADMUX (0x7C) to change reference voltage to the
  // proper source (01)
  *ADMUX_ESP |= B01000000;
 
  // Clear MUX3..0 in ADMUX (0x7C) in preparation for setting the analog
  // input (A0)
  *ADMUX_ESP &= B11110000;
 
  // Set MUX3..0 in ADMUX (0x7C) to read from AD8 (Internal temp)
  // Do not set above 15! You will overrun other parts of ADMUX. A full
  // list of possible inputs is available in Table 24-4 of the ATMega328
  // datasheet
  *ADMUX_ESP |= B00000000; // Binary equivalent
 
  // Set ADEN in ADCSRA (0x7A) to enable the ADC.
  // Note, this instruction takes 12 ADC clocks to execute
  *ADCSRA_ESP |= B10000000; // 0x7a
 
  // Set ADATE in ADCSRA (0x7A) to enable auto-triggering.
  *ADCSRA_ESP |= B00100000;
 
  // Clear ADTS2..0 in ADCSRB (0x7B) to set trigger mode to free running.
  // This means that as soon as an ADC has finished, the next will be
  // immediately started.
  *ADCSRB_ESP &= B11111000;
 
  // Set the Prescaler to 128 (16000KHz/128 = 125KHz)
  // Above 200KHz 10-bit results are not reliable.
  *ADCSRA_ESP |= B00000111;
 
  // Set ADIE in ADCSRA (0x7A) to enable the ADC interrupt.
  // Without this, the internal interrupt will not trigger.
  *ADCSRA_ESP |= B00001000;

    
  // Kick off the first ADC
  readFlag = 0;
  // Set ADSC in ADCSRA (0x7A) to start the ADC conversion
  *ADCSRA_ESP |=B01000000;

//Timer configuration------------------
  
  unsigned char *portDDRB; // for enternal LED
  portDDRB = (unsigned char *) 0x24;

  portB = (unsigned char *) 0x25; // external LED port

 //timer varables 
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

  //setting all to default(none)
  *TCCR1A_ESP = 0x00;
  *TCCR1B_ESP = 0x00;
  *TCNT1H_ESP = 0x00;
  *TCNT1L_ESP = 0x00;
  
  *OCR1AH_ESP = 0xF4; // configed top to be 1 second real time 
  *OCR1AL_ESP = 0x00;
  
  *TCCR1B_ESP |= 0x0C; // set to ctc
  *TIMSK1_ESP |= 0x02; // enable timer compare


 //external interrupt configuration -----------------

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

 
  // Enable interrupts using SREG
  unsigned char *ptrSREG;
  ptrSREG = (unsigned char *) 0x5F;
  *ptrSREG |= 0x80; // b 1000 0000 == 0x80 --> b 1xxx xxxx

}

// Processor loop
void loop(){

  // Check to see if the value has been updated
  if (readFlag == 1){
    // Perform whatever updating needed   
    readFlag = 0;
    
  }
   
}


ISR(INT0_vect){ //button interrupt function
    unsigned char *portPinD; // for input pin
 portPinD = (unsigned char *) 0x29; //assigning pin value
 
  if (*portPinD & 0x04){ // if button press is dectected. Helps with not having multiple button hits with one press
      counter++; // increment counter ex: 1 sec -> 2 sec delay
          
      if(counter == 5){
        //Serial.println("timer overflow");
        counter = 1; // resent timer
      }
  Serial.print(" Counter: "); Serial.println(counter);
  } 
}

ISR(TIMER1_COMPA_vect) { //for timer interupt
 //Serial.println("in timer interrupt");
  switch(counter) {

  case 1:// setting ADC to Temp sensor
    *ADMUX_ESP &= B11110000; // resetting from AVcc
    *ADMUX_ESP |= B00000000; // resetting back to default value from setup
    if(analogVal >= 50) { // checking analogval
      if(flag == true) { // if true turn on
        *portB |= 0x20; //switch LED to OFF
        flag = false; // switch to off
        Serial.println("ddenni19@kent.edu");
      }
      else { // if flag is flase turn off
        *portB &= ~0x20; //turn LED off
        flag = true; //switch to on
        Serial.println("ddenni19@kent.edu");
      }
  }
    break;
  case 2:// setting ADC to A1 pin
     *ADMUX_ESP |= B00000001; //configuring 
     if(analogVal >= 50) { // checking analogval
      if(flag == true) { // if true turn on
        *portB |= 0x20; //switch LED to OFF
        flag = false; // switch to off
        Serial.println("ddenni19@kent.edu");
      }
      else { // if flag is flase turn off
        *portB &= ~0x20; //turn LED off
        flag = true; //switch to on
        Serial.println("ddenni19@kent.edu");
      }
  }

    break;
  case 3:// setting ADC to GND
    *ADMUX_ESP |= B00001111; // no flashing LED because to gnd
    break;
  case 4:// setting ADC to AVcc
    *ADMUX_ESP |= B01000000; // no flashing LED AVcc without exteranl connection
    break;
  
    }
  
}

// Interrupt service routine for the ADC completion
ISR(ADC_vect){

  // Done reading
  readFlag = 1;
  
  // Must read low first
  analogVal = ADCL | (ADCH << 8);  // ADCL 0x78 ADCH 0x79

  // Serial Comm to be learned.
  // To check the ADC values on the computer (serial monitor)
  Serial.print("jykim2@kent.edu:");
  Serial.println(analogVal);
  
  // Not needed because free-running mode is enabled.
  // Set ADSC in ADCSRA (0x7A) to start another ADC conversion
  *ADCSRA_ESP |= B01000000;
}
