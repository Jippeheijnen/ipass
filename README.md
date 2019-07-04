# ipass

C++ IPASS PROJECT 2019
============================

This is the github page for the library and implementation I built this year as final assignment.

Dependencies
-----
For abstraction of communication with microcontrollers, this library uses [hwlib](http://github.com/wovo/hwlib) for it's hardware specific pins and wait functions. The included makefile is built for [BMPTK](http://github.com/wovo/bmptk).

Installation
-----
- Download the library `cd ~; git clone http://github.com/jippeheijnen/ipass`
- Include Makefile.inc from your project
- Make sure that both [hwlib](http://github.com/wovo/hwlib) and [BMPTK](http://github.com/wovo/bmptk) are installed in the home 
  directory.
  
 
Building the implementation
----
- When building the project's implementation, you should be able to just type
  `./runipass hourglass`
  - Note that hwlib and bmptk are actually required for this to work.
  - The run script will try to build and upload the project automatically.
  
