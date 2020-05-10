/*
 * MacInput.cpp
 * Copyright (C) 2007 by Bryan Duff <duff0097@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */
#include "MacInput.h"

//keys
int forwardskey;
int backwardskey;
int leftkey;
int rightkey;
int aimkey;
int psychicaimkey;
int psychickey;
int helpkey;

Boolean IsKeyDown(unsigned char *keyMap, unsigned short theKey)
{
  long keyMapIndex;
  Boolean isKeyDown;
  short bitToCheck;

  // Calculate the key map index
  keyMapIndex = keyMap[theKey / 8];

  // Calculate the individual bit to check
  bitToCheck = theKey % 8;

  // Check the status of the key
  isKeyDown = (keyMapIndex >> bitToCheck) & 0x01;

  // Return the status of the key
  return isKeyDown;
}

