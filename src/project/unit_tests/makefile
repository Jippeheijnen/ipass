 #############################################################################
#
# Project Makefile
#
# (c) Jippe Heijnen 2019
#
# This file is in the public domain.
#
#############################################################################

# source files in this project (main.cpp is automatically assumed)
SOURCES := spi.cpp matrixLib.cpp

# header files in this project
HEADERS := spi.hpp matrixLib.hpp

# other places to look for files for this project
SEARCH  := ../../lib
TARGET  := native

WSL := $(cat /proc/version | grep "Microsoft")
SERIAL_PORT ?= /dev/ttyACM0

ifeq ($(WSL), 1)
	DUE_BOOTMODE := cmd.exe /c @mode com5:1200
	SERIAL_PORT := /dev/ttyS5
else
	DUE_BOOTMODE := sudo stty -F $(SERIAL_PORT) speed 1200 cs8 -cstopb -parenb
endif

STM32_CLI := "/mnt/c/Program Files (x86)/STMicroelectronics/STM32 ST-LINK Utility/ST-LINK Utility/ST-LINK_CLI.exe"

RUN_PAUSE := echo "Run Complete"


# set RELATIVE to the next higher directory
# and defer to the appropriate Makefile.* there
RELATIVE := ..

HWLIB             ?= $(HOME)/hwlib
include $(HWLIB)/makefile.inc

BMPTK             ?= $(HOME)/bmptk
include $(BMPTK)/Makefile.inc

debugtarget:
	$(info    $(DEFINES))
