/*
 * Button.h 
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
#ifndef _GUI_BUTTON_H_
#define _GUI_BUTTON_H_

#include <string>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Quaternions.h"
#include "Window.h"

/* Currently this is only a logical button (it doesn't display anything)
 * it consists only of state information.
 * TODO: DrawButton() would consist of drawing a child text and image window.
 */

class Button : public Window {
public:
  Button();
  Button(float xpos, float ypos, float xwidth, float yheight);
  ~Button() {};
  void setButton(float xpos, float ypos, float xwidth, float yheight);
  bool isMouseOver(bool recalc, float _xpos=0, float _ypos=0);
  void setPressed(bool _pressed) { pressed = _pressed; }
  bool isPressed() { return pressed; }

private:
  GLfloat xpos, ypos;
  GLfloat xwidth, yheight;
  bool mouseover;
  bool pressed;
};

#endif
