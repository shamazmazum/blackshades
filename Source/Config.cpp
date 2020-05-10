/*
 * Config.cpp 
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
#include <fstream>
#include <iostream>

#include "Config.h"

Config::Config()
{
  std::cout << "instantiating config class" << std::endl;
  thirdperson = nocolors = visions = 0;

  screenwidth = 640;
  screenheight = 480;
  fullscreen = 0;
  grabmouse = 0;
  usermousesensitivity = .7;
#ifdef DEBUG
  debug = 1;
#else
  debug = 0;
#endif
  vblsync = 1;
  blood = 1;
  blurness = 0;
  mainmenuness = 1;
  customlevels = 0;
  musictoggle = 1;
  azertykeyboard = 0;
  
  const char *cfgFile = "config.txt";

  if(!ConfigExist(cfgFile)) {
    WriteConfig(cfgFile);
  }

  ReadConfig(cfgFile);

  // Sanity checks for fullscreen mode
  if (!fullscreen) {
      screenwidth = (screenwidth > 640)? screenwidth: 640;
      screenheight = (screenheight > 480)? screenheight:480;
  }

  //highscore
  hsFile = "Data/Highscore";
  highscore = 0;
  beatgame = 0;

  ReadHighScore();
}

bool Config::ConfigExist(const char *configFile)
{
  std::ifstream ipstream(configFile);

  if(ipstream) {
    ipstream.close();
    return true;
  }

  return false;
}

bool Config::ReadConfig(const char *configFile)
{
  //Read config
  std::ifstream ipstream(configFile);

  if(ipstream) {
    ipstream.ignore(256, '\n');
    ipstream >> screenwidth;
    ipstream.ignore(256, '\n');
    ipstream.ignore(256, '\n');
    ipstream >> screenheight;
    ipstream.ignore(256, '\n');
    ipstream.ignore(256, '\n');
    ipstream >> fullscreen;
    ipstream.ignore(256, '\n');
    ipstream.ignore(256, '\n');
    ipstream >> grabmouse;
    ipstream.ignore(256, '\n');
    ipstream.ignore(256, '\n');
    ipstream >> usermousesensitivity;
    ipstream.ignore(256, '\n');
    ipstream.ignore(256, '\n');
    ipstream >> debug;
    ipstream.ignore(256, '\n');
    ipstream.ignore(256, '\n');
    ipstream >> vblsync;
    ipstream.ignore(256, '\n');
    ipstream.ignore(256, '\n');
    ipstream >> blood;
    ipstream.ignore(256, '\n');
    ipstream.ignore(256, '\n');
    ipstream >> blurness;
    ipstream.ignore(256, '\n');
    ipstream.ignore(256, '\n');
    ipstream >> mainmenuness;
    ipstream.ignore(256, '\n');
    ipstream.ignore(256, '\n');
    ipstream >> customlevels;
    ipstream.ignore(256, '\n');
    ipstream.ignore(256, '\n');
    ipstream >> musictoggle;
    ipstream.ignore(256, '\n');
    ipstream.ignore(256, '\n');
    ipstream >> azertykeyboard;
    ipstream.close();
    std::cout << "debug: " << debug << std::endl;
    return true;
  }

  return false;
}

void Config::WriteConfig(const char *configFile)
{
  std::ofstream opstream(configFile);
  opstream << "Screenwidth:\n";
  opstream << screenwidth;
  opstream << "\nScreenheight:\n";
  opstream << screenheight;
  opstream << "\nFullscreen:\n";
  opstream << fullscreen;
  opstream << "\nGrab mouse:\n";
  opstream << grabmouse;
  opstream << "\nMouse sensitivity:\n";
  opstream << usermousesensitivity;
  opstream << "\nShow fps and other info:\n";
  opstream << debug;
  opstream << "\nVBL sync:\n";
  opstream << vblsync;
  opstream << "\nBlood:\n";
  opstream << blood;
  opstream << "\nBlur:\n";
  opstream << blurness;
  opstream << "\nMain Menu:\n";
  opstream << mainmenuness;
  opstream << "\nCustom levels:\n";
  opstream << customlevels;
  opstream << "\nMusic:\n";
  opstream << musictoggle;
  opstream << "\nazerty keyboard:\n";
  opstream << azertykeyboard;
  opstream.close();
}

bool Config::ReadHighScore()
{
  //Read high score
  std::ifstream ipstream(hsFile);
  if(!ipstream) {
    WriteHighScore(0);
  }

  if(ipstream) {
    ipstream >> highscore;
    ipstream.ignore(256, '\n');
    ipstream >> beatgame;
    ipstream.close();
    return true;
  }
  return false;
}

void Config::WriteHighScore(int score)
{
  std::ifstream ipstream(hsFile);
  if(!ipstream || score > highscore) {
    std::ofstream opstream(hsFile);
    highscore = score;
    opstream << highscore;
    opstream << "\n";
    opstream << beatgame;
    opstream.close();
  }
}
