/*
 * Viewer.h
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

#ifndef _VIEWER_H_
#define _VIEWER_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <cstdarg>

#include "SDL_funcs.h"
#include "MacInput.h"
#include "Quaternions.h"
#include "Camera.h"
#include "Skeleton.h"
#include "Files.h"
#include "Models.h"
#include "TGALoader.h"
#include "Fog.h"
#include "Font.h"
#include "Frustum.h"
#include "Sprites.h"
#include "Person.h"
#include "Decals.h"

#include "Threads.h"
#include "Window.h"
#include "Label.h"
#include "Image.h"

class Viewer {
public:
  static Viewer &getInstance();

  //Eventloop
  Boolean gQuit;
  float gamespeed;
  double end, start, framespersecond;
  int maxfps;
  //Graphics
  int screenwidth, screenheight;
  float viewdistance;

  //GL functions
  int DrawGLScene(void);
  //TODO: do me
  void DrawViewer() {};

  //Game Functions
  void HandleKeyDown(char theChar);
  void EventLoop(void);
  void Tick();
  void setup();

  //Mouse
  Point mouseloc;
  Point oldmouseloc;

  float mouserotation, mouserotation2;
  float oldmouserotation, oldmouserotation2;
  float mousesensitivity;
  float usermousesensitivity;

  int mouseoverbutton;
  int oldmouseoverbutton;

  Point olddrawmouse;

  //Project Specific
  //state
  bool initialized;

  //setting
  float losedelay;
  bool reloadtoggle;
  bool aimtoggle;
  float difficulty;
  bool azertykeyboard;
  ///TODO: add an 'always on' feature for the lasersight?
  bool lasersight;

  Person person[max_people];

  XYZ bodycoords;

  FRUSTUM frustum;
  Model sidewalkcollide;
  Model street;
  Model Bigstreet;
  Model blocksimple;
  XYZ boundingpoints[8];
  Files files;

  int machinegunsoundloop;
  bool blur;

  double eqn[4];

  float oldrot, oldrot2;

   ~Game() {
    glDeleteTextures(1, (const GLuint *)&personspritetextureptr);
    glDeleteTextures(1, (const GLuint *)&deadpersonspritetextureptr);
    glDeleteTextures(1, (const GLuint *)&scopetextureptr);
    glDeleteTextures(1, (const GLuint *)&flaretextureptr);
   }

private:
  static std::unique_ptr <Viewer> instance;
  static Mutex m;
};

#endif
