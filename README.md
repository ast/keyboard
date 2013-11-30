# Arduino clone midi keyboard


A Teensy (Arduino clone) powered velocity sensitive USB MIDI keyboard with sustain pedal. Works with diode matrix keybeds. I used one from Fatar (also known as Studiologic).

Using the teensy USB MIDI library it works as a class compliant USB MIDI device.

I have only included the source and a very basic schematic here. Feel free to contact me if you have any questions.

## Circuit

As you can see on the [keyboard matrix schematic](http://www.doepfer.de/DIY/Matrix_88.gif), the 88 keys are arranged in to two diode matrices. There are actually 176 switches in the matrix. The reason for this is that there are two switches under each key. When a key is pressed one of the switches closes slightly before the other one. By measuring the time of flight between the switches we can derive the velocity.

I've included a rudimentary schematic how I use 3 74ls138 demultiplexers to address the rows. With this arrangement I use 8 + 5 pins to scan the keybed.


## TODO / Bugs


This is a bit of a hack but it works quite well for now.

If you have any insights to these questions please let me know!

* Is a scanning frequency of 1000Hz too low? I've heard somewhere commercial keyboard use 2000Hz. Perhaps someone can measure one with a scope?
* Right now the velocity response is linear and quite low resolution. What is a good velocity response curve?
* Is it a bad idea to join the two matrices? Perhaps it's a better idea to use a separate micro controller for each matrix and let them talk together using i2c? This approach would be more modular and allow for stacking a lot of keys (in an organ i.e..)
* I'm using the teensy USB MIDI library. I'm not sure how this interferes with the timing of the scanning interrupt routine.



## Resources


* [Keyboard matrix schematic](http://www.doepfer.de/DIY/Matrix_88.gif)

* [Fatar](http://www.fatar.com/) (Keyboard manufacturer)

* [Teensy](http://www.pjrc.com/teensy/) (Arduino clone)

* [74ls138 datasheet](http://ecee.colorado.edu/~mcclurel/sn74ls138rev5.pdf)