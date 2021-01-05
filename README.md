# PIC based keyboard controller for use with 6051

## Overview

For the 6502 computer project I needed a keyboard interface and didn't want to
burden the processor with all the heavy lifting.  I have a few PIC devices 
laying around that I never got a chance to use.

This is the first real PIC project, so code might be crappy, but as of the 
first checked in version it seems to be working.

Inspiration for the code came from <http://sbc.rictor.org/pckbavr.html>.  That
code was written in assembler. I'm not that brave. I ported it to C without too
much optimizations, so that's why the code is a bit clumsy.  I might get around
to optimize it later, but it works for now.

## Hardware setup

The PS/2 keyboard is connected as follows:
* RA0 - keyboard clock
* RA1 - keyboard data
* Connect keyboard VCC and GND.

This controller is intended to be interfaced to the 6502 through a 
6522 VIA.  
* 8 bits of port B are to be connected to the VIA ports data lines.
* RA2 - to VIA CA1 (data ready)
* RA3 - from VIA CA2 (data ack)

When the controller receives a new character from the keyboard, it sets the
data bits on port B. It then asserts the data_ready signal.  The CPU can then
read the character through the VIA and that will automatically assert the 
data_ack signal.

This project has been developed and tested with a PIC16F1826.

## Features

* Receive bytes from keyboard on key press and presents the byte to the VIA.
* Send commands to keyboard (e.g. control LEDs)
* Interfaces with a 6502 through a VIA port.

Future:
* watchdog reset in case the controller gets stuck in any of the loops waiting
  for a given state.

## Development notes

IDE: MPLAB X IDE v5.40

I've been using a couple LEDs on RA6 and RA7 to monitor what's going on.  In
addition, the oscilloscoop was probing RA2 for key events to follow.  I'll 
remove some of that code for the final version.

https://microchipdeveloper.com/c:bits-bools-and-bit-fields

## Programming

I ran into programming issues with the *minipro* application and a TL866II Plus
on Linux. It appears that that combo doesn't work well.
Now using PIC-KIT3 right from the IDE.  Works like a charm.