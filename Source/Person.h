/*
 * Person.h
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
#ifndef _PERSON_H_
#define _PERSON_H_

#include <cmath>
#include <GL/gl.h>
#include <AL/al.h>
#include <AL/alut.h>
#include "Quaternions.h"
#include "Constants.h"
#include "Skeleton.h"
#include "Models.h"
#include "Camera.h"
#include "Sprites.h"
#include "Globals.h"
#include "Weapon.h"

#define playertype 0
#define civiliantype 1
#define eviltype 2
#define viptype 3
#define evilsnipertype 4
#define evilassaultrifletype 5
#define zombietype 6

class HitStruct {
public:
  Joint * joint1;
  Joint *joint2;
  XYZ hitlocation;
  bool collision;
};

class Person {
public:
  int eaten;
  int currentframe;
  int targetframe;
  int currentanimation;
  int targetanimation;
  float target;
  float playerhealth;
  int modelnum;
  XYZ oldplayercoords;
  XYZ playercoords;
  XYZ playervelocity;
  float playerrotation;
  float playerrotation2;
  float playerlowrotation;
  float pathcheckdelay;
  bool onground;
  bool backwardsanim;
  XYZ facing;
  XYZ velocity;

  bool existing;

  int type;

  int whichcostume;

  Skeleton skeleton;
  Animation tempanimation;

  bool freshshootkey;
  bool freshkickkey;
  int bufferattack;
  int jump_key;
  int left_key;
  int right_key;
  int duck_key;
  int shoot_key;
  int kick_key;
  int block_key;

  float speed;
  bool aiming;
  int grenphase;
  float grenamount;
  float aimamount;
  float speedmult;
  float pathsize;

  int pathnum;
  int oldpathnum;
  int oldoldpathnum;
  int oldoldoldpathnum;
  XYZ pathtarget;
  int whichblockx;
  int whichblocky;

  bool dead;

  XYZ averageloc;
  XYZ oldaverageloc;

  float lastdistancevictim;

  bool firstlongdead;
  float longdead;

  Joint *bjoint1, *bjoint2;
  float bleeding;
  float bleeddelay;

  float attacktarget;
  int attackframe;
  int killtarget;
  bool killtargetvisible;

  float oldhealth;
  float health;
  float maxhealth;
  int reloads[10];
  bool running;
  bool onpath;
  bool litup;
  Weapon weapon;

  void FindRotationGun(XYZ start, XYZ target);

  int DrawSkeleton(int who);
  void DoStuff(int who);
  void DoAnimations(int who);
  void DoAnimationslite(int who);
  HitStruct BulletCollideWithPlayer(int who, XYZ start, XYZ end);
};

class Costume {
public:
  float headcolor[3];
  float handcolor[3];
  float footcolor[3];
  float upperarmcolor[3];
  float lowerarmcolor[3];
  float upperlegcolor[3];
  float lowerlegcolor[3];
  float abdomencolor[3];
  float chestcolor[3];
};

///real human player
class Player : public Person {
  void setTackle();
};

#endif
