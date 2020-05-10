/*
 * Game.h
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

#ifndef _GAME_H_
#define _GAME_H_

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
#include "Text.h"
#include "TGALoader.h"
#include "Fog.h"
#include "Font.h"
#include "Frustum.h"
#include "Sprites.h"
#include "Person.h"
#include "Decals.h"
#include "Globals.h"
#include "Weapon.h"

#include "Threads.h"
#include "Window.h"
#include "Label.h"
#include "Image.h"
#include "Button.h"

#define num_blocks 		100
#define block_spacing 		360
#define max_people 		90
#define max_people_block 	20

class Game {
public:
  static Game &getInstance();

  //Eventloop
  Boolean gQuit;
  float gamespeed;
  double end, start, framespersecond;
  int maxfps;
  //Graphics
  int screenwidth, screenheight;
  float viewdistance;

  //GL functions
  void LoadingScreen(float percent);
  void DrawMouse();
  void DrawFlash();
  void DrawMainMenu();
  void DrawGame();
  int DrawGLScene(void);

  //Game Functions
  void SaveScreenshot(const char *path);
  void HandleKeyDown(char theChar);
  void EventLoop(void);
  //Tick functions
  void Splat(int k);
  void MainMenuTick();
  void SpawnPeople();
  void checkCollisions();
  void Fire();
  void GameTick();
  void Tick();

  void InitGame();
  void InitGUI();
  void Dispose();

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

  //keyboard
  bool tabkeydown;
  bool slomokeydown;
  bool oldvisionkey;

  //Project Specific
  //state
  enum game_state state;
  bool initialized;
  bool paused;
  bool gameinprogress;

  bool oldbutton;
  int enemystate;
  int cycle;

  //setting
  float losedelay;
  bool reloadtoggle;
  bool aimtoggle;
  float difficulty;
  bool azertykeyboard;
  ///TODO: add an 'always on' feature for the lasersight?
  bool lasersight;

  //stat
  int goodkills;
  int badkills;
  int civkills;

  int cityrotation[num_blocks][num_blocks];
  int citytype[num_blocks][num_blocks];
  int citypeoplenum[num_blocks][num_blocks];
  bool drawn[num_blocks][num_blocks];
  int onblockx, onblocky;

  float flashamount;
  float flashr, flashg, flashb;

  Person person[max_people];

  GLuint personspritetextureptr;
  GLuint deadpersonspritetextureptr;
  GLuint scopetextureptr;
  GLuint flaretextureptr;

  XYZ bodycoords;

  FRUSTUM frustum;
  Model blocks[4];
  Model blockwalls[4];
  Model blockcollide[4];
  Model blocksimplecollide[4];
  Model blockroofs[4];
  Model blockocclude;
  Model sidewalkcollide;
  Model street;
  Model Bigstreet;
  Model path;
  Model blocksimple;
  XYZ boundingpoints[8];
  Files files;
  Text text;

  int machinegunsoundloop;
  bool blur;

  XYZ vipgoal;
  XYZ aimer[2];

  double eqn[4];

  float oldrot, oldrot2;

  XYZ lastshot[2];
  bool zoom, oldzoom;

  int numpeople;
  float spawndelay;

  float psychicpower;

  int type;

  float timeremaining;
  int whichsong;
  int oldscore;
  int score;
  int mission;
  int nummissions;
  int numpossibleguns;
  int possiblegun[6];
  int evilprobability;

  int shotcount;

  //checkCollision, GameTick
  unsigned char theKeyMap[16];

  int beginx, endx;
  int beginz, endz;

  XYZ normalrotated;
  XYZ move;
  XYZ underpoint;
  XYZ overpoint;

  int whichtri;

  XYZ facing;
  int murderer;

   ~Game() {
    glDeleteTextures(1, (const GLuint *)&personspritetextureptr);
    glDeleteTextures(1, (const GLuint *)&deadpersonspritetextureptr);
    glDeleteTextures(1, (const GLuint *)&scopetextureptr);
    glDeleteTextures(1, (const GLuint *)&flaretextureptr);
   }

private:
  static std::auto_ptr <Game> instance;
  static Mutex m;
};

#endif
