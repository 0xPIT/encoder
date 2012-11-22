// ----------------------------------------------------------------------------
// Rotary Encoder Driver with Acceleration
// Supports Click, DoubleClick, Long Click
//
// (c) 2010 karl@pitrich.com
// based on timer-based rotary encoder logic by Peter Dannegger
// http://www.mikrocontroller.net/articles/Drehgeber
// ----------------------------------------------------------------------------

#ifndef __have__encoder_h__
#define __have__encoder_h__

// ----------------------------------------------------------------------------

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

// ----------------------------------------------------------------------------
// Hardware configuration
//
#define ENC_PINS        PIND
#define ENC_PORT        PORTD
#define ENC_DDR         DDRD

#define ENC_PIN_BUTTON  PD7
#define ENC_PIN_PHASEA  PD6
#define ENC_PIN_PHASEB  PD5

#define ENC_NORMAL      (1<<1)
#define ENC_FLAKY       (1<<2)

#define ENC_DECODER     ENC_NORMAL

#if ENC_DECODER == ENC_FLAKY
#  define ENC_STEPS     1
#else
#  define ENC_STEPS     2         // Encoder Type: 1, 2 or 4 steps per notch
#endif

// ----------------------------------------------------------------------------
// Button configuration
//
#define ENC_CLICKTIME         5   // > 50ms = click, debouce time
#define ENC_DOUBLECLICKTIME  50   // second click within 500ms
#define ENC_HOLDTIME        120   // report held button after 1.2s

// ----------------------------------------------------------------------------
// Acceleration configuration
//
#define ENC_ACCEL_TOP      3072   // max. acceleration: *12 (val >> 8)
#define ENC_ACCEL_INC        25
#define ENC_ACCEL_DEC         2

// ----------------------------------------------------------------------------

namespace State {
  typedef enum Button_e {
    Undefined = 0,
    
    On = 1,
    Off,
    
    Open,
    Closed,
    
    Pressed,
    Held,
    Released,
    
    Clicked,
    DoubleClicked
    
  } Button;
};

// ----------------------------------------------------------------------------

class RotaryEncoder
{
public:
  RotaryEncoder(void)
    : doubleClickEnabled(true), accelerationEnabled(true)
  {
  }
  
public:
  void init(void);
  void service(void);
  
public:
  void setDoubleClickEnabled(const bool &d)
  {
    doubleClickEnabled = d;
  }

  void setAccelerationEnabled(const bool &a)
  {
    accelerationEnabled = a;
    if (accelerationEnabled == false) {
      acceleration = 0;
    }
  }

public:
  int8_t getStep(void);
  State::Button getButton(void);
  
private:
#if ENC_DECODER != ENC_NORMAL
  static const int8_t table[16];
#endif
  volatile uint16_t serviceTicks;
  volatile int8_t delta;
  volatile int8_t last;
  volatile State::Button button;
  bool doubleClickEnabled;
  bool accelerationEnabled;
  volatile uint16_t acceleration;
};

// ----------------------------------------------------------------------------

#endif // __have__RotaryEncoder_h__
