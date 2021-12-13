//Dylan Dennison
//Project 1 part 2
//10/02/21

#define BIT3_MASK 0x08 // 0000 1000 for PB3
#define BIT5_MASK 0x20 // 0010 0000 for PD5 & PB5

#define PORTB_MASK 0x25 // 0010 0101
#define DDRB_MASK 0x24  // 0010 0100
#define PINB_MASK 0x23  // 0010 0011

#define PORTD_MASK 0x2B // 0010 1011
#define DDRD_MASK 0x2A  // 0010 1010
#define PIND_MASK 0x29  // 0010 1001

long i = 500000; // one second delay varible for delay between the button presses
//I used 500000 because that seemed to be a second while running it. Hope that is the same for you haha
void MyDelay(unsigned long mSec); // declaring MyDelay function

// The setup function runs once when you press reset or power the board
void setup() {
  // put your setup code here, to run once:
  unsigned char *portDDRB; //creating DDRB varibles 
  unsigned char *portDDRD;

  portDDRB = (unsigned char *) DDRB_MASK; //Assigning them to The MASKs defined above
  portDDRD = (unsigned char *) DDRD_MASK;

  *portDDRB &= ~BIT3_MASK;  // Configure port B bit 3 as an input using logical AND with the 3 bit of portDDRB
  *portDDRD |= BIT5_MASK;   // Configure port D bit 5 as an output using logical OR with the 5th bit portDDRD
  *portDDRB |= BIT5_MASK;   // Configure Port B bit 5 as an output using logical OR with the 5th bit portDDRB
}

void loop() {

  // initializing varibles 
  unsigned char *portPinB; // creating instance of pin B for the input button
  portPinB = (unsigned char *) PINB_MASK; // for pin B bit 3

  unsigned char *portD; // creating instance of port D for the external LED
  portD = (unsigned char *) PORTD_MASK; // for port D bit 5

  unsigned char *portB; //creating instance of port B for the internal LED
  portB = (unsigned char *) PORTB_MASK; // for port B bit 5

  bool buttonReleased = false; //boolean for when the switch is released. 
  bool buttonWasPressed = false;//boolean for when the switch is toggled.
  /*I had a problem with using only one bool for the button pressed. Seemed the 
  condition with one bool was happening too fast and it wouldnt switch on and off well. Sometimes it would
  switch and sometimes it wouldnt. I figured out using two booleans with a release value helped with this problem
  Basically, it was switching between the two while loops too quick. And just pressing the button would sometimes make it jump
  back and forth too soon. Having two booleans with two if statements seem to fix that problem.*/
  
  while (buttonReleased == false ) { //while the buttonReleased boolean is set to false
    
     if((*portPinB & BIT3_MASK) == 0x08 && i >= 500000){ // if the button is pressed. (*portB & BIT3_MASK) == 0x08) would mean the input has been pressed becasue the 3rd bit will be on(1)
      // and if the i value is 500000 nano seconds which is one second(on my computer)
      buttonWasPressed = true; // set the boolean value to true since the button was pressed
      i = 0; // start timer
    }
    else if(buttonWasPressed == true ) { //if the button was pressed ^^^
      buttonReleased = true; // button release is set to true. 
    }
    /*this is what I was talking about when I declared the boolean varibles ^^^. I had only one boolean value and I think the press of the button with just one if
    if statement would be too long of an execution. This would cause the board to go back in fourth between too fast. Saying one push could go back and forth. Sometimes 
    back to the orginal led. With the second boolean and the else if statement, I can check if the button was released. To give the program more time to realize the button was only
    pressed once.*/
    
    //internal flashing loop
    *portD &= 0xDF; // make sure external is OFF by logical AND portD with 0xDF hex value
    *portB |= 0x20; // internal ON with logical OR of portb & 0x20
    MyDelay(5); // optimal delay value
    *portB &= 0xDF; // intenal OFF
    MyDelay(5);// optimal delay value
    i = i + 5000;// adds 5000 nano seconds to the I value. I tried to line it up with the MyDelay logic
    }
 
  
  buttonReleased = false; //reset boolean values to false to enter the next loop
  buttonWasPressed = false;
 
   while (buttonReleased == false) {//while the buttonReleased boolean is set to false
    if((*portPinB & BIT3_MASK) == 0x08 && i >= 500000){// if the button is pressed. (*portB & BIT3_MASK) == 0x08) would mean the input has been pressed
      // and if the i value is 500000 nano seconds which is one second(on most computers)
      buttonWasPressed = true; // set the boolean value to true since the button was pressed
      i = 0;
    }
    else if(buttonWasPressed == true){  //if the button was pressed ^^^
      buttonReleased = true; // button release is set to true.
    }
    
    //external flashing loop
    *portB &= 0xDF;// make sure internal is OFF by logical and portD with 0xDF hex value
    *portD |= 0x20; // external ON with logical OR of portD & 0x20
    MyDelay(5); //optimal delay value
    *portD &= 0xDF;// external OFF
    MyDelay(5); //optimal delay
    i = i + 5000; // adds 5000 nano seconds to the I value. I tried to line it up with the MyDelay logic.
  }

  //second delay functionality
  /*I had a hard time trying to figure out the second delay for the button after it switches LEDs. To do this I created my own counter. 
  The counter has the same logic as the MyDelay function for the timing. If you look at the flashing loop(optimal delay) you can see the value I 
  have is five(MyDelay(5);). This would be equal to 5000 nano seconds. So everytime that loop runs it adds 5000 to the global varible i. The condition for the button
  to be hit also has the condition of i >= 500000. This means a second(on my computers hardware) will have to pass before the button can switch the boolean
  to switch the LED again. Was tricky but it works!*/
   
}

// MyDelay function given by instructor 
void MyDelay(unsigned long mSec) // takes an extended size varible for number storage 
{
  volatile unsigned long i; //volatile unsigned long makes sure the state change is immediately visible in the loop function 
  unsigned long endT = 1000 * mSec; // * the input varible by 1000

  for (i = 0; i < endT; i++); // the loop running is what actually creates a delay 
}
