/*
 * Decals.h
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
#ifndef _DECALS_H_
#define _DECALS_H_

#include "Quaternions.h"
///ZWARNING
///#include <GL/glut.h>
///GLUT REMOVED.
#include "Files.h"
#include "TGALoader.h"
#include "Quaternions.h"
#include "Camera.h"
#include "Models.h"
#include "Fog.h"
//
// Model Structures
//

#define maxdecals 120

class Decals {
public:
  GLuint bulletholetextureptr;
  GLuint cratertextureptr;
  GLuint bloodtextureptr[11];

  int howmanydecals;

  int type[maxdecals];

  XYZ points[8 * maxdecals];
  int numpoints[maxdecals];
  float texcoordsx[8 * maxdecals];
  float texcoordsy[8 * maxdecals];
  float alivetime[maxdecals];

  void draw();

  int DeleteDecal(int which);
  int MakeDecal(int atype, XYZ location, float size, XYZ normal, int poly,
                Model * model, XYZ move, float rotation);

  void DoStuff();
  void LoadBulletHoleTexture(const char *fileName);
  void LoadCraterTexture(const char *fileName);
  void LoadBloodTexture(const char *fileName, int which);

   ~Decals() {
    glDeleteTextures(1, (const GLuint *)&bulletholetextureptr);
    glDeleteTextures(1, (const GLuint *)&cratertextureptr);
    glDeleteTextures(11, (const GLuint *)&bloodtextureptr);
  };
};

#endif
