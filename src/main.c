/*============================================================================

  pico7219_cli

  main.c

  Copyright (c)2021 Kevin Boone, GPL v.30

============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "defs.h"
#include "config.h"
#include "pico7219usb.h"
#include "protocol.h"

/*============================================================================

  my_strerror 

============================================================================*/
const char *my_strerror (Pico7219Err errcode)
  {
  switch (errcode)
    {
    case PICO7219ERR_UNKNOWN:
      return "Unknown response from device";

    case PICO7219ERR_OPEN_DEV:
      return "Can't open serial device";

    case PICO7219ERR_CONFIG_DEV:
      return "Can't configure serial device";

    case PICO7219ERR_TOOLONG:
      return "String too long";

    default:
      return "unknown";
    }
  }

/*============================================================================

  show_version 

============================================================================*/
void show_version (void)
  {
  printf (NAME " version " VERSION "\n");
  }

/*============================================================================

  show_help

============================================================================*/
void show_help (void)
  {
  printf ("Usage: " NAME " {options}\n");
  printf ("   -a,--on {row,col}      Turn on the LED\n");
  printf ("   -b,--off {row,col}     Turn off the LED\n");
  printf ("   -c,--scroll-off        Stop scrolling\n");
  printf ("   -d,--dev {device}      USB serial device\n");
  printf ("   -f,--flush             Flush changes to display\n");
  printf ("   -i,--intensity {0..15} Flush changes to display\n");
  printf ("   -r,--reset             Reset display\n");
  printf ("   -s,--scroll-on         Start scrolling\n");
  printf ("   -t,--text \"string\"   Display text string\n");
  printf ("   -v,--version           Show version\n");

  }

/*============================================================================

  main

============================================================================*/
int main (int argc, char **argv)
  {
  char *dev = strdup (DEFAULT_DEV);
  char *text = NULL;

  BOOL version = FALSE;
  BOOL help = FALSE;
  BOOL reset = FALSE;
  BOOL scroll_on = FALSE;
  BOOL scroll_off = FALSE;
  BOOL carry_on = TRUE;
  BOOL turn_on = FALSE;
  BOOL turn_off = FALSE;
  BOOL intensity = FALSE;
  BOOL flush = FALSE;
  BOOL did_something = FALSE;
  int r = 0, c = 0;

  static struct option long_options[] =
    {
      {"on", required_argument, NULL, 'a'},
      {"off", required_argument, NULL, 'b'},
      {"dev", required_argument, NULL, 'd'},
      {"flush", no_argument, NULL, 'f'},
      {"help", no_argument, NULL, 'h'},
      {"version", no_argument, NULL, 'v'},
      {"reset", no_argument, NULL, 'r'},
      {"text", required_argument, NULL, 't'},
      {"intensity", required_argument, NULL, 'i'},
      {"scroll-on", no_argument, NULL, 's'},
      {"scroll-off", no_argument, NULL, 'c'},
      {0, 0, 0, 0}
    };

  int opt;
  while (carry_on)
    {
    int option_index = 0;
    opt = getopt_long (argc, argv, "a:b:cd:fhi:rst:v", long_options, &option_index);

    if (opt == -1) break;

    switch (opt)
      {
      case 0:
        break;
      case 'a':
	sscanf (optarg, "%d,%d", &r, &c);
	turn_on = TRUE;
        break;
      case 'b':
	sscanf (optarg, "%d,%d", &r, &c);
	turn_off = TRUE;
        break;
      case 'c':
        scroll_off = TRUE; 
        break;
      case 'd':
        free (dev);
        dev = strdup (optarg);
        break;
      case 'f':
        flush = TRUE; 
        break;
      case 'h':
        help = TRUE;
        break;
      case 'i':
        intensity = TRUE;
        r = atoi (optarg);
        break;
      case 'r':
        reset = TRUE; 
        break;
      case 's':
        scroll_on = TRUE; 
        break;
      case 't':
        text = strdup (optarg);
        break;
      case 'v':
        version = TRUE;
        break;
      default:
        carry_on = TRUE;
      }
    }

  if (carry_on)
    {
    if (version)
      {
      show_version();
      carry_on = FALSE;
      }
    else if (help)
      {
      show_help();
      carry_on = FALSE;
      }
    }

  if (carry_on)
    {
    Pico7219USB* p7u = pico7219usb_new (dev);  

    if (reset) // Reset display
      {
      Pico7219Err err = pico7219usb_init (p7u);
      if (err == 0)
        {
        err = pico7219usb_reset (p7u);
        if (err)
          fprintf (stderr, "%s: %s\n", argv[0], my_strerror (err));
        }
      else
        fprintf (stderr, "%s: %s\n", argv[0], my_strerror (err));
      did_something = TRUE;
      }
    else if (text) // display string
      {
      Pico7219Err err = pico7219usb_init (p7u);
      if (err == 0)
        {
        err = pico7219usb_display_text (p7u, text);
        if (err)
          fprintf (stderr, "%s: %s\n", argv[0], my_strerror (err));
        }
      else
        fprintf (stderr, "%s: %s\n", argv[0], my_strerror (err));
      did_something = TRUE;
      }
    else if (flush)
      {
      Pico7219Err err = pico7219usb_init (p7u);
      if (err == 0)
        {
        err = pico7219usb_flush (p7u);
        if (err)
          fprintf (stderr, "%s: %s\n", argv[0], my_strerror (err));
        }
      did_something = TRUE;
      }
    else if (scroll_on || scroll_off)
      {
      Pico7219Err err = pico7219usb_init (p7u);
      if (err == 0)
        {
	if (scroll_on)
          err = pico7219usb_scroll_on (p7u);
	else
          err = pico7219usb_scroll_off (p7u);
        if (err)
          fprintf (stderr, "%s: %s\n", argv[0], my_strerror (err));
        }
      did_something = TRUE;
      }
    else if (turn_on || turn_off)
      {
      Pico7219Err err = pico7219usb_init (p7u);
      if (err == 0)
        {
	if (turn_on)
          err = pico7219usb_on (p7u, r, c);
	else
          err = pico7219usb_off (p7u, r, c);
        if (err)
          fprintf (stderr, "%s: %s\n", argv[0], my_strerror (err));
        }
      else
        fprintf (stderr, "%s: %s\n", argv[0], my_strerror (err));
      did_something = TRUE;
      }
    else if (intensity)
      {
      if (intensity >= PICO7219_MIN_BRIGHTNESS 
            && intensity <= PICO7219_MAX_BRIGHTNESS)
        {
        Pico7219Err err = pico7219usb_init (p7u);
        if (err == 0)
          {
          err = pico7219usb_brightness (p7u, r);
          if (err)
            fprintf (stderr, "%s: %s\n", argv[0], my_strerror (err));
          }
        else
          fprintf (stderr, "%s: %s\n", argv[0], my_strerror (err));
	}
      else
        fprintf (stderr, "%s: brightness out of range (0-15)", argv[0]); 
        
      did_something = TRUE;
      }

    pico7219usb_destroy (p7u);
    }

  if (!did_something)
    {
    fprintf (stderr, "%s: no action specified\n", argv[0]);
    }

  free (dev);
  if (text) free (text);
  }

