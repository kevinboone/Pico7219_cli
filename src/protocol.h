/*=========================================================================
 
  Pico7219

  protocol.h 

  All commands start with a letter, followed by some data. Nothing is
  echo'd back to the sender, so all the sender needs to look for is
  a response. Command input is terminated by a line feed (character 10),
  but any additional CR (13) is skipped. Responses are terminated 
  by a LF, without CR.

  The response consists of a numeric code, then a single text token
  which is string version of the numeric code, then perhaps some
  additional data, depending on the error. A success code is
  "0 OK <LF>"

  =========================================================================*/
#pragma once

// ON -- Acol,row
// Turn on an LED
// Row, col are zero indexed, from the _bottom left_ corner. 
// It isn't an error to set values that are outside the range of the display,
// but they are silently ignored.  Values that are outside the range of the
// physical display, but in the range of the virtual display, could later be
// scrolled into view
#define CMD_ON       'A'

// OFF -- Bcol,row
// Turn off an LED
#define CMD_OFF      'B'

// CHAR -- Cc 
// Adds a character to the display buffer
#define CMD_CHAR     'C'

// STRING -- Dstring
// Replaces the line buffer with 'string', and flushes to hardware
#define CMD_STRING   'D'

// FLUSH -- F
// Flush changes to the physical hardware
#define CMD_FLUSH    'F'

// SCROLL_ON -- G
#define CMD_SCROLLON 'G'

// SCROLL_OFF -- H
#define CMD_SCROLLOFF 'H'

// BRIGHTNESS -- In
// 'n' is 0-15
#define CMD_BRIGHTNESS 'I'

// RESET -- R
// Clear pixels, clear the line buffer, flush, stop scrolling,
//   set brightness to 1
#define CMD_RESET    'R'

// SCROLL -- S 
// Scroll one pixel left. If scrolling is already active, this
//   won't be visible. Implicitly flush updates to the hardware.
#define CMD_SCROLL   'S'

// 
// Error codes
//
#define ERR_NONE     0

// Command input to short (no command character)
#define ERR_TOOSHORT 1

// Too many or to ofew arguments
#define ERR_ARGS     2

// Unknown command character
#define ERR_BADCMD   3

// Command input too long (e.g., too many characters in string)
#define ERR_TOOLONG  4

// Display brightness in the range 0-15
#define PICO7219_MAX_BRIGHTNESS 15
#define PICO7219_MIN_BRIGHTNESS 0


