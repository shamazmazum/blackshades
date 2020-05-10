/*
 * Fog.cpp
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
/**> HEADER FILES <**/
#include "Fog.h"
void Fog::SetFog(float colorR, float colorG, float colorB, float fStart,
                 float fEnd, float Density)
{
  GLfloat newfogColor[4];
  newfogColor[0] = colorR;
  newfogColor[1] = colorG;
  newfogColor[2] = colorB;
  newfogColor[3] = 1;
  fogStart = fStart;
  fogEnd = fEnd;
  fogDensity = Density;
  fogMode = GL_LINEAR;

  glFogi(GL_FOG_MODE, fogMode);
  glFogfv(GL_FOG_COLOR, newfogColor);
  glFogf(GL_FOG_DENSITY, fogDensity);
  glFogi(GL_FOG_HINT, GL_DONT_CARE);
  glFogf(GL_FOG_START, fogStart);
  glFogf(GL_FOG_END, fogEnd);

  glEnable(GL_FOG);
}

void Fog::SetFog(Color newColor, float fStart, float fEnd, float Density)
{
  GLfloat newfogColor[4];
  newfogColor[0] = newColor.r;
  newfogColor[1] = newColor.g;
  newfogColor[2] = newColor.b;
  newfogColor[3] = 1;
  fogStart = fStart;
  fogEnd = fEnd;
  fogDensity = Density;
  fogMode = GL_LINEAR;

  glFogi(GL_FOG_MODE, fogMode);
  glFogfv(GL_FOG_COLOR, newfogColor);
  glFogf(GL_FOG_DENSITY, fogDensity);
  glFogi(GL_FOG_HINT, GL_DONT_CARE);
  glFogf(GL_FOG_START, fogStart);
  glFogf(GL_FOG_END, fogEnd);

  glEnable(GL_FOG);
}

void Fog::TempFog(float colorR, float colorG, float colorB)
{
  GLfloat tempfogColor[4];
  tempfogColor[0] = colorR;
  tempfogColor[1] = colorG;
  tempfogColor[2] = colorB;
  tempfogColor[3] = 1;

  glFogi(GL_FOG_MODE, fogMode);
  glFogfv(GL_FOG_COLOR, tempfogColor);
  glFogf(GL_FOG_DENSITY, fogDensity);
  glFogi(GL_FOG_HINT, GL_DONT_CARE);
  glFogf(GL_FOG_START, fogStart);
  glFogf(GL_FOG_END, fogEnd);

  glEnable(GL_FOG);
}

void Fog::ResetFog()
{
  glFogi(GL_FOG_MODE, fogMode);
  glFogfv(GL_FOG_COLOR, fogColor);
  glFogf(GL_FOG_DENSITY, fogDensity);
  glFogi(GL_FOG_HINT, GL_DONT_CARE);
  glFogf(GL_FOG_START, fogStart);
  glFogf(GL_FOG_END, fogEnd);

  glEnable(GL_FOG);
}
