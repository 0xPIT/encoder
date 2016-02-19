/*
 
This example demonstrates the use of 2 devices controlled by the ClickEncoder library

In thie example, the 2 devides are a rotary encoder with built in button, and a normal momentary button

 */
#include <ClickEncoder.h>
#include <TimerOne.h>

#define ENCODER_PINA     5   // If the encoder moved in the wrong direction, swap PINA and PINB
#define ENCODER_PINB     6
#define ENCODER_BTN      7

#define ENCODER_STEPS_PER_NOTCH    4   // Change this depending on which encoder is used

#define EXTRA_BUTTON_PIN 8

ClickEncoder button = ClickEncoder(EXTRA_BUTTON_PIN);
ClickEncoder encoder = ClickEncoder(ENCODER_PINA,ENCODER_PINB,ENCODER_BTN,ENCODER_STEPS_PER_NOTCH);

void timerIsr() {      //Service methods from both instances must be included
  encoder.service();
  button.service();
}

void setup() {
  Serial.begin(115200);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr); 
  
  encoder.setButtonHeldEnabled(false);
  encoder.setDoubleClickEnabled(true);
  
  button.setButtonHeldEnabled(true);
  button.setDoubleClickEnabled(false);

}

void loop() {  
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
  
  ClickEncoder::Button b2 = button.getButton();
  if (b2 != ClickEncoder::Open) {
    Serial.print("Button2: ");
    #define VERBOSECASE(label) case label: Serial.println(#label); break;
    switch (b2) {
      VERBOSECASE(ClickEncoder::Pressed);
      VERBOSECASE(ClickEncoder::Held)
      VERBOSECASE(ClickEncoder::Released)
      VERBOSECASE(ClickEncoder::Clicked)
      VERBOSECASE(ClickEncoder::DoubleClicked)
    }
  }    
}
