/*
 * Image.cpp
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
#include "Image.h"
#include "Config.h"

extern Config config;

Image::Image()
{
  index = 0;
  color = Color(1.0, 1.0, 1.0, 1.0);
}

void Image::setDim(GLfloat _xpos, GLfloat _ypos, GLfloat _xwidth,
                   GLfloat _yheight)
{
  xpos = _xpos;
  ypos = _ypos;
  xwidth = _xwidth;
  yheight = _yheight;
}

void Image::setImage(GLuint _texture, unsigned int index)
{
  if(index < NUM_TEXTURES) {
    texture[index] = _texture;
  }
}

void Image::setActive(unsigned int _index)
{
  if(_index < NUM_TEXTURES) {
    index = _index;
  }
}

void Image::setColor(Color _color)
{
  color = _color;
}

void Image::update(float _sinefluct, float _sinefluctprog)
{
  sinefluct = _sinefluct;
  sinefluctprog = _sinefluctprog;
}

void Image::draw()              // Where The Printing Happens
{
  float width = config.screenwidth;
  float height = config.screenheight;

  GLint x = (int)(xpos * width);
  GLint y = (int)(ypos * height);
  GLint xw = (int)(xwidth * width);
  GLint yh = (int)(yheight * height);

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_DEPTH_TEST);     // Disables Depth Testing
  glDisable(GL_CULL_FACE);
  glDisable(GL_LIGHTING);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE);

  glDepthMask(0);

  glMatrixMode(GL_PROJECTION);  // Select The Projection Matrix

  glPushMatrix();               // Store The Projection Matrix

  glLoadIdentity();             // Reset The Projection Matrix

  glOrtho(0, width, 0, height, -100, 100);      // Set Up An Ortho Screen

  glMatrixMode(GL_MODELVIEW);   // Select The Modelview Matrix

  //Draw smoke
  glPushMatrix();               // Store The Modelview Matrix
  glLoadIdentity();             // Reset The Modelview Matrix

  glTranslatef(x + sinefluct * 40, y - sinefluct * 9, 0);
  //glTranslatef(x - sinefluct * 40, y + sinefluct * 9, 0);
  glScalef(xw - sinefluct * 80, yh + sinefluct * 18, 1);
  //glScalef(xw + sinefluct * 80, yh - sinefluct * 18, 1);

  glTranslatef(.5, .5, 0);
  glScalef(.5, .5, 1);

  glRotatef(sinefluctprog * 50, 0, 0, 1);

  glEnable(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, texture[index]);

  glEnable(GL_BLEND);

  glColor4f(color.r, color.g, color.b, .4 + sinefluct / 8);
  //flame images (for "New" and "Quit")
  //glColor4f(color.r, color.g, color.b, .5 +/- sinefluct / 6);

  glBegin(GL_TRIANGLES);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(1, 1, 0.0f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-1, 1, 0.0f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(1, -1, 0.0f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-1, -1, 0.0f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(1, -1, 0.0f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-1, 1, 0.0f);
  glEnd();

  glMatrixMode(GL_MODELVIEW);   // Select The Modelview Matrix

  glPopMatrix();

  glMatrixMode(GL_PROJECTION);  // Select The Projection Matrix

  glPopMatrix();                // Restore The Old Projection Matrix

  glDepthMask(1);
}
