// -----------------------------------------------------------------------------
// Rotary Encoder Driver with Acceleration
// Supports Click, DoubleClick, Long Click
//
// (c) 2010 karl@pitrich.com
// based on timer-based rotary encoder logic by Peter Dannegger
// http://www.mikrocontroller.net/articles/Drehgeber
// -----------------------------------------------------------------------------

#include "encoder.h"

// -----------------------------------------------------------------------------

#if ENC_DECODER != ENC_NORMAL
// decoding table for hardware with flaky notch (half resolution)
const int8_t RotaryEncoder::table[16] PROGMEM = { 0, 0, -1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, -1, 0, 0 };    

// decoding table for normal hardware
// { 0, 1, -1, 0, -1, 0, 0, 1, 1, 0, 0, -1, 0, -1, 1, 0 };    
#endif

// -----------------------------------------------------------------------------

void RotaryEncoder::init(void)
{
  delta = 0;
  last = 0;  
  serviceTicks = 0;
  acceleration = 0;
  button = State::Open;
  
  ENC_DDR &= ~(1 << ENC_PIN_BUTTON) | ~(1 << ENC_PIN_PHASEA) | ~(1 << ENC_PIN_PHASEB);
  ENC_PORT |= (1 << ENC_PIN_BUTTON) |  (1 << ENC_PIN_PHASEA) |  (1 << ENC_PIN_PHASEB);

  if (ENC_PINS & (1<<ENC_PIN_PHASEA)) {
    last = 3;
  }

  if (ENC_PINS & (1<<ENC_PIN_PHASEB)) {
    last ^= 1;
  }
}

// -----------------------------------------------------------------------------
// called every 1ms by timer ISR
//
void RotaryEncoder::service(void)
{
  serviceTicks++;
  int8_t moved = 0;

  if (accelerationEnabled) { // decelerate every tick
    acceleration -= ENC_ACCEL_DEC;
    if (acceleration & 0x8000) { // handle overflow of MSB is set
      acceleration = 0;
    }
  }

#if ENC_DECODER == ENC_FLAKY
  last = (last << 2) & 0x0F;

  if (ENC_PINS & (1<<ENC_PIN_PHASEA)) {
    last |= 2;
  }
  if (ENC_PINS & (1<<ENC_PIN_PHASEB)) {
    last |= 1;
  }

  uint8_t tbl = pgm_read_byte(&table[last]); 
  if (tbl) {
    delta += tbl;
    moved = 1;
  }
#elif ENC_DECODER == ENC_NORMAL
  int8_t curr = 0;

  if (ENC_PINS & (1<<ENC_PIN_PHASEA)) {
    curr = 3;
  }
  if (ENC_PINS & (1<<ENC_PIN_PHASEB)) {
    curr ^= 1;
  }
  
  int8_t diff = last - curr;

  if (diff & 1) {            // bit 0 = step
    last = curr;
    delta += (diff & 2) - 1; // bit 1 = direction (+/-)
    moved = 1;    
  }
#else
# error "Error: #define ENC_DECODER to NORMAL or FLAKY"
#endif

  if (accelerationEnabled && moved) {
    // increment accelerator if encoder has been moved
    if (acceleration <= (ENC_ACCEL_TOP - ENC_ACCEL_INC)) {
      acceleration += ENC_ACCEL_INC;
    }
  }

  // handle button
  //
  static uint16_t keyDownTicks = 0;    // maximum 'hold' time for 'release' to be reported depends on overflow of this integer
  static uint8_t doubleClickTicks = 0;
  
  if (!(serviceTicks % 10)) { // every 10ms is sufficient
    if (!(ENC_PINS & (1<<ENC_PIN_BUTTON))) { // key is down
      keyDownTicks++;
      if (keyDownTicks > ENC_HOLDTIME) {
        button = State::Held;
      }
    }
      
    if ((ENC_PINS & (1<<ENC_PIN_BUTTON))) { // key is now up
      if (keyDownTicks > ENC_CLICKTIME) {
        if (button == State::Held) {
          button = State::Released;
        }
        else {
          #define ENC_SINGLECLICKONLY 1
          if (doubleClickTicks > ENC_SINGLECLICKONLY) {   // prevent trigger in single click mode
            if (doubleClickTicks < ENC_DOUBLECLICKTIME) {
              button = State::DoubleClicked;
              doubleClickTicks = 0;
            }
          }
          else {
            doubleClickTicks = (doubleClickEnabled) ? ENC_DOUBLECLICKTIME : ENC_SINGLECLICKONLY;
          }
        }
      }

      keyDownTicks = 0;
    }
  
    if (doubleClickTicks > 0) {
      doubleClickTicks--;
      if (--doubleClickTicks == 0) {
        button = State::Clicked;
      }
    }
  }
}

// -----------------------------------------------------------------------------

int8_t RotaryEncoder::getStep(void)
{
  int8_t val;
  
  cli();
  val = delta;

#if ENC_STEPS == 1
  delta = 0;
#elif ENC_STEPS == 2
  delta = val & 1;
#elif ENC_STEPS == 4
  delta = val & 3;
#else
# error "Error: #define ENC_STEPS per notch to [1|2|4]"
#endif

  sei();
  
#if ENC_STEPS == 4
  val >>= 2;
#elif ENC_STEPS == 2
  val >>= 1;
#endif

  int8_t r = 0;
  int8_t accel = ((accelerationEnabled) ? (acceleration >> 8) : 0);

  if (val < 0) {
    r -= 1 + accel;
  }
  else if (val > 0) {
    r += 1 + accel;
  }

  return r;
}

// -----------------------------------------------------------------------------

State::Button RotaryEncoder::getButton(void)
{
  State::Button ret = button;
  button = State::Open; // reset
  return ret;
}
