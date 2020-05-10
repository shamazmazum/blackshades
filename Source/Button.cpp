/*
 * Button.cpp 
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

#include "Button.h"

Button::Button()
{
  xpos = ypos = xwidth = yheight = 0;
}

Button::Button(float xpos, float ypos, float xwidth, float yheight)
{
  setButton(xpos, ypos, xwidth, yheight);
}

void Button::setButton(float _xpos, float _ypos, float _xwidth, float _yheight)
{
  xpos = _xpos;
  ypos = _ypos;
  xwidth = _xwidth;
  yheight = _yheight;
}

bool Button::isMouseOver(bool recalc, float _xpos, float _ypos)
{
  if(!recalc) {
    return mouseover;
  }

  if(_xpos > xpos && _ypos > ypos &&
     _xpos < xwidth+xpos && _ypos < yheight+ypos) {
    mouseover = true;
  } else {
    mouseover = false;
  }
  return mouseover;
}

