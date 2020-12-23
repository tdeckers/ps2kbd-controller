# PIC based keyboard controller for use with 6051

## Overview

For the 6502 computer project I needed a keyboard interface and didn't want to
burden the processor with all the heavy lifting.  I have a few PIC devices 
laying around that I never got a chance to use.

This is the first real PIC project, so code might be crappy, but as of the 
first checked in version it seems to be working.

Inspiration for the code came from <http://sbc.rictor.org/pckbavr.html>.  That
code was written in assembler. I'm not that brave. I ported it to C.

This project has been developed and tested with a PIC16F1826.

## Features

* Receive bytes from keyboard on key press
* Send commands to keyboard (e.g. control LEDs)

Current version does the night rider effect on the LEDs.

Next up: 
* actual 6502 interfacing
* clean up variables, etc..


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