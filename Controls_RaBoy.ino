#include <Joystick.h>

// Create the Joystick
Joystick_ Joystick;

int xPin = A0;                   // Joystick X Analog Pin
int yPin = A1;                   // Joystick Y Analog Pin

int ButtonPins[] = {2,3,4,5,6,7,8,9,10,16,14,15,A0,A1,A2,A3}; //These pins will be threated as input pins for buttons. The position within the array defines the button number. i.e. first pin in array will be button #1

int defaultX;                    //used for calibration
int defaultY;                    //used for calibration
int lowRange = -127;            //Joystick boundary low 
int highRange = 127;            //Joystick boundary high

int i;                          // FOR iterator int

int DeadZone = 20;              //Sets the deadzone where we should pass on 0 as state. 

void setup() {
  // put your setup code here, to run once:

  Joystick.begin();
  
  //loop through defined button pins and assign pinmode
  for (i = 0; i < (sizeof(ButtonPins)/sizeof(ButtonPins[i])); i++) {
    pinMode(ButtonPins[i], INPUT_PULLUP);
    //Serial.print(i);
  }

 /* defaultX = analogRead(xPin);
  Serial.print("default X");
  Serial.println(defaultX);
  
  defaultY = analogRead(yPin);
  Serial.print("default Y");
  Serial.println(defaultY);
  //set joystick axis to 0
  Joystick.setXAxis(0);
  Joystick.setYAxis(0);*/
  
}

void loop() {

  /*Read the analog pins connected to the joystick. Then map the output to the defined low/high range and pass it through as axis value.
  int xpos = map(analogRead(xPin), defaultX-512, defaultX+512, lowRange, highRange);
  int ypos = map(analogRead(yPin), defaultY-512, defaultY+512, lowRange, highRange);

  //verify deadzone
  if ((xpos > DeadZone) || (xpos < (DeadZone*-1)) ) {
    Joystick.setXAxis(xpos);  
    Serial.print(" X");
    Serial.println(xpos); 
  } else {
    //we are within deadzone so we need to communicate 0. Else we will keep on going one direction
    Joystick.setXAxis(0);   
  }

  if ((ypos > DeadZone) || (ypos < (DeadZone*-1)) ) {
    Joystick.setYAxis(ypos);
    Serial.print(" Y");
    Serial.println(ypos);  
  } else {
    //we are within deadzone so we need to communicate 0. Else we will keep on going one direction
    Joystick.setYAxis(0);   
  }
*/
  
  delay(10);
  
  //loop through all buttons and set correct passthrough current state
  for (i = 0; i < (sizeof(ButtonPins)/sizeof(ButtonPins[i])); i++) {
      ButtonHandler(ButtonPins[i], i);
  }
  
  delay(10);

}

void ButtonHandler (int Pin, int ButtonNumber) {
  
  if ( digitalRead(Pin) == HIGH) {
    //button not pressed
    Joystick.releaseButton(ButtonNumber);
  } else {
    //button pressed
    Joystick.pressButton(ButtonNumber);
    Serial.print("Button pressed: ");
    Serial.print(ButtonNumber);
  };

}

