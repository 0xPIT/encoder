/*
 
This example demonstrates the use of the library on an ESP8266. without using a timer interrupt

 */

#include <ClickEncoder.h>
//#include <TimerOne.h>


#define ENCODER_PINA     13
#define ENCODER_PINB     14
#define ENCODER_BTN      0

#define ENCODER_STEPS_PER_NOTCH    4   // Change this depending on which encoder is used

ClickEncoder encoder = ClickEncoder(ENCODER_PINA,ENCODER_PINB,ENCODER_BTN,ENCODER_STEPS_PER_NOTCH);

void setup() {
  Serial.begin(115200);

  encoder.setButtonHeldEnabled(true);
  encoder.setDoubleClickEnabled(true);
  
  // Enable the button to be on pin 0.  Normally pin 0 is not recognized as a valid pin for a button,
  // this is to maintain backward compatibility with an old version of the library
  // This version can have the button on pin zero, and this call enables the feature.
  // in this version best to use pin -1 instead of 0 to disable button functions
  encoder.setButtonOnPinZeroEnabled(true);

}

void loop() { 

  //Call Service in loop becasue using timer interrupts may affect ESP8266 WIFI
  //however call no more than 1 time per millisecond to reduce encoder bounce
  static uint32_t lastService = 0;
  if (lastService + 1000 < micros()) {
    lastService = micros();                
    encoder.service();  
  }

 
static int16_t last, value;
    value += encoder.getValue();
  
  if (value != last) {
    last = value;
    Serial.print("Encoder Value: ");
    Serial.println(value);

  }
  
  ClickEncoder::Button b = encoder.getButton();
  if (b != ClickEncoder::Open) {
    Serial.print("Button: ");
    #define VERBOSECASE(label) case label: Serial.println(#label); break;
    switch (b) {
      VERBOSECASE(ClickEncoder::Pressed);
      VERBOSECASE(ClickEncoder::Held)
      VERBOSECASE(ClickEncoder::Released)
      VERBOSECASE(ClickEncoder::Clicked)
      VERBOSECASE(ClickEncoder::DoubleClicked)
    }
  }    
}
