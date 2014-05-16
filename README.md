ClickEncoder
=============

Arduino library to handle rotary encoders with buttons as user input device.

Demo Video: <link>

Encoder and button can be connected to any input pin, as this library utilizes a timer interrupt service routine.
The example uses TimerOne, you can use any interval timer to call the method ClickEncoder::service() every 1 millisecond.

== Encoder ==
The *Encoder* Supports acceleration, so when the encoder is rotated faster, the encoder's valud will increment faster
Acceleration can be enabled or disabled at runtine using setAccelerationEnabled(bool). 
For example, it makes sense to disable acceleration when entering a configuration menu that will be navigated using the encoder.

Please note that the acceleration parameters have been tuned for 1ms timer intervals, and need to be changed if you decide to call the service method in another interval. (you'd need to increase ENC_ACCEL_INC and ENC_ACCEL_INC)

If you have trouble with certain encoders, try 

#define ENC_DECODER (1 << 2)

to use a table-based decoder, which can be tuned using 

#define ENC_HALFSTEP 

or not. (default is ENC_HALFSTEP 1)

Also, depending on the type of your encoder, you can define ENC_STEPS to either 1, 2 or 4 steps per notch, with 2 being the default, as this seems to work for most encoders.

== Button ==
The *Button* reports multiple states: clicked, DoubleClicked, Held and Released
You can fine-tune timings in the header file.

If your encoder does not have a button and you need to save program memory, use 
#ifndef WITHOUT_BUTTON 
prior including ClickEncoder.h and ignore the third parameter `BTN` of the constructor.

Please see examples/ClickEncoderTest/ClickEncoderTest.ino for usage.
