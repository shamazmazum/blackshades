/*
 * Label.cpp
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
#include "Label.h"
#include "Config.h"

extern Config config;

Label::Label()
{
  xpos = ypos = size = 0;
  color = Color(0.0, 0.0, 0.0, 1.0);    //black
}

void Label::setLabel(GLfloat _xpos, GLfloat _ypos, float _size)
{
  xpos = _xpos;
  ypos = _ypos;
  size = _size;
}

void Label::setFont(int set, GLuint _base, GLuint _texture)
{
  if(set > 1) {
    set = 1;
  }

  fontSet = set;
  base = _base;
  texture = _texture;
}

void Label::setText(const char *string)
{
  text = string;
}

void Label::setColor(Color _color)
{
  color = _color;
}

//FIXME: base, FontTexture
void Label::draw()              // Where The Printing Happens
{
  Window::draw();               //draw children

  float width = config.screenwidth;
  float height = config.screenheight;

  GLint x = (int)(xpos * width);
  GLint y = (int)(ypos * height);

  glEnable(GL_TEXTURE_2D);
  glColor4f(color.r, color.g, color.b, color.a);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D, texture);        // Select Our Font Texture
  glDisable(GL_DEPTH_TEST);     // Disables Depth Testing
  glDisable(GL_LIGHTING);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glMatrixMode(GL_PROJECTION);  // Select The Projection Matrix
  glPushMatrix();               // Store The Projection Matrix
  glLoadIdentity();             // Reset The Projection Matrix
  glOrtho(0, width, 0, height, -100, 100);      // Set Up An Ortho Screen
  glMatrixMode(GL_MODELVIEW);   // Select The Modelview Matrix
  glPushMatrix();               // Store The Modelview Matrix
  glLoadIdentity();
  glScalef(size, size, 1);      // Reset The Modelview Matrix
  glTranslated(x, y, 0);        // Position The Text (0,0 - Bottom Left)
  glListBase(base - 32 + (128 * fontSet));      // Choose The Font Set (0 or 1)
  glCallLists(text.length(), GL_BYTE, text.c_str());    // Write The Text To The Screen
  glMatrixMode(GL_PROJECTION);  // Select The Projection Matrix
  glPopMatrix();                // Restore The Old Projection Matrix
  glMatrixMode(GL_MODELVIEW);   // Select The Modelview Matrix
  glPopMatrix();                // Restore The Old Projection Matrix
  glEnable(GL_DEPTH_TEST);      // Enables Depth Testing
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}
