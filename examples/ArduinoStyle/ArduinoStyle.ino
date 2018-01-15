#include <ClickEncoder.h>
#include <TimerOne.h>

int16_t oldEncPos, encPos;
uint8_t buttonState;

#define pinA A2
#define pinB A1
#define pinSw A0 //switch
#define STEPS 4

ClickEncoder encoder(pinA, pinB, pinSw, STEPS);

void setup() {
  Serial.begin(9600);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);

  encoder.setAccelerationEnabled(true);

  Serial.print("Acceleration is ");
  Serial.println((encoder.getAccelerationEnabled()) ? "enabled" : "disabled");

  oldEncPos = -1;
}

void loop() {
  encPos += encoder.getValue();

  if (encPos != oldEncPos) {
    oldEncPos = encPos;
    Serial.print("Encoder Value: ");
    Serial.println(encPos);
  }

  buttonState = encoder.getButton();

  if (buttonState != 0) {
    Serial.print("Button: "); Serial.println(buttonState);
    switch (buttonState) {
      case ClickEncoder::Open:          //0
        break;

      case ClickEncoder::Closed:        //1
        break;

      case ClickEncoder::Pressed:       //2
        break;

      case ClickEncoder::Held:          //3
        break;

      case ClickEncoder::Released:      //4
        break;

      case ClickEncoder::Clicked:       //5
        break;

      case ClickEncoder::DoubleClicked: //6
        break;
    }
  }
}

void timerIsr() {
  encoder.service();
}
