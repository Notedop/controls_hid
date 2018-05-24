#include <avr/sleep.h>
#include <Joystick.h>

const int wakePin = 2;                                                                          //Pin used for waking up. Should be pin 2 or 3!
const int secondsTillSleep = 60;                                                                //Specifies the time which needs to pass without button press untill controller goes to sleep
const int buttonPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 16, 14, 15, A0, A1, A2, A3};              //These pins will be threated as input pins for buttons. The position within the array defines the button number. i.e. first pin in array will be button #1
int previousButtonState[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //Array to hold current state of button


unsigned long previousMillis1 = 0;                                                        //Used to keep track of time last button was pressed

void setup()
{

  Joystick.begin();

  //loop through defined button pins and assign pinmode
  for (int i = 0; i < (sizeof(buttonPins) / sizeof(buttonPins[i])); i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    //Serial.print(i);
  }
  
  pinMode(wakePin, INPUT_PULLUP);
  Serial.begin(9600);
  
  //comment this delay out if you're comfortable with the settings. 
  //Be warned: if the secondsTillSleep is a low value, then 
  delay(10000);
  
}

void wakeUpNow() {
  
  //ISR
  //millis() relies on interrupts to count, so it will never increment inside an ISR.
  //However we just read value and assign the value
  previousMillis1 = millis();

  //Detach the interrupt as we don't want wakeUpNow to run every time the wakePin is touched. We'll set it before going to sleep.
  detachInterrupt(digitalPinToInterrupt(wakePin));

}

void sleepNow()         // here we put the arduino to sleep
{
  // based on https://forum.sparkfun.com/viewtopic.php?f=32&t=39214&start=15#p175098
  // Attach interupt, detach USB, go to sleep, wake up, attach USB 
  // detach the USB below code does what USBDevice.detach() should do

  attachInterrupt(digitalPinToInterrupt(wakePin), wakeUpNow, LOW);

  USBCON |= _BV(FRZCLK);  //freeze USB clock
  PLLCSR &= ~_BV(PLLE);   // turn off USB PLL
  USBCON &= ~_BV(USBE);   // disable USB

  //power down
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();

  //when awakened, code returns to here
  //routine to properly wake up
  sleep_disable();
  delay(100);
  USBDevice.attach(); // keep this
  delay(100);
  Serial.begin(9600);
  delay(1000);
  Serial.println("I am awake !");
}

void loop()
{

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis1 >= (secondsTillSleep*1000)) {
    Serial.println("Timer: Entering Sleep mode");
    delay(100); // this delay is needed, the sleep
    //function will provoke a Serial error otherwise!!
    sleepNow(); // sleep function called here
  }

  // display information about the counter
  Serial.print("Awake for ");
  Serial.print((currentMillis - previousMillis1) / 1000);
  Serial.println(" seconds");

  //loop through all buttons and set correct passthrough current state
  for (int i = 0; i < (sizeof(buttonPins) / sizeof(buttonPins[i])); i++) {
    ButtonHandler(buttonPins[i], i);
  }
}


void ButtonHandler (int Pin, int buttonNumber) {

  int currentButtonState = !digitalRead(Pin);

  if ( currentButtonState != previousButtonState[buttonNumber]) {
    
    //Send state and update previous state
    Joystick.setButton(buttonNumber, currentButtonState);
    previousButtonState[buttonNumber] = currentButtonState;

    //reset sleep milliseconds 
    previousMillis1 = millis();
  };
}

