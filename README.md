<<<<<<< HEAD
ClickEncoder
=============

> Arduino library to handle rotary encoders **with** buttons as a user input device.


Encoder and button can be connected to any input pin, as this library requires it's timer interrupt service routine ClickEncoder:service() to be called every millisecond. The example uses [TimerOne] for that.


### Encoder
The library supports **acceleration**, so when the encoder is rotated faster, the encoders value will increment faster.

Acceleration can be enabled or disabled at runtine using `setAccelerationEnabled(bool)`.

For instance, it makes sense to disable acceleration when entering a configuration menu that will be navigated using the encoder.

**Please note** that the acceleration parameters have been tuned for **1ms timer** intervals, and need to be changed if you decide to call the service method in another interval. (You'd need to increase ENC_ACCEL_INC and ENC_ACCEL_INC).

Depending on the type of your encoder, you can define use the constructors parameter `stepsPerNotch` an set it to either `1`, `2` or `4` steps per notch, with `1` being the default.

If you have trouble with certain encoders, try 

    #define ENC_DECODER (1 << 2)

to use a table-based decoder, which can then be tuned using 

    #define ENC_HALFSTEP

The default is ENC_HALFSTEP 1.

### Button
The Button reports multiple states: `Clicked`, `DoubleClicked`, `Held` and `Released`. You can fine-tune the timings in the library's header file.

If your encoder does not have a button, and you need to save program memory, use `#define WITHOUT_BUTTON 1`
prior including `ClickEncoder.h`, and ignore the third parameter `BTN` of the constructor.


Please see `examples/ClickEncoderTest/ClickEncoderTest.ino` for usage.


[TimerOne]:http://playground.arduino.cc/Code/Timer1
=======
RotaryEncoder with Button
=============

**See Arduino Library in the [Branch arduino]**

Atmel AVR C++ RotaryEncoder with Button Implementation.

- Timer-Based: Works on any IO-Pin.
- Supports rotary acceleration, so when the encoder is rotated faster, the encoders value will increment faster
- Button reports multiple states: `Clicked`, `DoubleClicked`, `Held` and `Released`


[Branch arduino]:https://github.com/0xPIT/encoder/tree/arduino
>>>>>>> 733e1f515a82d04bb14246524af6e830e461ad05
