//Dylan Dennison
//Project 3 part 1
// 11/1/21


bool flag = true; //flag for switching between on and off
unsigned char *portB; // internal LED instance

void setup() {
Serial.begin(9600);
  
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
}

ISR(TIMER1_COMPA_vect) {

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

void loop() {
  // put your main code here, to run repeatedly:

}
