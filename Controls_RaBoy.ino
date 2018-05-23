//Controls for arduino Retropie

#include <Joystick.h>

//These pins will be threated as input pins for buttons. The position within the array defines the button number. i.e. first pin in array will be button #1
int buttonPins[] = {2,3,4,5,6,7,8,9,10,16,14,15,A0,A1,A2,A3}; 
//Array to hold current state of button
int previousButtonState[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //

//int i;                          // FOR iterator int

void setup() {
  // put your setup code here, to run once:

  Joystick.begin();
  
  //loop through defined button pins and assign pinmode
  for (int i = 0; i < (sizeof(buttonPins)/sizeof(buttonPins[i])); i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    //Serial.print(i);
  }  
}

void loop() {
  
  //loop through all buttons and set correct passthrough current state
  for (int i = 0; i < (sizeof(buttonPins)/sizeof(buttonPins[i])); i++) {
      ButtonHandler(buttonPins[i], i);
  }
  
  delay(50);

}

void ButtonHandler (int Pin, int buttonNumber) {

  int currentButtonState = !digitalRead(Pin);
  
  if ( currentButtonState!= previousButtonState[buttonNumber]) {
    //Send state and update previous state
    Joystick.setButton(buttonNumber, currentButtonState);
    previousButtonState[buttonNumber] = currentButtonState;
  };
}

