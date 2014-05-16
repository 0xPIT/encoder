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

// ----------------------------------------------------------------------------

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "Arduino.h"

// ----------------------------------------------------------------------------

#define ENC_NORMAL        (1 << 1)   // uses Peter Danneger's decoder
#define ENC_FLAKY         (1 << 2)   // uses Table-based decoder

// ----------------------------------------------------------------------------

#ifndef ENC_DECODER
#  define ENC_DECODER     ENC_NORMAL
#endif

// ----------------------------------------------------------------------------

#if ENC_DECODER == ENC_FLAKY
#  define ENC_STEPS       1
#  ifndef ENC_HALFSTEP
#    define ENC_HALFSTEP  1        // use table for half step
#  endif
#else
#  ifndef ENC_STEPS
#    define ENC_STEPS     2        // Encoder Type: 1, 2 or 4 steps per notch
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
  ClickEncoder(uint8_t A, uint8_t B, uint8_t BTN, bool active = LOW);
  
public:
  void init(void);
  void service(void);
  
public:
  void setDoubleClickEnabled(const bool &d)
  {
    doubleClickEnabled = d;
  }

  const bool getDoubleClickEnabled()
  {
    return doubleClickEnabled;
  }

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

public:
  int16_t getValue(void);
  Button getButton(void);
  
private:
  const uint8_t pinA;
  const uint8_t pinB;
  const uint8_t pinBTN;
  const bool pinsActive;

  volatile uint16_t serviceTicks;
  volatile int16_t delta;
  volatile int16_t last;
  volatile Button button;
  bool doubleClickEnabled;
  bool accelerationEnabled;
  volatile uint16_t acceleration;
#if ENC_DECODER == ENC_FLAKY
  static const int8_t table[16];
#endif
};

// ----------------------------------------------------------------------------

#endif // __have__ClickEncoder_h__
