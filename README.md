# ipass

C++ IPASS PROJECT 2019
============================

This is the github page for the library and implementation I built this year as final assignment.

Dependencies
-----
For abstraction of communication with microcontrollers, this library uses [hwlib](http://github.com/wovo/hwlib) for it's hardware specific pins and wait functions.

The included makefile is built for [BMPTK](http://github.com/wovo/bmptk).

Installation
-----
- Download the library `git clone http://github.com/jippeheijnen/ipass`
- Include Makefile.inc from your project

Building with BMPTK
----
- Make sure to set the TARGET before including *Makefile.inc*
- Include *Makefile.inc*


Building without BMPTK
----
The included makefile only sets some variables, but doesn't do any compiling itself. When building without BMPTK, some variables should be interpreted properly after including.
- HEADERS: all .hpp header files
- SOURCES: all .cpp source files
- SEARCH: theinclude path for header files of this library 


