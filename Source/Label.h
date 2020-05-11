/*
 * Label.h
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
#ifndef _GUI_LABEL_H_
#define _GUI_LABEL_H_

#include "Quaternions.h"
#include <GL/gl.h>

#include <string>

#include "Window.h"
#include "Colors.h"

class Label:public Window {
public:
  Label();
  ~Label() {
    text.clear();
  };
  void setLabel(float xpos, float ypos, float size);
  void setFont(int set, GLuint _base, GLuint _texture);
  void setText(const char *text);
  void setColor(Color _color);
  void draw();

private:
  GLfloat xpos, ypos;
  float size;
  string text;
  Color color;

  //Font stuff
  int fontSet;
  GLuint texture;
  GLuint base;
};

#endif
