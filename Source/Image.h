/*
 * Image.h
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
#ifndef _GUI_IMAGE_H_
#define _GUI_IMAGE_H_

#include "Quaternions.h"
#include <GL/gl.h>
#include <GL/glu.h>

#include <string>

#include "Window.h"
#include "Colors.h"

#define NUM_TEXTURES 2

class Image:public Window {
public:
  Image();
  ~Image() {
  } void setDim(float _xpos, float _ypos, float _xwidth, float _yheight);
  void setImage(GLuint _texture, unsigned int index);
  void setActive(unsigned int index);
  void setColor(Color _color);
  void update(float _sinefluct, float _sinefluctprog);
#if 0
  void glScale(float x, float y);
  void glTranslate(float x, float y);
  void glRotate(float deg, float x, float y, float z);
  void glColor(float r, float g, float b, float a);
#endif
  void draw();

private:
  GLfloat xpos, ypos, xwidth, yheight;
  unsigned int index;
  GLuint texture[NUM_TEXTURES];
  float sinefluct, sinefluctprog;
  Color color;
};

#endif
