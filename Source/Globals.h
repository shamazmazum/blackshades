/*
 * Globals.h
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
#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "Colors.h"
#include "Config.h"

#define RAD2DEG 		56.54866776;

enum game_state {
  INIT = 0,
  MAINMENU,
  GAME,
  HELP
};

///simple container classes, here for now
class Environment {
public:
  float precipitationhorz;
  float precipitationvert;
  float precipitationdensity;
  float snowdelay;

  float viewdistance;

  int type;                     //environment

  Color fogcolor;
};

class Mission
{
public:
  int enemytype;
  int num;
  int numpossibleguns;
  int evilprobability;
  int timeremaining;
  float difficulty;
  void setMission(int num, int enemytype, int timeremaining, int difficulty);
};

#endif
