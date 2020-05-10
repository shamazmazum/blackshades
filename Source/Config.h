/*
 * Config.h
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
#ifndef _CONFIG_H_
#define _CONFIG_H_

class Config {
public:
  Config();
  bool ConfigExist(const char *configFile);
  bool ReadConfig(const char *configFile);
  void WriteConfig(const char *configFile);

  bool ReadHighScore();
  void WriteHighScore(int score);

  int thirdperson;
  int nocolors;
  int visions;

  //scoring
  const char *hsFile;
  int highscore;
  bool beatgame;

  //read-only
  int bpp;

  //configurable
  int screenwidth;
  int screenheight;
  bool fullscreen;
  int grabmouse;
  float usermousesensitivity;
  bool debug;
  bool vblsync;
  bool blood;
  int blurness;
  bool mainmenuness;
  int customlevels;
  int musictoggle;
  int azertykeyboard;
  int invertmouse;
};

#endif
