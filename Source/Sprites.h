/*
 * Sprites.h
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
#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "Quaternions.h"
#include <GL/gl.h>
#include "Files.h"
#include "TGALoader.h"
#include "Quaternions.h"
#include "Camera.h"
#include "Models.h"
#include "Fog.h"
//
// Model Structures
//

#define maxsprites 2000

#define muzzleflashsprite 1
#define smokesprite 2
#define smokespritenoup 3
#define flashsprite 6
#define grenadesprite 7
#define pinsprite 8
#define spoonsprite 9
#define bloodspritedown 10
#define bloodspritenoup 11
#define particlesspritedown 12
#define snowsprite 13
#define rainsprite 14

#define bullet 4
#define bulletinstant 5

class Sprites {
public:
  GLuint flaretextureptr;
  GLuint muzzleflaretextureptr;
  GLuint smoketextureptr;
  GLuint bullettextureptr;
  GLuint bloodtextureptr;
  GLuint raintextureptr;
  GLuint snowtextureptr;

  int howmanysprites;

  XYZ location[maxsprites];
  XYZ oldlocation[maxsprites];
  XYZ velocity[maxsprites];
  XYZ initialvelocity[maxsprites];
  float size[maxsprites];
  float initialsize[maxsprites];
  float brightness[maxsprites];
  float initialbrightness[maxsprites];
  float color1[maxsprites];
  float color2[maxsprites];
  float color3[maxsprites];
  float alivetime[maxsprites];
  float rotation[maxsprites];
  int type[maxsprites];
  int owner[maxsprites];

  void draw();

  int DeleteSprite(int which);
  int MakeSprite(int atype, float abrightness, float acolor1, float acolor2,
                 float acolor3, XYZ alocation, XYZ avelocity, float asize);
  int MakeSprite(int atype, float abrightness, float acolor1, float acolor2,
                 float acolor3, XYZ alocation, XYZ avelocity, float asize,
                 int aowner);

  void DoStuff();
  void LoadMuzzleFlareTexture(const char *fileName);
  void LoadFlareTexture(const char *fileName);
  void LoadSmokeTexture(const char *fileName);
  void LoadBulletTexture(const char *fileName);
  void LoadBloodTexture(const char *fileName);
  void LoadSnowTexture(const char *fileName);
  void LoadRainTexture(const char *fileName);

   ~Sprites() {
    glDeleteTextures(1, (const GLuint *)&muzzleflaretextureptr);
    glDeleteTextures(1, (const GLuint *)&flaretextureptr);
    glDeleteTextures(1, (const GLuint *)&bullettextureptr);
    glDeleteTextures(1, (const GLuint *)&smoketextureptr);
    glDeleteTextures(1, (const GLuint *)&bloodtextureptr);
    glDeleteTextures(1, (const GLuint *)&raintextureptr);
    glDeleteTextures(1, (const GLuint *)&snowtextureptr);
  };
};

#endif
