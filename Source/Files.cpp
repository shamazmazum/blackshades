/*
 * Files.cpp
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
#include "Files.h"

short Files::OpenFile(Str255 Name)
{
  short volume;
  char filename[33];
  short tFile;
  tFile = -1;
  Point ptOrigin = { 0, 0 };
  volume = 0;
  sprintf(filename, "%s", Name);
  SetVol(nil, volume);
  CtoPstr(filename);
  FSOpen((Pstr) filename, volume, &tFile);
  PtoCstr((Pstr) filename);
  sFile = tFile;
  return (tFile);
}

short Files::OpenNewFile(SFReply * psfReply,
                         OSType osTypeCreator, OSType osTypeType)
{
  sFile = 0;
  OSErr osErr;

  SetVol(nil, psfReply->vRefNum);
  osErr = Create(psfReply->fName, psfReply->vRefNum, osTypeCreator, osTypeType);

  if(osErr == dupFNErr) {
    FSDelete(psfReply->fName, psfReply->vRefNum);
    Create(psfReply->fName, psfReply->vRefNum, osTypeCreator, osTypeType);
  }

  FSOpen(psfReply->fName, psfReply->vRefNum, &sFile);

  return (sFile);
}


short Files::PromptForSaveAS(short sPromptID,
                             short sNameID,
                             Str255 str255NamePrompt,
                             OSType osTypeCreator,
                             OSType osTypeType, SFReply * psfReply)
{
  Str255 str255Prompt;
  Str255 str255Name;
  sFile = 0;
  Point ptOrigin = { 0, 0 };

  GetIndString(str255Prompt, FILE_STRINGS, sPromptID);

  if(!str255NamePrompt)
    GetIndString(str255Name, FILE_STRINGS, sNameID);

  else
    memcpy(str255Name, str255NamePrompt, *str255NamePrompt + 1);

  SFPutFile(ptOrigin, str255Prompt, str255Name, nil, psfReply);

  if(psfReply->good) {
    sFile = OpenNewFile(psfReply, osTypeCreator, osTypeType);
  }

  return (sFile);
}

short Files::OpenSavedGame(Str255 Name)
{
  Point ptOrigin = { 0, 0 };
  sSavedGameVolume = 0;
  sprintf(szSavedGameName, "%s", Name);
  SetVol(nil, sSavedGameVolume);
  CtoPstr(szSavedGameName);
  FSOpen((Pstr) szSavedGameName, sSavedGameVolume, &sFile);
  PtoCstr((Pstr) szSavedGameName);
  return (sFile);
}

short Files::OpenFileDialog()
{
  Point ptOrigin = { 0, 0 };
  sFile = 0;
  SFReply sfReply;
  SFTypeList sfTypeList = { 'DMAP', '\p', '\p', '\p' };
  SFGetFile(ptOrigin, "\p", nil, 1, sfTypeList, nil, &sfReply);

  if(sfReply.good) {
    PtoCstr(sfReply.fName);
    strcpy(szSavedGameName, (Cstr) sfReply.fName);
  }

  if(sfReply.good) {
    sSavedGameVolume = sfReply.vRefNum;
    SetVol(nil, sSavedGameVolume);

    CtoPstr(szSavedGameName);

    FSOpen((Pstr) szSavedGameName, sSavedGameVolume, &sFile);

    PtoCstr((Pstr) szSavedGameName);
  }

  return (sFile);
}

void Files::StartSave()
{
  int x, y;
  SFReply sfReply;
  sFile = 0;
  long lSize;
  long lLongSize = sizeof (long);

  CtoPstr(szSavedGameName);

  sFile =
      PromptForSaveAS(SAVE_GAME_STRING, 0, (Pstr) szSavedGameName, 'DAVD',
                      'DMAP', &sfReply);

  PtoCstr((Pstr) szSavedGameName);

  if(sFile) {
    sSavedGameVolume = sfReply.vRefNum;
    PtoCstr(sfReply.fName);
    strcpy(szSavedGameName, (Cstr) sfReply.fName);
  }

  else {
    sfReply.vRefNum = sSavedGameVolume;
    strcpy((Cstr) sfReply.fName, szSavedGameName);
    CtoPstr((Cstr) sfReply.fName);

    sFile = OpenNewFile(&sfReply, 'GLF2', 'SKLT');
  }
}

void Files::EndSave()
{
  if(sFile)
    FSClose(sFile);
}

void Files::StartLoad()
{
  sFile = OpenFileDialog();
}

void Files::EndLoad()
{
  if(sFile)
    FSClose(sFile);
}
