/*
 * Files.h
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
#ifndef _FILES_H_
#define _FILES_H_

#include <stdio.h>
#include <stdlib.h>             // Header File For Standard functions
#include <stdio.h>              // Header File For Standard Input/Output
#include <string.h>
#include <ctype.h>
#include <cstdarg>
#include <cmath>
#include <iostream>
#include <fstream>
#include <GL/gl.h>
#include "MacInput.h"           // Header for Special Mac Input functions

#define FILE_STRINGS              130
#define Pstr unsigned char *
#define Cstr char *

#define FILE_ERROR_ALERT          138
#define OLD_SAVE_VERSION          139
#define UNREADABLE_SCORES_ALERT   140

#define SAVE_WARNING_ID           132
#define PB_SAVE                     1
#define PB_CANCEL                   2
#define PB_DONT_SAVE                3

#define FILE_ERROR_STRINGS        129
#define FILE_ERROR_PREFIX           1
#define FILE_ERROR_SUFFIX           2

#define SAVE_WARNING_STRINGS      132

#define FILE_STRINGS              130
#define SAVE_GAME_STRING            1
#define SAVE_SCORES_STRING          2
#define SCORES_NAME_STRING          3
#define SAVE_JOURNAL_STRING         4
#define JOURNAL_NAME_STRING         5
#define UNTITLED_STRING             6
#define SAVE_FORM_STRING            7
#define FORM_NAME_STRING            8

#define REGISTRATION_FORM         136

#define FILE_NAME_SIZE             32
#define ERROR_LENGTH               80

/**> Files Opening <**/
class Files {
public:
  char szSavedGameName[FILE_NAME_SIZE + 1];
  short sSavedGameVolume;
  int sFile;

  int OpenFile(Str255 Name);
    Files():sFile(-1) {
  }
  short OpenSavedGame(Str255 Name);
  short OpenFileDialog();
  void LoadNamedMap(Str255 Name);
  void LoadGame(Str255 Name, int animnum);

  void LoadMap();
  void StartSave();
  void EndSave();
  void StartLoad();
  void EndLoad();
};

#endif
