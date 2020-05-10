/*
 * Colors.h
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
#ifndef _COLOR_H_
#define _COLOR_H_

#include <GL/gl.h>

class Color {
public:
  Color() {
    r = g = b = a = 0;
  };
  Color(GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a = 1) {
    setColor(_r, _g, _b, _a);
  };

  void setColor(const Color & _color) {
    r = _color.r;
    g = _color.g;
    b = _color.b;
    a = _color.a;
  }

  void setColor(GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a = 1) {
    r = _r;
    g = _g;
    b = _b;
    a = _a;
  };

  GLfloat r;                    //red
  GLfloat g;                    //green
  GLfloat b;                    //blue
  GLfloat a;                    //alpha
};

#endif
