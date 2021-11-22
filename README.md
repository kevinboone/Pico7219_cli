# pico7219\_cli

Version 0.1a

## What is this?

`pico7219_cli` is a Linux command-line client for the 
[Pico7219usb](https://github.com/kevinboone/Pico7219usb) 
USB-connected LED matrix controller. The LED matrix consists of
a number of 8x8 LED modules controlled by a Raspberry Pi Pico.
Although `Pico7219usb` can be controlled
using relatively simple command-line operations, using this
self-contained client might be a bit more elegant. However, it doesn't
do anything that can't, in principle, be accomplished by sending
strings of characters to the display device's USB serial port.

The file `src/pico7219usb.c` and its accompanyting header form a
self-contained library for interacting with the Pico-based display firmware.

## Examples

Reset and clear the display

    $ pico7219-cli --reset

Display some text on the LED module:

    $ pico7219-cli --text "Show this text"

Start the text scrolling:

    $ pico7219-cli --scroll-on

## Building

The usual:

    $ make
    $ sudo make install

## Device selection

This utility assumes that the Pico device appears as `/dev/ttyACM0`.
Use the --dev switch to override this behaviour. Note that, after
plugging in the Pico, you might have to wait until the modem
manager has finished trying (and failing) to initialise the Pico as if it were
a modem (or disable the modem manager).

## Command line options

_a,--on {row,col}_

Turn on the LED at row,col. Coordinates start from 0,0 in the bottom-left
corner. Changes are not shown immediately -- use `--flush` to update
the LED array. The virtual display size is expanded, if necessary, to
accomodate the specified width.
 
_b,--off {row,col}_

Turn off the LED at row,col. Coordinates start from 0,0 in the bottom-left
corner. Changes are not shown immediately -- use `--flush` to update
the LED array. The virtual display size is expanded, if necessary, to
accomodate the specified width.

_-c,--scroll-off_

Turns off display scrolling.

_-r,--reset_

Reset the display -- set brightness to default, stop scrolling, turn
all LEDs off.

_-d,--dev {device}_

Select the serial device. Default is `/dev/ttyACM0`.

_-f,--flush_

Flush changes to the LED matrix that were made using `--on` and `--off`.

_-i,--intensity {0-15}_

Set the display intensity, in the range 0-15. Note that "zero" brightness
is not "off" on MAX7219 devices -- you'll need to clear the display
completely.

_-r,--reset_

Reset the LED matrix and display firmware. Clear all LEDs, set the
virtual size to the physical size, turn off scrolling, brightness to
default (1).

_-s,--scroll-on_

Turns on display scrolling.

_-t,--text "Display this text"_

Display some text. Don't forget to quote the string if it contains
spaces.

_-v,--version_

Show program version.

## Limitations

Please refer to the general 
[Pico7219usb](https://github.com/kevinboone/Pico7219usb) documentation --
this utility is just a simple wrapper for the control protocol, and
will have all the sample limitations that the display firmware has.
Most significantly:

- Don't try to update the display whilst it is scrolling -- reset it first

- The display supports only the 8-bit CP437 character set -- there
is no Unicode or multi-byte character support. See the Pico7219usb
documentation for suggestions how to accomodate this character
set in a Linux environment.

- Text of any length can be displayed (subject to memory), but it's
  hard to read scrolling text of more than a few dozen words.

## Legal, etc

This software is copyright (c)2021 Kevin Boone, distributed under the
terms of the GNU Public Licence, v3.0. There is no warranty of
any kind.


