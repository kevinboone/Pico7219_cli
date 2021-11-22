/*===========================================================================

  pico7219_cli

  pico2719usb.h 

  Copyright (c)2021 Kevin Boone, GPL v.30

===========================================================================*/

#pragma once

#include "defs.h"

struct _Pico7219USBPriv;

typedef struct _Pico7219USB
  {
  struct _Pico7219USBPriv *priv;
  } Pico7219USB;

/** Error codes */

typedef enum _Pico7219Err
  {
  PICO7219ERR_OK = 0,
  PICO7219ERR_UNKNOWN = 1,
  PICO7219ERR_OPEN_DEV = 2,
  PICO7219ERR_CONFIG_DEV = 3,
  PICO7219ERR_INTERNAL = 4,
  PICO7219ERR_TOOLONG = 5
  } Pico7219Err;

BEGIN_DECLS

/** Create a new Pico7219USB object, specifying the serial device. 
    This method always succeeds -- no initialization is done at this
    stage. */
Pico7219USB *pico7219usb_new (const char *dev);

/** Clean up and close resources. */
void         pico7219usb_destroy (Pico7219USB *self);

/** Initialize the object. This will attempt to open and set the
    operating parameters of the serial device. Returns an error code
    in the event of a failure. */
Pico7219Err pico7219usb_init (Pico7219USB *self);

/** Close the serial device. The object can be reused by
    calling _init again. */
void        pico7219usb_deinit (Pico7219USB *self);

/** Send a string to the serial device, and wait for a response. 
    Returns an error code. */
Pico7219Err pico7219usb_send_and_receive (Pico7219USB *self, 
      const char *string);

/** Send the specified text to the display. */
Pico7219Err pico7219usb_display_text (Pico7219USB *self, const char *text);

/** Reset the display hardware -- turn off all LEDs, disable scrolling,
    set brightness to default, etc. */
Pico7219Err pico7219usb_reset (Pico7219USB *self);

/** Turn on scrolling in the display device. */
Pico7219Err pico7219usb_scroll_on (Pico7219USB *self);

/** Turn off scrolling in the display device. */
Pico7219Err pico7219usb_scroll_off (Pico7219USB *self);

Pico7219Err pico7219usb_on (Pico7219USB *self, int row, int col);

Pico7219Err pico7219usb_off (Pico7219USB *self, int row, int col);

Pico7219Err pico7219usb_flush (Pico7219USB *self);

Pico7219Err pico7219usb_brightness (Pico7219USB *self, int b);

END_DECLS

