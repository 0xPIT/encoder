// ----------------------------------------------------------------------------
// Rotary Encoder Driver with Acceleration
// Supports Click, DoubleClick, Long Click
//
// (c) 2010 karl@pitrich.com
// (c) 2014 karl@pitrich.com
// 
// Timer-based rotary encoder logic by Peter Dannegger
// http://www.mikrocontroller.net/articles/Drehgeber
// ----------------------------------------------------------------------------

#ifndef __have__ClickEncoder_h__
#define __have__ClickEncoder_h__

// ---Button defaults-------------------------------------------------------------

#define BTN_DOUBLECLICKTIME  400  // second click within 400ms
#define BTN_HOLDTIME        1000  // report held button after 1s


// ----------------------------------------------------------------------------

#include <stdint.h>
#if defined(__AVR__)
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#endif
#include "Arduino.h"

// ----------------------------------------------------------------------------

#define ENC_NORMAL        (1 << 1)   // use Peter Danneger's decoder
#define ENC_FLAKY         (1 << 2)   // use Table-based decoder

// ----------------------------------------------------------------------------

#ifndef ENC_DECODER
#  define ENC_DECODER     ENC_NORMAL
#endif

#if ENC_DECODER == ENC_FLAKY
#  ifndef ENC_HALFSTEP
#    define ENC_HALFSTEP  1        // use table for half step per default
#  endif
#endif

// ----------------------------------------------------------------------------

class ClickEncoder
{
public:
  typedef enum Button_e {
    Open = 0,
    Closed,
    
    Pressed,
    Held,
    Released,
    
    Clicked,
    DoubleClicked
    
  } Button;

public:
  ClickEncoder(int8_t A, int8_t B, int8_t BTN = -1, 
               uint8_t stepsPerNotch = 1, bool active = LOW);
			   
#ifndef WITHOUT_BUTTON
  explicit ClickEncoder(int8_t BTN, bool active = LOW);
  explicit ClickEncoder(int8_t BTN, int16_t rangeLow, int16_t rangeHigh);  // Constructor for using alaog input range as a button

#endif

  void service(void);  
  int16_t getValue(void);

#ifndef WITHOUT_BUTTON
public:
  Button getButton(void);
#endif

#ifndef WITHOUT_BUTTON
public:
  void setDoubleClickTime(uint16_t durationMilliseconds)
  {
    buttonDoubleClickTime = durationMilliseconds;
  }
  
public:
  void setHoldTime(uint16_t durationMilliseconds)
  {
    buttonHoldTime = durationMilliseconds;
  }
  
public:
  void setDoubleClickEnabled(const bool &d)
  {
    doubleClickEnabled = d;
  }

  const bool getDoubleClickEnabled()
  {
    return doubleClickEnabled;
  }
  
public:
  void setButtonHeldEnabled(const bool &d)
  {
    buttonHeldEnabled = d;
  }

  const bool getButtonHeldEnabled()
  {
    return buttonHeldEnabled;
  }
  
public:
  void setButtonOnPinZeroEnabled(const bool &d)
  {
    buttonOnPinZeroEnabled = d;
  }

  const bool getButtonOnPinZeroEnabled()
  {
    return buttonOnPinZeroEnabled;
  }
#endif

public:
  void setAccelerationEnabled(const bool &a)
  {
    accelerationEnabled = a;
    if (accelerationEnabled == false) {
      acceleration = 0;
    }
  }

  const bool getAccelerationEnabled() 
  {
    return accelerationEnabled;
  }

private:
  const int8_t pinA;
  const int8_t pinB;
  const int8_t pinBTN;
  const bool pinsActive;
  volatile int16_t delta;
  volatile int16_t last;
  volatile uint8_t steps;
  volatile uint16_t acceleration;
  bool accelerationEnabled;
#if ENC_DECODER != ENC_NORMAL
  static const int8_t table[16];
#endif
#ifndef WITHOUT_BUTTON
  volatile Button button;
  bool doubleClickEnabled;
  bool buttonHeldEnabled;
  bool buttonOnPinZeroEnabled = false;
  bool analogInput = false;
  uint16_t keyDownTicks = 0;
  uint16_t doubleClickTicks = 0;
  uint16_t buttonHoldTime = BTN_HOLDTIME;
  uint16_t buttonDoubleClickTime = BTN_DOUBLECLICKTIME;
  unsigned long lastButtonCheck = 0;
  int16_t anlogActiveRangeLow = 0;
  int16_t anlogActiveRangeHigh = 0;
  bool getPinState();
#endif
};

// ----------------------------------------------------------------------------

#endif // __have__ClickEncoder_h__
