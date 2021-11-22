/*============================================================================

  pico7219_cli

  pico2719usb.c

  Copyright (c)2021 Kevin Boone, GPL v.30

============================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include "pico7219usb.h"
#include "protocol.h"

typedef struct _Pico7219USBPriv 
  {
  char *dev;
  int fd;
  } Pico7219USBPriv;

/*============================================================================

  pico7219usb_new

============================================================================*/
Pico7219USB *pico7219usb_new (const char *dev)
  {
  Pico7219USB *self = malloc (sizeof (Pico7219USB));

  Pico7219USBPriv *priv = malloc (sizeof (Pico7219USBPriv));

  self->priv = priv;
  priv->fd = -1;
  self->priv->dev = strdup (dev);

  return self;
  }

/*============================================================================

  pico7219usb_destroy

============================================================================*/
void pico7219usb_destroy (Pico7219USB *self)
  {
  if (self)
    {
    pico7219usb_deinit (self);
    if (self->priv)
      {
      if (self->priv->dev) free (self->priv->dev);
      free (self->priv);
      }
    free (self);
    }
  }

/*============================================================================

  pico7219usb_init

============================================================================*/
Pico7219Err pico7219usb_init (Pico7219USB *self)
  {
  int speed = 115200; // TODO
  int parity = 0;
  Pico7219Err ret = 0;
  self->priv->fd = open (self->priv->dev, O_RDWR);
  if (self->priv->fd >= 0)
    {
    struct termios tty;
    if (tcgetattr (self->priv->fd, &tty) == 0)
      {
      cfsetospeed (&tty, speed);
      cfsetispeed (&tty, speed);

      tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
      tty.c_iflag &= ~IGNBRK;         // Disable break processing
      tty.c_lflag = 0;                // No signal chars, no echo,
                                      //   no canonical processing
      tty.c_oflag = 0;                // No remapping, no delays
      tty.c_cc[VMIN]  = 1;            // Read blocks
      tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

      tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Ignore xon/xoff

      tty.c_cflag |= (CLOCAL | CREAD);// Ignore modem controls,
      tty.c_cflag &= ~(PARENB | PARODD);  // Ignore parity 
      tty.c_cflag |= parity;
      tty.c_cflag &= ~CSTOPB;
      tty.c_cflag &= ~CRTSCTS;

      tcsetattr (self->priv->fd, TCSANOW, &tty);
      }
    else
      {
      ret = PICO7219ERR_CONFIG_DEV;
      }
    }
  else
    {
    ret = PICO7219ERR_OPEN_DEV;
    }
  return ret;
  }

/*============================================================================

  pico7219usb_deinit

============================================================================*/
void pico7219usb_deinit (Pico7219USB *self)
  {
  if (self->priv)
    {
    if (self->priv->fd >= 0)
      {
      close (self->priv->fd);
      self->priv->fd = -1;
      }
    }
  }


/*============================================================================

  pico7219usb_send_and_receive

============================================================================*/
Pico7219Err pico7219usb_send_and_receive (Pico7219USB *self, 
      const char *string)
  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result" 
  write (self->priv->fd, string, strlen (string));
  write (self->priv->fd, "\n", 1);
#pragma GCC diagnostic pop 
  char buff [30]; // Should be enough -- responses are short
  char c = 0;
  int n;
  int l = 0;
  do
    {
    n = read (self->priv->fd, &c, 1);
    if (n == 1 && c != 10 && l < sizeof (buff) - 1)
      {
      buff[l] = c;
      buff[l + 1] = 0;
      l++;
      }
    } while (n > 0 && c != 10);
  int p7err = 0;
  sscanf (buff, "%d", &p7err);
  switch (p7err)
    {
    case 0: return 0;
    case 1: case 2: case 3: 
      return PICO7219ERR_INTERNAL;
    case 4: 
      return PICO7219ERR_TOOLONG;
    default:
      return PICO7219ERR_UNKNOWN;
    }
  }

/*============================================================================

  pico7219usb_display_text

============================================================================*/
Pico7219Err pico7219usb_display_text (Pico7219USB *self, const char *text)
  {
  int l = strlen (text);
  char *cmd = malloc (l + 2);
  cmd[0] = CMD_STRING;
  cmd[1] = 0;
  strcat (cmd, text);
  Pico7219Err ret = pico7219usb_send_and_receive (self, cmd);
  free (cmd);
  return ret;
  }

/*============================================================================

  pico7219usb_reset

============================================================================*/
Pico7219Err pico7219usb_reset (Pico7219USB *self)
  {
  char buff[2];
  buff[0] = CMD_RESET;
  buff[1] = 0;
  Pico7219Err ret = pico7219usb_send_and_receive (self, buff);
  return ret;
  }

/*============================================================================

  pico7219usb_flush

============================================================================*/
Pico7219Err pico7219usb_flush (Pico7219USB *self)
  {
  char buff[2];
  buff[0] = CMD_FLUSH;
  buff[1] = 0;
  Pico7219Err ret = pico7219usb_send_and_receive (self, buff);
  return ret;
  }

/*============================================================================

  pico7219usb_scroll_on

============================================================================*/
Pico7219Err pico7219usb_scroll_on (Pico7219USB *self)
  {
  char buff[2];
  buff[0] = CMD_SCROLLON;
  buff[1] = 0;
  Pico7219Err ret = pico7219usb_send_and_receive (self, buff);
  return ret;
  }

/*============================================================================

  pico7219usb_scroll_off

============================================================================*/
Pico7219Err pico7219usb_scroll_off (Pico7219USB *self)
  {
  char buff[2];
  buff[0] = CMD_SCROLLOFF;
  buff[1] = 0;
  Pico7219Err ret = pico7219usb_send_and_receive (self, buff);
  return ret;
  }

/*============================================================================

  pico7219usb_brightness

============================================================================*/
Pico7219Err pico7219usb_brightness (Pico7219USB *self, int b)
  {
  char buff[20];
  if (b < PICO7219_MIN_BRIGHTNESS) b = PICO7219_MIN_BRIGHTNESS;
  if (b > PICO7219_MAX_BRIGHTNESS) b = PICO7219_MAX_BRIGHTNESS;
  sprintf (buff, "%c%d", CMD_BRIGHTNESS, b);
  Pico7219Err ret = pico7219usb_send_and_receive (self, buff);
  return ret;
  }

/*============================================================================

  pico7219usb_on

============================================================================*/
Pico7219Err pico7219usb_on (Pico7219USB *self, int row, int col)
  {
  char buff[20];
  sprintf (buff, "%c%d,%d", CMD_ON, row, col);
  Pico7219Err ret = pico7219usb_send_and_receive (self, buff);
  return ret;
  }

/*============================================================================

  pico7219usb_off

============================================================================*/
Pico7219Err pico7219usb_off (Pico7219USB *self, int row, int col)
  {
  char buff[20];
  sprintf (buff, "%c%d,%d", CMD_OFF, row, col);
  Pico7219Err ret = pico7219usb_send_and_receive (self, buff);
  return ret;
  }


