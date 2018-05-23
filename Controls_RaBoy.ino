#include <avr/sleep.h>
#include <Joystick.h>

//These pins will be threated as input pins for buttons. The position within the array defines the button number. i.e. first pin in array will be button #1
int buttonPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 16, 14, 15, A0, A1, A2, A3};
//Array to hold current state of button
int previousButtonState[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //
int sleepStatus = 0; // variable to store a request for sleep
int count = 0; // counter

int wakePin = 3;                 // pin used for waking up
int ledPin = 13;                 //onboard LED pin
bool blinkState = false;          // variable to store a request for sleep

unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;

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
  delay(10000);
  Serial.println("Immediately before attachInterupt");
  attachInterrupt(0, wakeUpNow, LOW);
  Serial.println("Immediately after attachInterupt");
}

void wakeUpNow() {
  previousMillis1 = millis();

}

void sleepNow()         // here we put the arduino to sleep
{
  // disable the USB
  USBCON |= _BV(FRZCLK);  //freeze USB clock
  PLLCSR &= ~_BV(PLLE);   // turn off USB PLL
  USBCON &= ~_BV(USBE);   // disable USB

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  //attachInterrupt(0, wakeUpNow, LOW);
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

  if (currentMillis - previousMillis1 >= 30000) {
    Serial.println("Timer: Entering Sleep mode");
    delay(100); // this delay is needed, the sleep
    //function will provoke a Serial error otherwise!!
    count = 0;
    sleepNow(); // sleep function called here
  }

  // display information about the counter
  Serial.print("Awake for ");
  Serial.print((currentMillis - previousMillis1) / 1000);
  Serial.println("sec");

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
    previousMillis1 = millis();
  };
}

