/*
 * GameInitDispose.cpp
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
#include <ctype.h>
#include <time.h>

#include "Weapon.h"
#include "Game.h"

extern unsigned int gSourceID[100];

extern unsigned int gSampleSet[100];

extern Camera camera;
extern Skeleton testskeleton;
extern Sprites sprites;
extern Decals decals;
extern Environment environment;
extern Config config;
extern Font font;

/*********************> InitGame()<*****/

extern Model skeletonmodels[10];
extern Model gunmodels[10];
extern Costume costume[10];
extern Animation animation[30];
extern Fog fog;
extern Mission mission;

extern bool blood;

extern float soundscalefactor;

extern int slomo;

extern int forwardskey;
extern int backwardskey;
extern int leftkey;
extern int rightkey;
extern int aimkey;
extern int psychicaimkey;
extern int psychickey;
extern int helpkey;

void LoadSounds(bool musictoggle);

void LoadSounds(bool musictoggle)
{
  char *pBuffer1;
  long lBuffer1Len;

  ALenum formatBuffer1;
  ALsizei freqBuffer1;
  SInt16 iNumSources, iNumSampleSets;

  // initialize OpenAL
  alutInit(NULL, 0);

  // load up some audio data...

  // generate ten OpenAL sample sets and two sources
  iNumSources = 37;
  iNumSampleSets = 37;

  alGenSources(iNumSources, &gSourceID[0]);
  alGenBuffers(iNumSampleSets, &gSampleSet[0]);

#ifdef NOOGG

#define loadwav(file, sound) do {\
  alutLoadWAVFile((char *)file, &formatBuffer1, \
                  (void **)&pBuffer1, (unsigned int *)&lBuffer1Len, \
                  &freqBuffer1); \
  alBufferData(gSampleSet[sound], formatBuffer1, pBuffer1, lBuffer1Len, \
               freqBuffer1); \
  alSourcei(gSourceID[sound], AL_BUFFER, gSampleSet[sound]); \
  alutUnloadWAV(formatBuffer1, pBuffer1, lBuffer1Len, freqBuffer1); \
  } while(0)

  loadwav(":Data:Sounds:underwater.wav", visionsound);
  loadwav(":Data:Sounds:soulin.wav", soulinsound);
  loadwav(":Data:Sounds:soulout.wav", souloutsound);
  loadwav(":Data:Sounds:footstep1.wav", footstepsound);
  loadwav(":Data:Sounds:footstep2.wav", footstepsound + 1);
  loadwav(":Data:Sounds:footstep3.wav", footstepsound + 2);
  loadwav(":Data:Sounds:footstep4.wav", footstepsound + 3);
  loadwav(":Data:Sounds:footstep5.wav", footstepsound + 4);
  loadwav(":Data:Sounds:bodyland.wav", bodylandsound);
  loadwav(":Data:Sounds:headland.wav", headlandsound);
  loadwav(":Data:Sounds:sniperrifle.wav", riflesound);
  loadwav(":Data:Sounds:BodyHit.wav", bodyhitsound);
  loadwav(":Data:Sounds:WallHit.wav", wallhitsound);
  loadwav(":Data:Sounds:machinegun.wav", machinegunsound);
  loadwav(":Data:Sounds:Nearbullet.wav", nearbulletsound);
  loadwav(":Data:Sounds:riflewhack.wav", headwhacksound);
  loadwav(":Data:Sounds:headshot.wav", headshotsound);
  loadwav(":Data:Sounds:reload.wav", reloadsound);
  loadwav(":Data:Sounds:click.wav", clicksound);
  loadwav(":Data:Sounds:SW.wav", pistol1sound);
  loadwav(":Data:Sounds:glock.wav", pistol2sound);
  loadwav(":Data:Sounds:pinpull.wav", pinpullsound);
  loadwav(":Data:Sounds:pinreplace.wav", pinreplacesound);
  loadwav(":Data:Sounds:handlerelease.wav", grenadethrowsound);
  loadwav(":Data:Sounds:bounce.wav", bouncesound);
  loadwav(":Data:Sounds:bounce2.wav", bounce2sound);
  loadwav(":Data:Sounds:explosion.wav", explosionsound);
  loadwav(":Data:Sounds:headland.wav", bodywhacksound);
  loadwav(":Data:Sounds:rain.wav", rainsound);
  loadwav(":Data:Sounds:Lose.wav", losesound);
  loadwav(":Data:Sounds:Disguisekill.wav", disguisekillsound);
  loadwav(":Data:Sounds:knifeslash.wav", knifeslashsound);
  loadwav(":Data:Sounds:shotgun.wav", shotgunsound);

  if(musictoggle) {
    loadwav(":Data:Sounds:mainmenusong.wav", mainmenusong);
    loadwav(":Data:Sounds:shootsong.wav", shootsong);
    loadwav(":Data:Sounds:zombiesong.wav", zombiesong);
    loadwav(":Data:Sounds:knifesong.wav", knifesong);
  }
#else

#define loadogg(file, sound) do {\
  LoadOGG_CFH((char *)file, &formatBuffer1, \
              (void **)&pBuffer1, (unsigned int *)&lBuffer1Len, &freqBuffer1); \
  alBufferData(gSampleSet[sound], formatBuffer1, pBuffer1, lBuffer1Len, \
               freqBuffer1); \
  alSourcei(gSourceID[sound], AL_BUFFER, gSampleSet[sound]); \
  FreeOGG(formatBuffer1, pBuffer1, lBuffer1Len, freqBuffer1); \
  } while(0)

  loadogg(":Data:Sounds:underwater.ogg", visionsound);
  loadogg(":Data:Sounds:soulin.ogg", soulinsound);
  loadogg(":Data:Sounds:soulout.ogg", souloutsound);
  loadogg(":Data:Sounds:footstep1.ogg", footstepsound);
  loadogg(":Data:Sounds:footstep2.ogg", footstepsound + 1);
  loadogg(":Data:Sounds:footstep3.ogg", footstepsound + 2);
  loadogg(":Data:Sounds:footstep4.ogg", footstepsound + 3);
  loadogg(":Data:Sounds:footstep5.ogg", footstepsound + 4);
  loadogg(":Data:Sounds:bodyland.ogg", bodylandsound);
  loadogg(":Data:Sounds:headland.ogg", headlandsound);
  loadogg(":Data:Sounds:sniperrifle.ogg", riflesound);
  loadogg(":Data:Sounds:BodyHit.ogg", bodyhitsound);
  loadogg(":Data:Sounds:WallHit.ogg", wallhitsound);
  loadogg(":Data:Sounds:machinegun.ogg", machinegunsound);
  loadogg(":Data:Sounds:Nearbullet.ogg", nearbulletsound);
  loadogg(":Data:Sounds:riflewhack.ogg", headwhacksound);
  loadogg(":Data:Sounds:headshot.ogg", headshotsound);
  loadogg(":Data:Sounds:reload.ogg", reloadsound);
  loadogg(":Data:Sounds:click.ogg", clicksound);
  loadogg(":Data:Sounds:SW.ogg", pistol1sound);
  loadogg(":Data:Sounds:glock.ogg", pistol2sound);
  loadogg(":Data:Sounds:pinpull.ogg", pinpullsound);
  loadogg(":Data:Sounds:pinreplace.ogg", pinreplacesound);
  loadogg(":Data:Sounds:handlerelease.ogg", grenadethrowsound);
  loadogg(":Data:Sounds:bounce.ogg", bouncesound);
  loadogg(":Data:Sounds:bounce2.ogg", bounce2sound);
  loadogg(":Data:Sounds:explosion.ogg", explosionsound);
  loadogg(":Data:Sounds:headland.ogg", bodywhacksound);
  loadogg(":Data:Sounds:rain.ogg", rainsound);
  loadogg(":Data:Sounds:Lose.ogg", losesound);
  loadogg(":Data:Sounds:Disguisekill.ogg", disguisekillsound);
  loadogg(":Data:Sounds:knifeslash.ogg", knifeslashsound);
  loadogg(":Data:Sounds:shotgun.ogg", shotgunsound);

  if(musictoggle) {
    loadogg(":Data:Sounds:mainmenusong.ogg", mainmenusong);
    loadogg(":Data:Sounds:shootsong.ogg", shootsong);
    loadogg(":Data:Sounds:zombiesong.ogg", zombiesong);
    loadogg(":Data:Sounds:knifesong.ogg", knifesong);
  }
#endif

  float gLoc[3];

  gLoc[0] = 0;
  gLoc[1] = 0;
  gLoc[2] = 0;

  alSourcefv(gSourceID[visionsound], AL_POSITION, gLoc);
  alSourcei(gSourceID[visionsound], AL_LOOPING, 1);
  alSourcef(gSourceID[visionsound], AL_MIN_GAIN, 1);

  alSourcefv(gSourceID[soulinsound], AL_POSITION, gLoc);
  alSourcei(gSourceID[soulinsound], AL_LOOPING, 0);
  alSourcef(gSourceID[soulinsound], AL_MIN_GAIN, 1);

  alSourcefv(gSourceID[souloutsound], AL_POSITION, gLoc);
  alSourcei(gSourceID[souloutsound], AL_LOOPING, 0);
  alSourcef(gSourceID[souloutsound], AL_MIN_GAIN, 1);

  for(int i = 0; i < 5; i++) {
    alSourcefv(gSourceID[footstepsound + i], AL_POSITION, gLoc);
    alSourcei(gSourceID[footstepsound + i], AL_LOOPING, 0);
    alSourcef(gSourceID[footstepsound + i], AL_MIN_GAIN, 0);
  }

  alSourcefv(gSourceID[bodylandsound], AL_POSITION, gLoc);
  alSourcei(gSourceID[bodylandsound], AL_LOOPING, 0);
  alSourcef(gSourceID[bodylandsound], AL_MIN_GAIN, 0);

  alSourcefv(gSourceID[headlandsound], AL_POSITION, gLoc);
  alSourcei(gSourceID[headlandsound], AL_LOOPING, 0);
  alSourcef(gSourceID[headlandsound], AL_MIN_GAIN, 0);

  alSourcefv(gSourceID[riflesound], AL_POSITION, gLoc);
  alSourcei(gSourceID[riflesound], AL_LOOPING, 0);
  alSourcef(gSourceID[riflesound], AL_MIN_GAIN, 0);

  alSourcefv(gSourceID[bodyhitsound], AL_POSITION, gLoc);
  alSourcei(gSourceID[bodyhitsound], AL_LOOPING, 0);
  alSourcef(gSourceID[bodyhitsound], AL_MIN_GAIN, .1);

  alSourcefv(gSourceID[wallhitsound], AL_POSITION, gLoc);
  alSourcei(gSourceID[wallhitsound], AL_LOOPING, 0);
  alSourcef(gSourceID[wallhitsound], AL_MIN_GAIN, 0);
  alSourcef(gSourceID[wallhitsound], AL_MAX_GAIN, .6);

  alSourcefv(gSourceID[machinegunsound], AL_POSITION, gLoc);
  alSourcei(gSourceID[machinegunsound], AL_LOOPING, 0);
  alSourcef(gSourceID[machinegunsound], AL_MIN_GAIN, 0);

  alSourcefv(gSourceID[nearbulletsound], AL_POSITION, gLoc);
  alSourcei(gSourceID[nearbulletsound], AL_LOOPING, 0);
  alSourcef(gSourceID[nearbulletsound], AL_MIN_GAIN, 0);

  alSourcefv(gSourceID[headwhacksound], AL_POSITION, gLoc);
  alSourcei(gSourceID[headwhacksound], AL_LOOPING, 0);
  alSourcef(gSourceID[headwhacksound], AL_MIN_GAIN, 0);

  alSourcefv(gSourceID[headshotsound], AL_POSITION, gLoc);
  alSourcei(gSourceID[headshotsound], AL_LOOPING, 0);
  alSourcef(gSourceID[headshotsound], AL_MIN_GAIN, 0);

  alSourcefv(gSourceID[reloadsound], AL_POSITION, gLoc);
  alSourcei(gSourceID[reloadsound], AL_LOOPING, 0);
  alSourcef(gSourceID[reloadsound], AL_MIN_GAIN, 0);

  alSourcefv(gSourceID[clicksound], AL_POSITION, gLoc);
  alSourcei(gSourceID[clicksound], AL_LOOPING, 0);
  alSourcef(gSourceID[clicksound], AL_MIN_GAIN, 0);

  alSourcefv(gSourceID[pistol1sound], AL_POSITION, gLoc);
  alSourcei(gSourceID[pistol1sound], AL_LOOPING, 0);
  alSourcef(gSourceID[pistol1sound], AL_MIN_GAIN, 0);

  alSourcefv(gSourceID[pistol2sound], AL_POSITION, gLoc);
  alSourcei(gSourceID[pistol2sound], AL_LOOPING, 0);
  alSourcef(gSourceID[pistol2sound], AL_MIN_GAIN, 0);

  alSourcefv(gSourceID[pinpullsound], AL_POSITION, gLoc);
  alSourcei(gSourceID[pinpullsound], AL_LOOPING, 0);
  alSourcef(gSourceID[pinpullsound], AL_MIN_GAIN, 0);

  alSourcefv(gSourceID[pinreplacesound], AL_POSITION, gLoc);
  alSourcei(gSourceID[pinreplacesound], AL_LOOPING, 0);
  alSourcef(gSourceID[pinreplacesound], AL_MIN_GAIN, 0);

  alSourcefv(gSourceID[grenadethrowsound], AL_POSITION, gLoc);
  alSourcei(gSourceID[grenadethrowsound], AL_LOOPING, 0);
  alSourcef(gSourceID[grenadethrowsound], AL_MIN_GAIN, 0);

  alSourcefv(gSourceID[bouncesound], AL_POSITION, gLoc);
  alSourcei(gSourceID[bouncesound], AL_LOOPING, 0);
  alSourcef(gSourceID[bouncesound], AL_MIN_GAIN, 0);

  alSourcefv(gSourceID[bounce2sound], AL_POSITION, gLoc);
  alSourcei(gSourceID[bounce2sound], AL_LOOPING, 0);
  alSourcef(gSourceID[bounce2sound], AL_MIN_GAIN, 0);

  alSourcefv(gSourceID[explosionsound], AL_POSITION, gLoc);
  alSourcei(gSourceID[explosionsound], AL_LOOPING, 0);
  alSourcef(gSourceID[explosionsound], AL_MIN_GAIN, 0);

  alSourcefv(gSourceID[bodywhacksound], AL_POSITION, gLoc);
  alSourcei(gSourceID[bodywhacksound], AL_LOOPING, 0);
  alSourcef(gSourceID[bodywhacksound], AL_MIN_GAIN, 0);

  alSourcefv(gSourceID[rainsound], AL_POSITION, gLoc);
  alSourcei(gSourceID[rainsound], AL_LOOPING, 1);
  alSourcef(gSourceID[rainsound], AL_MIN_GAIN, .3);

  alSourcefv(gSourceID[losesound], AL_POSITION, gLoc);
  alSourcei(gSourceID[losesound], AL_LOOPING, 0);
  alSourcef(gSourceID[losesound], AL_MIN_GAIN, 1);

  alSourcefv(gSourceID[disguisekillsound], AL_POSITION, gLoc);
  alSourcei(gSourceID[disguisekillsound], AL_LOOPING, 0);
  alSourcef(gSourceID[disguisekillsound], AL_MIN_GAIN, 1);

  alSourcefv(gSourceID[knifeslashsound], AL_POSITION, gLoc);
  alSourcei(gSourceID[knifeslashsound], AL_LOOPING, 0);
  alSourcef(gSourceID[knifeslashsound], AL_MIN_GAIN, 0);

  alSourcefv(gSourceID[shotgunsound], AL_POSITION, gLoc);
  alSourcei(gSourceID[shotgunsound], AL_LOOPING, 0);
  alSourcef(gSourceID[shotgunsound], AL_MIN_GAIN, 0);

  alSourcefv(gSourceID[knifesong], AL_POSITION, gLoc);
  alSourcei(gSourceID[knifesong], AL_LOOPING, 1);
  alSourcef(gSourceID[knifesong], AL_MIN_GAIN, 1);

  alSourcefv(gSourceID[mainmenusong], AL_POSITION, gLoc);
  alSourcei(gSourceID[mainmenusong], AL_LOOPING, 1);
  alSourcef(gSourceID[mainmenusong], AL_MIN_GAIN, 1);

  alSourcefv(gSourceID[zombiesong], AL_POSITION, gLoc);
  alSourcei(gSourceID[zombiesong], AL_LOOPING, 1);
  alSourcef(gSourceID[zombiesong], AL_MIN_GAIN, 1);

  alSourcefv(gSourceID[shootsong], AL_POSITION, gLoc);
  alSourcei(gSourceID[shootsong], AL_LOOPING, 1);
  alSourcef(gSourceID[shootsong], AL_MIN_GAIN, 1);
}

void LoadPersonSpriteTexture(const char *fileName, GLuint * textureid);

void LoadPersonSpriteTexture(const char *fileName, GLuint * textureid)
{
  TGAImageRec *tempTexture;
  GLuint type;

  //Load Image
  tempTexture = LoadTGA(fileName);

  //Is it valid?
  if(tempTexture) {
    //Alpha channel?
    if(tempTexture->bpp == 24)
      type = GL_RGB;
    else
      type = GL_RGBA;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, textureid);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, *textureid);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, type, tempTexture->sizeX,
                 tempTexture->sizeY, 0, type, GL_UNSIGNED_BYTE,
                 tempTexture->data);

    free(tempTexture->data);
    free(tempTexture);
  }
}

void Game::InitGame()
{
  screenwidth = config.screenwidth;
  screenheight = config.screenheight;

  shotcount = 1; //first screenshot

  //test class is deprecated
  //needed here for the loading screen
  text.LoadFontTexture(":Data:Textures:Font.tga");
  text.BuildFont();

  //Setup loading screen
  float loadingscreenamount = 0;
  float loadingscreenamounttotal = 200;

  if(initialized)
    loadingscreenamounttotal = 20;

  if(!initialized) {
    LoadingScreen(loadingscreenamount / loadingscreenamounttotal * 100);
  }

  //Set up rain and snow
  environment.precipitationhorz = 60;
  environment.precipitationvert = 40;
  environment.precipitationdensity = 25;

  //Bodyguard stats
  person[0].playercoords = camera.position;
  person[0].oldplayercoords = person[0].playercoords;
  person[0].type = playertype;
  person[0].existing = 1;

  for(int i = 0; i < 10; i++) {
    person[0].reloads[i] = 0;
  }

  //Level setup
  //TODO: add level class
  if(config.customlevels) {            //Load custom levels
    nummissions = 1;            //Default level in case of load failure

    type = randomshoot_type;

    possiblegun[0] = handgun1;
    possiblegun[1] = handgun2;
    possiblegun[2] = shotgun;

    numpossibleguns = 3;

    evilprobability = 6;

    person[0].weapon.type = knife;
    person[0].reloads[person[0].weapon.type] = 6;

    if(!gameinprogress)
      score = 0;

    timeremaining = 50;

    difficulty = .8;

    ifstream ipstream("Data/customlevels.txt");

    if(!ipstream) {
      config.customlevels = 0;
    }
    ///FIXME: set environment correctly
    if(ipstream) {

      ipstream.ignore(256, '\n');       //ignore descriptive text

      ipstream >> nummissions;

      for(int j = 0; j < mission + 1; j++) {

        ipstream.ignore(256, '\n');
        ipstream.ignore(256, '\n');

        ipstream >> type;

        ipstream.ignore(256, '\n');
        ipstream.ignore(256, '\n');

        ipstream >> environment.type;

        ipstream.ignore(256, '\n');
        ipstream.ignore(256, '\n');

        ipstream >> numpossibleguns;

        ipstream.ignore(256, '\n');
        ipstream.ignore(256, '\n');

        for(int i = 0; i < numpossibleguns; i++) {
          ipstream >> possiblegun[i];
        }

        ipstream.ignore(256, '\n');
        ipstream.ignore(256, '\n');

        ipstream >> evilprobability;

        ipstream.ignore(256, '\n');
        ipstream.ignore(256, '\n');

        ipstream >> person[0].weapon.type;

        ipstream.ignore(256, '\n');
        ipstream.ignore(256, '\n');

        ipstream >> person[0].reloads[person[0].weapon.type];

        ipstream.ignore(256, '\n');
        ipstream.ignore(256, '\n');

        ipstream >> timeremaining;

        ipstream.ignore(256, '\n');
        ipstream.ignore(256, '\n');

        ipstream >> difficulty;

        ipstream.ignore(256, '\n');
      }

      ipstream.close();
    }
  }

  if(!config.customlevels) {           //Setup hardcoded default levels

    if(mission == 0) {

      environment.type = sunny_environment;

      type = randomshoot_type;

      possiblegun[0] = handgun1;
      possiblegun[1] = handgun2;
      possiblegun[2] = shotgun;

      numpossibleguns = 3;

      evilprobability = 6;

      person[0].weapon.type = assaultrifle;
      person[0].reloads[person[0].weapon.type] = 6;

      if(!gameinprogress)
        score = 0;

      timeremaining = 50;

      difficulty = .6;
    }

    if(mission == 1) {

      environment.type = snowy_environment;

      type = randomshoot_type;

      possiblegun[0] = knife;
      possiblegun[1] = assaultrifle;

      numpossibleguns = 2;

      evilprobability = 5;

      person[0].weapon.type = handgun2;
      person[0].reloads[person[0].weapon.type] = 3;

      if(!gameinprogress)
        score = 0;

      timeremaining = 40;

      difficulty = .6;
    }

    if(mission == 2) {

      environment.type = foggy_environment;

      type = randomshoot_type;

      possiblegun[0] = sniperrifle;

      numpossibleguns = 1;

      evilprobability = 5;

      person[0].weapon.type = sniperrifle;
      person[0].reloads[person[0].weapon.type] = 4;

      if(!gameinprogress)
        score = 0;

      timeremaining = 50;

      difficulty = 0.9;
    }

    if(mission == 3) {

      environment.type = firey_environment;

      type = zombie_type;

      numpossibleguns = 0;

      evilprobability = 5;

      person[0].weapon.type = shotgun;
      person[0].reloads[person[0].weapon.type] = 5;

      if(!gameinprogress)
        score = 0;

      timeremaining = 35;

      difficulty = .7;
    }

    if(mission == 4) {

      environment.type = snowy_environment;

      type = randomshoot_type;

      possiblegun[0] = sniperrifle;
      possiblegun[1] = assaultrifle;

      numpossibleguns = 2;

      evilprobability = 5;

      person[0].weapon.type = grenade;

      person[0].reloads[person[0].weapon.type] = 20;

      if(!gameinprogress)
        score = 0;

      timeremaining = 30;

      difficulty = .5;

    }

    if(mission == 5) {

      environment.type = rainy_environment;

      type = randomshoot_type;

      possiblegun[0] = handgun1;
      possiblegun[1] = shotgun;
      possiblegun[2] = assaultrifle;

      numpossibleguns = 3;

      evilprobability = 6;

      person[0].weapon.type = knife;
      person[0].reloads[person[0].weapon.type] = 3;

      if(!gameinprogress)
        score = 0;

      timeremaining = 40;

      difficulty = .8;
    }

    if(mission == 6) {

      environment.type = night_environment;

      type = randomshoot_type;

      possiblegun[1] = handgun1;
      possiblegun[2] = handgun2;
      possiblegun[3] = shotgun;

      numpossibleguns = 3;

      evilprobability = 5;

      person[0].weapon.type = handgun1;
      person[0].reloads[person[0].weapon.type] = 4;

      if(!gameinprogress)
        score = 0;

      timeremaining = 30;

      difficulty = 1;
    }

    if(mission == 7) {

      environment.type = firey_environment;

      type = zombie_type;

      person[0].weapon.type = assaultrifle;
      person[0].reloads[person[0].weapon.type] = 5;

      if(!gameinprogress)
        score = 0;

      timeremaining = 30;

      difficulty = 1;
    }

    if(mission == 8) {

      environment.type = rainy_environment;

      type = randomshoot_type;

      possiblegun[0] = handgun1;
      possiblegun[1] = handgun2;
      possiblegun[2] = shotgun;
      possiblegun[3] = sniperrifle;
      possiblegun[4] = assaultrifle;

      numpossibleguns = 5;

      evilprobability = 5;

      person[0].weapon.type = nogun;
      person[0].reloads[person[0].weapon.type] = 3;

      if(!gameinprogress)
        score = 0;

      timeremaining = 40;

      difficulty = .8;

    }

    if(mission == 9) {

      environment.type = snowy_environment;

      type = randomshoot_type;

      possiblegun[0] = knife;
      possiblegun[1] = handgun1;
      possiblegun[2] = handgun2;
      possiblegun[3] = shotgun;
      possiblegun[4] = sniperrifle;
      possiblegun[5] = assaultrifle;

      numpossibleguns = 6;

      evilprobability = 4;

      person[0].weapon.type = handgun1;
      person[0].reloads[person[0].weapon.type] = 3;

      if(!gameinprogress)
        score = 0;

      timeremaining = 90;

      difficulty = 1;
    }

    if(mission == 10) {

      environment.type = night_environment;

      type = randomshoot_type;

      possiblegun[0] = sniperrifle;

      numpossibleguns = 1;

      evilprobability = 5;

      person[0].weapon.type = sniperrifle;
      person[0].reloads[person[0].weapon.type] = 4;

      if(!gameinprogress)
        score = 0;

      timeremaining = 30;

      difficulty = 1.3;
    }

    if(mission == 11) {

      environment.type = sunny_environment;

      type = randomshoot_type;

      possiblegun[0] = knife;
      possiblegun[1] = sniperrifle;

      numpossibleguns = 2;

      evilprobability = 4;

      person[0].weapon.type = knife;

      if(!gameinprogress)
        score = 0;

      timeremaining = 30;

      difficulty = 1.5;
    }

    if(mission == 12) {

      environment.type = firey_environment;

      type = zombie_type;

      possiblegun[0] = knife;
      possiblegun[1] = sniperrifle;

      person[0].weapon.type = handgun2;
      person[0].reloads[person[0].weapon.type] = 10;

      if(!gameinprogress)
        score = 0;

      timeremaining = 60;

      difficulty = 1.5;
    }

    nummissions = 13;
  }

  //setup env: sunny, night, snowy, foggy, rainy, firey
  switch (environment.type) {
    case sunny_environment:
      environment.viewdistance = 2000;
      environment.fogcolor.setColor(0.5, 0.5, 0.8);
      break;
    case night_environment:
      environment.viewdistance = 500;
      environment.fogcolor.setColor(0.15, 0.15, 0.3);
      break;
    case snowy_environment:
      environment.viewdistance = 800;
      environment.fogcolor.setColor(0.5, 0.5, 0.5);
      break;
    case foggy_environment:
      environment.viewdistance = 500;
      environment.fogcolor.setColor(0.5, 0.5, 0.5);
      break;
    case rainy_environment:
      environment.viewdistance = 700;
      environment.fogcolor.setColor(0.3, 0.3, 0.3);
      break;
    case firey_environment:
      environment.viewdistance = 600;
      environment.fogcolor.setColor(0.3, 0.1, 0.0);
      break;
    default:
      //set a default?
      break;
  }

  //visions off by default
  config.visions = 0;

  //Setup bounding cylinder model
  float boundingscale = 3;

  if(!initialized) {
    boundingpoints[0] = 0;      //x
    boundingpoints[0].z = boundingscale;
    boundingpoints[0].y = 0;

    for(int i = 1; i < 8; i++) {
      boundingpoints[i] = DoRotation(boundingpoints[0], 0, i * 360 / 7, 0);
    }
  }

  civkills = 0;
  badkills = 0;
  goodkills = 0;
  enemystate = 2;

  if(!initialized) {

    if(!azertykeyboard) {
      forwardskey = MAC_W_KEY;
      backwardskey = MAC_S_KEY;
      leftkey = MAC_A_KEY;
      rightkey = MAC_D_KEY;
      aimkey = MAC_Q_KEY;
      psychicaimkey = MAC_E_KEY;
      psychickey = MAC_Z_KEY;
      helpkey = MAC_F1_KEY;
    } else if(azertykeyboard) {
      forwardskey = MAC_Z_KEY;
      backwardskey = MAC_S_KEY;
      leftkey = MAC_Q_KEY;
      rightkey = MAC_D_KEY;
      aimkey = MAC_A_KEY;
      psychicaimkey = MAC_E_KEY;
      psychickey = MAC_W_KEY;
      helpkey = MAC_F1_KEY;
    }

    soundscalefactor = soundscalefactordefault; //Setup sound falloff

    gQuit = false;

    //Sounds
    LoadSounds(config.musictoggle);

    //Play correct song
    if(environment.type == rainy_environment)
      alSourcePlay(gSourceID[rainsound]);

    if(environment.type != rainy_environment)
      alSourcePause(gSourceID[rainsound]);

    alSourceStop(gSourceID[whichsong]);

    alSourcef(gSourceID[whichsong], AL_MIN_GAIN, 0);
    alSourcef(gSourceID[whichsong], AL_MAX_GAIN, 0);

    whichsong = mainmenusong;

    alSourceStop(gSourceID[whichsong]);

    alSourcef(gSourceID[whichsong], AL_PITCH, 1);

    alSourcePlay(gSourceID[whichsong]);

    alSourcef(gSourceID[whichsong], AL_MIN_GAIN, 1);
    alSourcef(gSourceID[whichsong], AL_MAX_GAIN, 1);
  }

  loadingscreenamount += 5;

  if(!initialized)
    LoadingScreen(loadingscreenamount / loadingscreenamounttotal * 100);

  //Setup random seed
  srand(time(NULL));

  gamespeed = 1;

  //Setup camera
  camera.position = 0;
  camera.position.x = num_blocks / 2 * block_spacing + block_spacing / 2;
  camera.position.z = num_blocks / 2 * block_spacing + block_spacing / 2;
  camera.position.y = 30;
  camera.oldposition = camera.position;

  numpeople = 1;

  //Setup path to walk around blocks
  path.load((unsigned char *)":Data:Models:path.solid");
  path.Rotate(90, 0, 0);
  path.Scale(.8, .8, .8);
  path.CalculateNormals();

  loadingscreenamount += 5;

  if(!initialized)
    LoadingScreen(loadingscreenamount / loadingscreenamounttotal * 100);

  person[0].speedmult = 1.3;

  //Add vip       
  person[numpeople].playerrotation = 0;
  person[numpeople].whichcostume = vipcostume;

  person[numpeople].whichblockx =
      (int)((person[0].playercoords.x + block_spacing / 2) / block_spacing);

  person[numpeople].whichblocky =
      (int)((person[0].playercoords.x + block_spacing / 2) / block_spacing);

  person[numpeople].pathnum = -1;
  person[numpeople].oldpathnum = -1;
  person[numpeople].oldoldpathnum = -1;
  person[numpeople].oldoldoldpathnum = -1;

  while(person[numpeople].pathnum < 0
        || person[numpeople].pathnum >= path.vertexNum
        || person[numpeople].pathnum == 1) {

    person[numpeople].pathnum = Random() % path.vertexNum;
  }

  person[numpeople].pathtarget.x = path.vertex[person[numpeople].pathnum].x;
  person[numpeople].pathtarget.z = path.vertex[person[numpeople].pathnum].z;
  person[numpeople].pathsize = .98 + float (abs(Random() % 20)) / 400;
  person[numpeople].pathtarget *= person[numpeople].pathsize;

  person[numpeople].pathtarget.x +=
      person[numpeople].whichblockx * block_spacing;

  person[numpeople].pathtarget.z +=
      person[numpeople].whichblocky * block_spacing;

  person[numpeople].playercoords = person[numpeople].pathtarget;
  person[numpeople].oldplayercoords = person[numpeople].playercoords;

  person[0].playercoords = person[numpeople].playercoords;
  person[0].playercoords.x += 1;
  person[0].playercoords.z += 1;

  person[0].oldplayercoords = person[0].playercoords;

  person[numpeople].skeleton.free = 0;
  person[numpeople].targetanimation = walkanim;
  person[numpeople].speed = 1;
  person[numpeople].existing = 0;
  person[numpeople].speedmult = 1;

  if(type == zombie_type)
    person[numpeople].speedmult = .8;

  person[numpeople].health = 100;
  person[numpeople].playerrotation2 = 0;        //20;

  person[numpeople].lastdistancevictim = 200000;

  if(person[numpeople].skeleton.broken)
    person[numpeople].skeleton.Load((char *)":Data:Skeleton:Basic Figure");

  person[numpeople].type = viptype;
  person[numpeople].weapon.type = nogun;
  person[numpeople].aiming = 0;
  person[numpeople].killtarget = -1;
  person[numpeople].existing = 1;

  citypeoplenum[person[numpeople].whichblockx][person[numpeople].whichblocky]++;

  numpeople++;

  spawndelay = .1;

  XYZ vipdistance;

  vipdistance = 0;
  vipdistance.x = 10000000;

  vipgoal =
      person[1].playercoords + DoRotation(vipdistance, 0, Random() % 360, 0);

  //Setup block models    

  if(!initialized) {
    blocks[0].load((unsigned char *)":Data:Models:Block1.solid");
    blocks[0].Rotate(90, 0, 0);
    blocks[0].Scale(.8, .8, .8);
    blocks[0].CalculateNormals();

    blocks[1].load((unsigned char *)":Data:Models:Block2.solid");
    blocks[1].Rotate(90, 0, 0);
    blocks[1].Scale(.8, .8, .8);
    blocks[1].CalculateNormals();

    loadingscreenamount += 5;

    if(!initialized)
      LoadingScreen(loadingscreenamount / loadingscreenamounttotal * 100);

    blocks[2].load((unsigned char *)":Data:Models:Block3.solid");
    blocks[2].Rotate(90, 0, 0);
    blocks[2].Scale(.8, .8, .8);
    blocks[2].CalculateNormals();

    blocks[3].load((unsigned char *)":Data:Models:Block4.solid");
    blocks[3].Rotate(90, 0, 0);
    blocks[3].Scale(.8, .8, .8);
    blocks[3].CalculateNormals();

    loadingscreenamount += 5;

    if(!initialized)
      LoadingScreen(loadingscreenamount / loadingscreenamounttotal * 100);

    sidewalkcollide.
        load((unsigned char *)":Data:Models:Lowheightcollide.solid");
    sidewalkcollide.Rotate(90, 0, 0);
    sidewalkcollide.Scale(.8, .8, .8);
    sidewalkcollide.CalculateNormals();

    blockwalls[0].load((unsigned char *)":Data:Models:Block1collide.solid");
    blockwalls[0].Rotate(90, 0, 0);
    blockwalls[0].Scale(.8, .75, .8);
    blockwalls[0].CalculateNormals();

    blockwalls[0].load((unsigned char *)":Data:Models:Block2collide.solid");
    blockwalls[0].Rotate(90, 0, 0);
    blockwalls[0].Scale(.8, .75, .8);
    blockwalls[0].CalculateNormals();

    blockwalls[0].load((unsigned char *)":Data:Models:Block3collide.solid");
    blockwalls[0].Rotate(90, 0, 0);
    blockwalls[0].Scale(.8, .75, .8);
    blockwalls[0].CalculateNormals();

    blockwalls[0].load((unsigned char *)":Data:Models:Block4collide.solid");
    blockwalls[0].Rotate(90, 0, 0);
    blockwalls[0].Scale(.8, .75, .8);
    blockwalls[0].CalculateNormals();

    loadingscreenamount += 5;

    if(!initialized)
      LoadingScreen(loadingscreenamount / loadingscreenamounttotal * 100);

    blockroofs[0].load((unsigned char *)":Data:Models:Highblock1collide.solid");
    blockroofs[0].Rotate(90, 0, 0);
    blockroofs[0].Scale(.8, .8, .8);
    blockroofs[0].CalculateNormals();

    blockroofs[1].load((unsigned char *)":Data:Models:Highblock2collide.solid");
    blockroofs[1].Rotate(90, 0, 0);
    blockroofs[1].Scale(.8, .8, .8);
    blockroofs[1].CalculateNormals();

    blockroofs[2].load((unsigned char *)":Data:Models:Highblock3collide.solid");
    blockroofs[2].Rotate(90, 0, 0);
    blockroofs[2].Scale(.8, .8, .8);
    blockroofs[2].CalculateNormals();

    blockroofs[3].load((unsigned char *)":Data:Models:Highblock4collide.solid");
    blockroofs[3].Rotate(90, 0, 0);
    blockroofs[3].Scale(.8, .8, .8);
    blockroofs[3].CalculateNormals();

    blockcollide[0].load((unsigned char *)":Data:Models:block1complete.solid");
    blockcollide[0].Rotate(90, 0, 0);
    blockcollide[0].Scale(.8, .8, .8);
    blockcollide[0].CalculateNormals();

    blockcollide[1].load((unsigned char *)":Data:Models:block2complete.solid");
    blockcollide[1].Rotate(90, 0, 0);
    blockcollide[1].Scale(.8, .8, .8);
    blockcollide[1].CalculateNormals();

    blockcollide[2].load((unsigned char *)":Data:Models:block3complete.solid");
    blockcollide[2].Rotate(90, 0, 0);
    blockcollide[2].Scale(.8, .8, .8);
    blockcollide[2].CalculateNormals();

    blockcollide[3].load((unsigned char *)":Data:Models:block4complete.solid");
    blockcollide[3].Rotate(90, 0, 0);
    blockcollide[3].Scale(.8, .8, .8);
    blockcollide[3].CalculateNormals();

    loadingscreenamount += 5;

    if(!initialized)
      LoadingScreen(loadingscreenamount / loadingscreenamounttotal * 100);

    blocksimplecollide[0].
        load((unsigned char *)":Data:Models:lowsimplecollide1.solid");
    blocksimplecollide[0].Rotate(90, 0, 0);
    blocksimplecollide[0].Scale(.8, .8, .8);
    blocksimplecollide[0].CalculateNormals();

    blocksimplecollide[1].
        load((unsigned char *)":Data:Models:lowsimplecollide2.solid");
    blocksimplecollide[1].Rotate(90, 0, 0);
    blocksimplecollide[1].Scale(.8, .8, .8);
    blocksimplecollide[1].CalculateNormals();

    blocksimplecollide[2].
        load((unsigned char *)":Data:Models:lowsimplecollide3.solid");
    blocksimplecollide[2].Rotate(90, 0, 0);
    blocksimplecollide[2].Scale(.8, .8, .8);
    blocksimplecollide[2].CalculateNormals();

    blocksimplecollide[3].
        load((unsigned char *)":Data:Models:lowsimplecollide4.solid");
    blocksimplecollide[3].Rotate(90, 0, 0);
    blocksimplecollide[3].Scale(.8, .8, .8);
    blocksimplecollide[3].CalculateNormals();

    loadingscreenamount += 5;

    if(!initialized)
      LoadingScreen(loadingscreenamount / loadingscreenamounttotal * 100);

    blockocclude.load((unsigned char *)":Data:Models:blockocclude.solid");
    blockocclude.Rotate(90, 0, 0);
    blockocclude.Scale(.8, .8, .8);
    blockocclude.CalculateNormals();

    blocksimple.load((unsigned char *)":Data:Models:blocksimple.solid");
    blocksimple.Rotate(90, 0, 0);
    blocksimple.Scale(.8, 2, .8);
    blocksimple.CalculateNormals();

    street.load((unsigned char *)":Data:Models:streetsubdivided2.solid");
    street.Rotate(90, 0, 0);
    street.Scale(.01, .01, .01);
    street.CalculateNormals();

    Bigstreet = street;
    Bigstreet.Scale(10000, 10000, 10000);

    loadingscreenamount += 5;

    if(!initialized)
      LoadingScreen(loadingscreenamount / loadingscreenamounttotal * 100);

    path.load((unsigned char *)":Data:Models:path.solid");
    path.Rotate(90, 0, 0);
    path.Scale(.8, .8, .8);
    path.CalculateNormals();

    //Fix block radius

    for(int i = 0; i < 4; i++) {
      blocks[i].boundingspherecenter.x = 0;
      blocks[i].boundingspherecenter.z = 0;
      blocks[i].boundingsphereradius = 0;

      for(int x = 0; i < blocks[x].vertexNum; x++) {
        if(findDistancefast(blocks[i].boundingspherecenter, blocks[i].vertex[x])
           > blocks[i].boundingsphereradius) {
          blocks[i].boundingsphereradius =
              findDistancefast(blocks[i].boundingspherecenter,
                               blocks[i].vertex[x]);
        }
      }

      blocks[i].boundingsphereradius = sqrt(blocks[i].boundingsphereradius);
    }
  }
 
  //FIXME: why is the user specified value being overridden?
  mousesensitivity = 1;

  //init city block rotations
  for(int i = 0; i < num_blocks; i++) {
    for(int j = 0; j < num_blocks; j++) {
      cityrotation[i][j] = Random() % 4;
      citytype[i][j] = Random() % 4;

      citypeoplenum[i][j] = 0;

      if(citytype[i][j] < 0)
        citytype[i][j] = 0;

      if(citytype[i][j] > 3)
        citytype[i][j] = 3;
    }
  }

  if(!initialized) {

    //Load player model
    skeletonmodels[0].load((unsigned char *)":Data:Models:Head.solid");
    skeletonmodels[0].Rotate(90, 0, 0);
    skeletonmodels[0].Scale(.02, .02, .02);
    skeletonmodels[0].CalculateNormals();

    skeletonmodels[1].load((unsigned char *)":Data:Models:Chest.solid");
    skeletonmodels[1].Rotate(90, 0, 0);
    skeletonmodels[1].Scale(.02, .02, .02);
    skeletonmodels[1].CalculateNormals();

    skeletonmodels[2].load((unsigned char *)":Data:Models:Abdomen.solid");
    skeletonmodels[2].Rotate(90, 0, 0);
    skeletonmodels[2].Scale(.02, .02, .02);
    skeletonmodels[2].CalculateNormals();

    skeletonmodels[3].load((unsigned char *)":Data:Models:Upper arm.solid");
    skeletonmodels[3].Rotate(90, 0, 0);
    skeletonmodels[3].Scale(.02, .02, .02);
    skeletonmodels[3].CalculateNormals();

    skeletonmodels[4].load((unsigned char *)":Data:Models:Lower arm.solid");
    skeletonmodels[4].Rotate(90, 0, 0);
    skeletonmodels[4].Scale(.02, .02, .02);
    skeletonmodels[4].CalculateNormals();

    skeletonmodels[5].load((unsigned char *)":Data:Models:Hand.solid");
    skeletonmodels[5].Rotate(90, 0, 0);
    skeletonmodels[5].Scale(.02, .02, .02);
    skeletonmodels[5].CalculateNormals();

    skeletonmodels[6].load((unsigned char *)":Data:Models:Upper leg.solid");
    skeletonmodels[6].Rotate(90, 0, 0);
    skeletonmodels[6].Scale(.02, .02, .02);
    skeletonmodels[6].CalculateNormals();

    skeletonmodels[7].load((unsigned char *)":Data:Models:Lower leg.solid");
    skeletonmodels[7].Rotate(90, 0, 0);
    skeletonmodels[7].Scale(.02, .02, .02);
    skeletonmodels[7].CalculateNormals();

    skeletonmodels[8].load((unsigned char *)":Data:Models:Foot.solid");
    skeletonmodels[8].Rotate(90, 0, 0);
    skeletonmodels[8].Scale(.02, .02, .02);
    skeletonmodels[8].CalculateNormals();

    skeletonmodels[9].load((unsigned char *)":Data:Models:Shades.solid");
    skeletonmodels[9].Rotate(90, 0, 0);
    skeletonmodels[9].Scale(.02, .02, .02);
    skeletonmodels[9].CalculateNormals();

    //Load gun models
    loadingscreenamount += 5;

    if(!initialized)
      LoadingScreen(loadingscreenamount / loadingscreenamounttotal * 100);

    gunmodels[sniperriflemodel].
        load((unsigned char *)":Data:Models:sniperrifle.solid");
    gunmodels[sniperriflemodel].Rotate(0, 0, 90);
    gunmodels[sniperriflemodel].Scale(.001, .001, .001);
    gunmodels[sniperriflemodel].CalculateNormals();

    gunmodels[assaultriflemodel].
        load((unsigned char *)":Data:Models:assaultrifle.solid");
    gunmodels[assaultriflemodel].Rotate(0, 0, 90);
    gunmodels[assaultriflemodel].Scale(.01, .01, .01);
    gunmodels[assaultriflemodel].CalculateNormals();

    gunmodels[handgunbasemodel].
        load((unsigned char *)":Data:Models:Handgunbase.solid");
    gunmodels[handgunbasemodel].Rotate(0, 0, 90);
    gunmodels[handgunbasemodel].Rotate(180, 0, 0);
    gunmodels[handgunbasemodel].Scale(.014, .014, .014);
    gunmodels[handgunbasemodel].CalculateNormals();
    gunmodels[handgunbasemodel].MultColor(.6);

    loadingscreenamount += 5;

    if(!initialized)
      LoadingScreen(loadingscreenamount / loadingscreenamounttotal * 100);

    gunmodels[handgunslidemodel].
        load((unsigned char *)":Data:Models:Handgunslide.solid");
    gunmodels[handgunslidemodel].Rotate(0, 0, 90);
    gunmodels[handgunslidemodel].Rotate(180, 0, 0);
    gunmodels[handgunslidemodel].Scale(.014, .014, .014);
    gunmodels[handgunslidemodel].CalculateNormals();
    gunmodels[handgunslidemodel].MultColor(.6);

    gunmodels[handgun2basemodel].
        load((unsigned char *)":Data:Models:glockbase.solid");
    gunmodels[handgun2basemodel].Rotate(0, 0, 90);
    gunmodels[handgun2basemodel].Rotate(180, 0, 0);
    gunmodels[handgun2basemodel].Scale(.014, .014, .014);
    gunmodels[handgun2basemodel].CalculateNormals();
    gunmodels[handgun2basemodel].MultColor(.6);

    gunmodels[handgun2slidemodel].
        load((unsigned char *)":Data:Models:glockslide.solid");
    gunmodels[handgun2slidemodel].Rotate(0, 0, 90);
    gunmodels[handgun2slidemodel].Rotate(180, 0, 0);
    gunmodels[handgun2slidemodel].Scale(.014, .014, .014);
    gunmodels[handgun2slidemodel].CalculateNormals();
    gunmodels[handgun2slidemodel].MultColor(.6);

    loadingscreenamount += 5;

    if(!initialized)
      LoadingScreen(loadingscreenamount / loadingscreenamounttotal * 100);

    gunmodels[grenadebasemodel].
        load((unsigned char *)":Data:Models:grenadebase.solid");
    gunmodels[grenadebasemodel].Rotate(0, 0, 90);
    gunmodels[grenadebasemodel].Rotate(180, 0, 0);
    gunmodels[grenadebasemodel].Scale(.014, .014, .014);
    gunmodels[grenadebasemodel].CalculateNormals();

    gunmodels[grenadepinmodel].
        load((unsigned char *)":Data:Models:grenadepin.solid");
    gunmodels[grenadepinmodel].Rotate(0, 0, 90);
    gunmodels[grenadepinmodel].Rotate(180, 0, 0);
    gunmodels[grenadepinmodel].Scale(.014, .014, .014);
    gunmodels[grenadepinmodel].CalculateNormals();

    gunmodels[grenadespoonmodel].
        load((unsigned char *)":Data:Models:grenadespoon.solid");
    gunmodels[grenadespoonmodel].Rotate(0, 0, 90);
    gunmodels[grenadespoonmodel].Rotate(180, 0, 0);
    gunmodels[grenadespoonmodel].Scale(.014, .014, .014);
    gunmodels[grenadespoonmodel].CalculateNormals();

    gunmodels[knifemodel].load((unsigned char *)":Data:Models:Knife.solid");
    gunmodels[knifemodel].Rotate(0, 0, 90);
    gunmodels[knifemodel].Rotate(180, 0, 0);
    gunmodels[knifemodel].Scale(.014, .014, .014);
    gunmodels[knifemodel].CalculateNormals();

    gunmodels[shotgunmodel].load((unsigned char *)":Data:Models:shotgun.solid");
    gunmodels[shotgunmodel].Rotate(0, 0, 90);
    gunmodels[shotgunmodel].Scale(.001, .001, .001);
    gunmodels[shotgunmodel].CalculateNormals();
    gunmodels[shotgunmodel].MultColor(.6);
  }

  loadingscreenamount += 5;

  if(!initialized)
    LoadingScreen(loadingscreenamount / loadingscreenamounttotal * 100);

  //Setup costumes
  float headcolor[3];
  float footcolor[3];
  float handcolor[3];
  float topcolor[3];
  float bottomcolor[3];

  //Police
  headcolor[0] = (float)240 / 255;
  headcolor[1] = (float)183 / 255;
  headcolor[2] = (float)132 / 255;

  footcolor[0] = (float)119 / 255;
  footcolor[1] = (float)68 / 255;
  footcolor[2] = (float)18 / 255;

  handcolor[0] = (float)240 / 255;
  handcolor[1] = (float)183 / 255;
  handcolor[2] = (float)132 / 255;

  topcolor[0] = (float)14 / 255;
  topcolor[1] = (float)18 / 255;
  topcolor[2] = (float)195 / 255;

  bottomcolor[0] = (float)14 / 255;
  bottomcolor[1] = (float)18 / 255;
  bottomcolor[2] = (float)195 / 255;

  //Greenish skin if zombies

  if(type == zombie_type) {
    headcolor[0] = (float)223 / 255;
    headcolor[1] = (float)243 / 255;
    headcolor[2] = (float)197 / 255;

    handcolor[0] = (float)223 / 255;
    handcolor[1] = (float)243 / 255;
    handcolor[2] = (float)197 / 255;
  }

  costume[policecostume].headcolor[0] = headcolor[0];
  costume[policecostume].headcolor[1] = headcolor[1];
  costume[policecostume].headcolor[2] = headcolor[2];

  costume[policecostume].handcolor[0] = handcolor[0];
  costume[policecostume].handcolor[1] = handcolor[1];
  costume[policecostume].handcolor[2] = handcolor[2];

  costume[policecostume].chestcolor[0] = topcolor[0];
  costume[policecostume].chestcolor[1] = topcolor[1];
  costume[policecostume].chestcolor[2] = topcolor[2];

  costume[policecostume].abdomencolor[0] = topcolor[0];
  costume[policecostume].abdomencolor[1] = topcolor[1];
  costume[policecostume].abdomencolor[2] = topcolor[2];

  costume[policecostume].upperarmcolor[0] = topcolor[0];
  costume[policecostume].upperarmcolor[1] = topcolor[1];
  costume[policecostume].upperarmcolor[2] = topcolor[2];

  costume[policecostume].lowerarmcolor[0] = topcolor[0];
  costume[policecostume].lowerarmcolor[1] = topcolor[1];
  costume[policecostume].lowerarmcolor[2] = topcolor[2];

  costume[policecostume].upperlegcolor[0] = bottomcolor[0];
  costume[policecostume].upperlegcolor[1] = bottomcolor[1];
  costume[policecostume].upperlegcolor[2] = bottomcolor[2];

  costume[policecostume].lowerlegcolor[0] = bottomcolor[0];
  costume[policecostume].lowerlegcolor[1] = bottomcolor[1];
  costume[policecostume].lowerlegcolor[2] = bottomcolor[2];

  costume[policecostume].footcolor[0] = footcolor[0];
  costume[policecostume].footcolor[1] = footcolor[1];
  costume[policecostume].footcolor[2] = footcolor[2];

  //casual
  topcolor[0] = (float)14 / 255;
  topcolor[1] = (float)200 / 255;
  topcolor[2] = (float)30 / 255;

  bottomcolor[0] = (float)14 / 255;
  bottomcolor[1] = (float)18 / 255;
  bottomcolor[2] = (float)195 / 255;

  costume[casualcostumes].headcolor[0] = headcolor[0];
  costume[casualcostumes].headcolor[1] = headcolor[1];
  costume[casualcostumes].headcolor[2] = headcolor[2];

  costume[casualcostumes].handcolor[0] = handcolor[0];
  costume[casualcostumes].handcolor[1] = handcolor[1];
  costume[casualcostumes].handcolor[2] = handcolor[2];

  costume[casualcostumes].chestcolor[0] = topcolor[0];
  costume[casualcostumes].chestcolor[1] = topcolor[1];
  costume[casualcostumes].chestcolor[2] = topcolor[2];

  costume[casualcostumes].abdomencolor[0] = topcolor[0];
  costume[casualcostumes].abdomencolor[1] = topcolor[1];
  costume[casualcostumes].abdomencolor[2] = topcolor[2];

  costume[casualcostumes].upperarmcolor[0] = topcolor[0];
  costume[casualcostumes].upperarmcolor[1] = topcolor[1];
  costume[casualcostumes].upperarmcolor[2] = topcolor[2];

  costume[casualcostumes].lowerarmcolor[0] = handcolor[0];
  costume[casualcostumes].lowerarmcolor[1] = handcolor[1];
  costume[casualcostumes].lowerarmcolor[2] = handcolor[2];

  costume[casualcostumes].upperlegcolor[0] = bottomcolor[0];
  costume[casualcostumes].upperlegcolor[1] = bottomcolor[1];
  costume[casualcostumes].upperlegcolor[2] = bottomcolor[2];

  costume[casualcostumes].lowerlegcolor[0] = bottomcolor[0];
  costume[casualcostumes].lowerlegcolor[1] = bottomcolor[1];
  costume[casualcostumes].lowerlegcolor[2] = bottomcolor[2];

  costume[casualcostumes].footcolor[0] = footcolor[0];
  costume[casualcostumes].footcolor[1] = footcolor[1];
  costume[casualcostumes].footcolor[2] = footcolor[2];

  //casual 2

  topcolor[0] = (float)140 / 255;
  topcolor[1] = (float)55 / 255;
  topcolor[2] = (float)4 / 255;

  bottomcolor[0] = (float)14 / 255;
  bottomcolor[1] = (float)18 / 255;
  bottomcolor[2] = (float)135 / 255;

  costume[casualcostumes + 1].headcolor[0] = headcolor[0];
  costume[casualcostumes + 1].headcolor[1] = headcolor[1];
  costume[casualcostumes + 1].headcolor[2] = headcolor[2];

  costume[casualcostumes + 1].handcolor[0] = handcolor[0];
  costume[casualcostumes + 1].handcolor[1] = handcolor[1];
  costume[casualcostumes + 1].handcolor[2] = handcolor[2];

  costume[casualcostumes + 1].chestcolor[0] = topcolor[0];
  costume[casualcostumes + 1].chestcolor[1] = topcolor[1];
  costume[casualcostumes + 1].chestcolor[2] = topcolor[2];

  costume[casualcostumes + 1].abdomencolor[0] = topcolor[0];
  costume[casualcostumes + 1].abdomencolor[1] = topcolor[1];
  costume[casualcostumes + 1].abdomencolor[2] = topcolor[2];

  costume[casualcostumes + 1].upperarmcolor[0] = topcolor[0];
  costume[casualcostumes + 1].upperarmcolor[1] = topcolor[1];
  costume[casualcostumes + 1].upperarmcolor[2] = topcolor[2];

  costume[casualcostumes + 1].lowerarmcolor[0] = topcolor[0];
  costume[casualcostumes + 1].lowerarmcolor[1] = topcolor[1];
  costume[casualcostumes + 1].lowerarmcolor[2] = topcolor[2];

  costume[casualcostumes + 1].upperlegcolor[0] = bottomcolor[0];
  costume[casualcostumes + 1].upperlegcolor[1] = bottomcolor[1];
  costume[casualcostumes + 1].upperlegcolor[2] = bottomcolor[2];

  costume[casualcostumes + 1].lowerlegcolor[0] = bottomcolor[0];
  costume[casualcostumes + 1].lowerlegcolor[1] = bottomcolor[1];
  costume[casualcostumes + 1].lowerlegcolor[2] = bottomcolor[2];

  costume[casualcostumes + 1].footcolor[0] = footcolor[0];
  costume[casualcostumes + 1].footcolor[1] = footcolor[1];
  costume[casualcostumes + 1].footcolor[2] = footcolor[2];

  //casual 3
  topcolor[0] = (float)134 / 255;
  topcolor[1] = (float)80 / 255;
  topcolor[2] = (float)3 / 255;

  bottomcolor[0] = (float)30 / 255;
  bottomcolor[1] = (float)30 / 255;
  bottomcolor[2] = (float)30 / 255;

  footcolor[0] = (float)20 / 255;
  footcolor[1] = (float)20 / 255;
  footcolor[2] = (float)20 / 255;

  costume[casualcostumes + 2].headcolor[0] = headcolor[0];
  costume[casualcostumes + 2].headcolor[1] = headcolor[1];
  costume[casualcostumes + 2].headcolor[2] = headcolor[2];

  costume[casualcostumes + 2].handcolor[0] = handcolor[0];
  costume[casualcostumes + 2].handcolor[1] = handcolor[1];
  costume[casualcostumes + 2].handcolor[2] = handcolor[2];

  costume[casualcostumes + 2].chestcolor[0] = topcolor[0];
  costume[casualcostumes + 2].chestcolor[1] = topcolor[1];
  costume[casualcostumes + 2].chestcolor[2] = topcolor[2];

  costume[casualcostumes + 2].abdomencolor[0] = topcolor[0];
  costume[casualcostumes + 2].abdomencolor[1] = topcolor[1];
  costume[casualcostumes + 2].abdomencolor[2] = topcolor[2];

  costume[casualcostumes + 2].upperarmcolor[0] = topcolor[0];
  costume[casualcostumes + 2].upperarmcolor[1] = topcolor[1];
  costume[casualcostumes + 2].upperarmcolor[2] = topcolor[2];

  costume[casualcostumes + 2].lowerarmcolor[0] = topcolor[0];
  costume[casualcostumes + 2].lowerarmcolor[1] = topcolor[1];
  costume[casualcostumes + 2].lowerarmcolor[2] = topcolor[2];

  costume[casualcostumes + 2].upperlegcolor[0] = bottomcolor[0];
  costume[casualcostumes + 2].upperlegcolor[1] = bottomcolor[1];
  costume[casualcostumes + 2].upperlegcolor[2] = bottomcolor[2];

  costume[casualcostumes + 2].lowerlegcolor[0] = bottomcolor[0];
  costume[casualcostumes + 2].lowerlegcolor[1] = bottomcolor[1];
  costume[casualcostumes + 2].lowerlegcolor[2] = bottomcolor[2];

  costume[casualcostumes + 2].footcolor[0] = footcolor[0];
  costume[casualcostumes + 2].footcolor[1] = footcolor[1];
  costume[casualcostumes + 2].footcolor[2] = footcolor[2];

  //casual 4

  topcolor[0] = (float)228 / 255;
  topcolor[1] = (float)220 / 255;
  topcolor[2] = (float)0 / 255;

  bottomcolor[0] = (float)20 / 255;
  bottomcolor[1] = (float)20 / 255;
  bottomcolor[2] = (float)20 / 255;

  footcolor[0] = (float)119 / 255;
  footcolor[1] = (float)68 / 255;
  footcolor[2] = (float)18 / 255;

  costume[casualcostumes + 3].headcolor[0] = headcolor[0];
  costume[casualcostumes + 3].headcolor[1] = headcolor[1];
  costume[casualcostumes + 3].headcolor[2] = headcolor[2];

  costume[casualcostumes + 3].handcolor[0] = handcolor[0];
  costume[casualcostumes + 3].handcolor[1] = handcolor[1];
  costume[casualcostumes + 3].handcolor[2] = handcolor[2];

  costume[casualcostumes + 3].chestcolor[0] = topcolor[0];
  costume[casualcostumes + 3].chestcolor[1] = topcolor[1];
  costume[casualcostumes + 3].chestcolor[2] = topcolor[2];

  costume[casualcostumes + 3].abdomencolor[0] = topcolor[0];
  costume[casualcostumes + 3].abdomencolor[1] = topcolor[1];
  costume[casualcostumes + 3].abdomencolor[2] = topcolor[2];

  costume[casualcostumes + 3].upperarmcolor[0] = topcolor[0];
  costume[casualcostumes + 3].upperarmcolor[1] = topcolor[1];
  costume[casualcostumes + 3].upperarmcolor[2] = topcolor[2];

  costume[casualcostumes + 3].lowerarmcolor[0] = handcolor[0];
  costume[casualcostumes + 3].lowerarmcolor[1] = handcolor[1];
  costume[casualcostumes + 3].lowerarmcolor[2] = handcolor[2];

  costume[casualcostumes + 3].upperlegcolor[0] = bottomcolor[0];
  costume[casualcostumes + 3].upperlegcolor[1] = bottomcolor[1];
  costume[casualcostumes + 3].upperlegcolor[2] = bottomcolor[2];

  costume[casualcostumes + 3].lowerlegcolor[0] = bottomcolor[0];
  costume[casualcostumes + 3].lowerlegcolor[1] = bottomcolor[1];
  costume[casualcostumes + 3].lowerlegcolor[2] = bottomcolor[2];

  costume[casualcostumes + 3].footcolor[0] = footcolor[0];
  costume[casualcostumes + 3].footcolor[1] = footcolor[1];
  costume[casualcostumes + 3].footcolor[2] = footcolor[2];

  if(!initialized) {

    //vip
    topcolor[0] = (float)235 / 255;
    topcolor[1] = (float)235 / 255;
    topcolor[2] = (float)235 / 255;

    bottomcolor[0] = (float)200 / 255;
    bottomcolor[1] = (float)200 / 255;
    bottomcolor[2] = (float)200 / 255;

    footcolor[0] = (float)119 / 255;
    footcolor[1] = (float)68 / 255;
    footcolor[2] = (float)18 / 255;

    headcolor[0] = (float)240 / 255;
    headcolor[1] = (float)183 / 255;
    headcolor[2] = (float)132 / 255;

    footcolor[0] = (float)119 / 255;
    footcolor[1] = (float)68 / 255;
    footcolor[2] = (float)18 / 255;

    handcolor[0] = (float)240 / 255;
    handcolor[1] = (float)183 / 255;
    handcolor[2] = (float)132 / 255;

    costume[vipcostume].headcolor[0] = headcolor[0];
    costume[vipcostume].headcolor[1] = headcolor[1];
    costume[vipcostume].headcolor[2] = headcolor[2];

    costume[vipcostume].handcolor[0] = handcolor[0];
    costume[vipcostume].handcolor[1] = handcolor[1];
    costume[vipcostume].handcolor[2] = handcolor[2];

    costume[vipcostume].chestcolor[0] = topcolor[0];
    costume[vipcostume].chestcolor[1] = topcolor[1];
    costume[vipcostume].chestcolor[2] = topcolor[2];

    costume[vipcostume].abdomencolor[0] = topcolor[0];
    costume[vipcostume].abdomencolor[1] = topcolor[1];
    costume[vipcostume].abdomencolor[2] = topcolor[2];

    costume[vipcostume].upperarmcolor[0] = topcolor[0];
    costume[vipcostume].upperarmcolor[1] = topcolor[1];
    costume[vipcostume].upperarmcolor[2] = topcolor[2];

    costume[vipcostume].lowerarmcolor[0] = topcolor[0];
    costume[vipcostume].lowerarmcolor[1] = topcolor[1];
    costume[vipcostume].lowerarmcolor[2] = topcolor[2];

    costume[vipcostume].upperlegcolor[0] = bottomcolor[0];
    costume[vipcostume].upperlegcolor[1] = bottomcolor[1];
    costume[vipcostume].upperlegcolor[2] = bottomcolor[2];

    costume[vipcostume].lowerlegcolor[0] = bottomcolor[0];
    costume[vipcostume].lowerlegcolor[1] = bottomcolor[1];
    costume[vipcostume].lowerlegcolor[2] = bottomcolor[2];

    costume[vipcostume].footcolor[0] = footcolor[0];
    costume[vipcostume].footcolor[1] = footcolor[1];
    costume[vipcostume].footcolor[2] = footcolor[2];

    //Bodyguard
    topcolor[0] = (float)50 / 255;
    topcolor[1] = (float)50 / 255;
    topcolor[2] = (float)50 / 255;

    bottomcolor[0] = (float)30 / 255;
    bottomcolor[1] = (float)30 / 255;
    bottomcolor[2] = (float)30 / 255;

    footcolor[0] = (float)20 / 255;
    footcolor[1] = (float)20 / 255;
    footcolor[2] = (float)20 / 255;

    costume[bodyguardcostume].headcolor[0] = headcolor[0];
    costume[bodyguardcostume].headcolor[1] = headcolor[1];
    costume[bodyguardcostume].headcolor[2] = headcolor[2];

    costume[bodyguardcostume].handcolor[0] = handcolor[0];
    costume[bodyguardcostume].handcolor[1] = handcolor[1];
    costume[bodyguardcostume].handcolor[2] = handcolor[2];

    costume[bodyguardcostume].chestcolor[0] = topcolor[0];
    costume[bodyguardcostume].chestcolor[1] = topcolor[1];
    costume[bodyguardcostume].chestcolor[2] = topcolor[2];

    costume[bodyguardcostume].abdomencolor[0] = topcolor[0];
    costume[bodyguardcostume].abdomencolor[1] = topcolor[1];
    costume[bodyguardcostume].abdomencolor[2] = topcolor[2];

    costume[bodyguardcostume].upperarmcolor[0] = topcolor[0];
    costume[bodyguardcostume].upperarmcolor[1] = topcolor[1];
    costume[bodyguardcostume].upperarmcolor[2] = topcolor[2];

    costume[bodyguardcostume].lowerarmcolor[0] = topcolor[0];
    costume[bodyguardcostume].lowerarmcolor[1] = topcolor[1];
    costume[bodyguardcostume].lowerarmcolor[2] = topcolor[2];

    costume[bodyguardcostume].upperlegcolor[0] = bottomcolor[0];
    costume[bodyguardcostume].upperlegcolor[1] = bottomcolor[1];
    costume[bodyguardcostume].upperlegcolor[2] = bottomcolor[2];

    costume[bodyguardcostume].lowerlegcolor[0] = bottomcolor[0];
    costume[bodyguardcostume].lowerlegcolor[1] = bottomcolor[1];
    costume[bodyguardcostume].lowerlegcolor[2] = bottomcolor[2];

    costume[bodyguardcostume].footcolor[0] = footcolor[0];
    costume[bodyguardcostume].footcolor[1] = footcolor[1];
    costume[bodyguardcostume].footcolor[2] = footcolor[2];

    //Load animations

    loadingscreenamount += 5;

    if(!initialized)
      LoadingScreen(loadingscreenamount / loadingscreenamounttotal * 100);

    testskeleton.Load((char *)":Data:Skeleton:Basic Figure");
    animation[idleanim].Load((char *)":Data:Animations:Breathe");
    animation[joganim].Load((char *)":Data:Animations:Run");
    animation[pistolaimanim].Load((char *)":Data:Animations:PistolAim");

    loadingscreenamount += 5;

    if(!initialized)
      LoadingScreen(loadingscreenamount / loadingscreenamounttotal * 100);

    animation[walkanim].Load((char *)":Data:Animations:Walk");
    animation[rifleholdanim].Load((char *)":Data:Animations:Riflehold");
    animation[rifleaimanim].Load((char *)":Data:Animations:Rifleaim");
    animation[assaultrifleaimanim].
        Load((char *)":Data:Animations:AssaultRifleaim");

    animation[crouchanim].Load((char *)":Data:Animations:Crouch");

    loadingscreenamount += 5;

    if(!initialized)
      LoadingScreen(loadingscreenamount / loadingscreenamounttotal * 100);

    animation[headpainanim].Load((char *)":Data:Animations:Headshot");
    animation[chestpainanim].Load((char *)":Data:Animations:Chestshot");
    animation[stomachpainanim].Load((char *)":Data:Animations:Stomachshot");
    animation[rightarmpainanim].Load((char *)":Data:Animations:Rightarmshot");
    animation[leftarmpainanim].Load((char *)":Data:Animations:Leftarmshot");

    loadingscreenamount += 5;

    if(!initialized)
      LoadingScreen(loadingscreenamount / loadingscreenamounttotal * 100);

    animation[rightlegpainanim].Load((char *)":Data:Animations:Rightlegshot");
    animation[leftlegpainanim].Load((char *)":Data:Animations:Leftlegshot");
    animation[riflehitanim].Load((char *)":Data:Animations:Riflehit");
    animation[grenadeaimanim].Load((char *)":Data:Animations:grenadeaim");
    animation[grenadechargeanim].Load((char *)":Data:Animations:grenadecharge");

    loadingscreenamount += 5;

    if(!initialized)
      LoadingScreen(loadingscreenamount / loadingscreenamounttotal * 100);

    animation[grenadethrowanim].Load((char *)":Data:Animations:grenadethrow");
    animation[zombieeatanim].Load((char *)":Data:Animations:Zombiemunch");
    animation[zombiejoganim].Load((char *)":Data:Animations:ZombieRun");
    animation[zombiewalkanim].Load((char *)":Data:Animations:Zombiewalk");

    loadingscreenamount += 5;

    if(!initialized)
      LoadingScreen(loadingscreenamount / loadingscreenamounttotal * 100);

    animation[getupfrontanim].Load((char *)":Data:Animations:Getupfromfront");
    animation[getupbackanim].Load((char *)":Data:Animations:Getupfromback",
                                  180);
    animation[diveanim].Load((char *)":Data:Animations:Dive");
    animation[throwanim].Load((char *)":Data:Animations:Aikidothrow");
    animation[thrownanim].Load((char *)":Data:Animations:Aikidothrown");
  }
  //Setup people
  for(int i = 0; i < max_people; i++) {
    if(i == 0) {
      person[i].whichcostume = bodyguardcostume;
    }

    if(i > 1) {
      person[i].whichcostume = casualcostumes + abs(Random()) % numcasual;
    }
    //person[i].firstlongdead=0;
    person[i].dead = 0;
    person[i].health = 100;
    person[i].skeleton.free = 0;
    person[i].weapon.ammo = 0;
    person[i].velocity = 0;

    //Load skeleton structure
    if(!initialized)
      person[i].skeleton.Load((char *)":Data:Skeleton:Basic Figure");

    if(i % 5 == 0) {
      loadingscreenamount += 5;
      if(!initialized)
        LoadingScreen(loadingscreenamount / loadingscreenamounttotal * 100);
    }
  }

  loadingscreenamount += 5;

  if(!initialized)
    LoadingScreen(loadingscreenamount / loadingscreenamounttotal * 100);

  if(initialized)
    person[0].skeleton.Load((char *)":Data:Skeleton:Basic Figure");

  person[0].attackframe = -1;

  spawndelay = 0;

  fog.SetFog(fog.fogcolor.r, fog.fogcolor.g, fog.fogcolor.b, 0,
             environment.viewdistance * .8, .1);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //light
  GLfloat LightAmbient[] = { .3, .3, .3, 1.0f };
  GLfloat LightDiffuse[] = { 1, 1, 1, 1.0f };

  glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);

  glEnable(GL_LIGHT0);

  loadingscreenamount += 5;

  //Load some textures
  if(!initialized) {
    LoadingScreen(loadingscreenamount / loadingscreenamounttotal * 100);

    LoadPersonSpriteTexture(":Data:Textures:Personsprite.tga",
                            &personspritetextureptr);

    LoadPersonSpriteTexture(":Data:Textures:DeadPersonsprite.tga",
                            &deadpersonspritetextureptr);

    LoadPersonSpriteTexture(":Data:Textures:Scope.tga", &scopetextureptr);
    LoadPersonSpriteTexture(":Data:Textures:Flare.tga", &flaretextureptr);

    //used by the mainmenu
    sprites.LoadSmokeTexture(":Data:Textures:Smoke.tga");
    sprites.LoadFlareTexture(":Data:Textures:HitFlash.tga");

    sprites.LoadMuzzleFlareTexture(":Data:Textures:MuzzleFlash.tga");
    sprites.LoadBloodTexture(":Data:Textures:Blood.tga");
    sprites.LoadRainTexture(":Data:Textures:rain.tga");
    sprites.LoadSnowTexture(":Data:Textures:snow.tga");

    decals.LoadBulletHoleTexture(":Data:Textures:BulletHole.tga");

    decals.LoadCraterTexture(":Data:Textures:Crater.tga");

    decals.LoadBloodTexture(":Data:Textures:Blood:Blood1.tga", 0);
    decals.LoadBloodTexture(":Data:Textures:Blood:Blood2.tga", 1);
    decals.LoadBloodTexture(":Data:Textures:Blood:Blood3.tga", 2);
    decals.LoadBloodTexture(":Data:Textures:Blood:Blood4.tga", 3);
    decals.LoadBloodTexture(":Data:Textures:Blood:Blood5.tga", 4);
    decals.LoadBloodTexture(":Data:Textures:Blood:Blood6.tga", 5);
    decals.LoadBloodTexture(":Data:Textures:Blood:Blood7.tga", 6);
    decals.LoadBloodTexture(":Data:Textures:Blood:Blood8.tga", 7);
    decals.LoadBloodTexture(":Data:Textures:Blood:Blood9.tga", 8);
    decals.LoadBloodTexture(":Data:Textures:Blood:Blood10.tga", 9);
    decals.LoadBloodTexture(":Data:Textures:Blood:Blood11.tga", 10);
  }
  //Setup clip plane equation
  eqn[0] = 0;
  eqn[1] = 1;
  eqn[2] = 0;
  eqn[3] = 0;

  glClearColor(fog.fogcolor.r, fog.fogcolor.g, fog.fogcolor.b, 1);

  if(!initialized)
    InitMouse();

  //Draw city one frame to fix evil menu bug
  //if(!initialized)
  //  mainmenu = 2;

  if(!initialized) {
    LoadingScreen(loadingscreenamount / loadingscreenamounttotal * 100);

    flashamount = 1;

    flashr = 1;
    flashg = 1;
    flashb = 1;

    alSourcePlay(gSourceID[soulinsound]);
  }

  initialized = 1;

  loadingscreenamount += 5;

  decals.howmanydecals = 0;
  sprites.howmanysprites = 0;

  losedelay = 1;

  oldscore = score;

  //done loading, now get into the game...
  if(config.mainmenuness && !gameinprogress) {
    state = MAINMENU;
  } else {
    gameinprogress = 1;
    state = GAME;
  }
}

/* initialize all GUI elements 
 * - mainmenu
 * - in game
 * - help
 */
void Game::InitGUI()
{
  font.LoadFontTexture(":Data:Textures:Font.tga");
  font.BuildFont();

  Window *mainmenu = new Window();
  mainmenu->setName("mainmenu");

  Button *button = new Button();
  button->setName("button1");
  button->setButton(0.1875, 0.49, 0.6875, 0.145);
  mainmenu->attach(button);

  button = new Button();
  button->setName("button2");
  button->setButton(0.1875, 0.233, 0.6875, 0.195);
  mainmenu->attach(button);

  Label *label = new Label();
  label->setName("title");
  label->setFont(1, font.base, font.FontTexture);
  label->setLabel(0.156, 0.365, 2 * (screenheight / 480));
  label->setText("Black Shades");
  mainmenu->attach(label);

  Image *image = new Image();
  image->setName("title_image1");
  image->setDim(0.094, 0.698, 0.781, 0.146);
  image->setImage(sprites.smoketextureptr, 0);
  label->attach(image);

  image = new Image();
  image->setName("title_image2");
  image->setDim(0.094, 0.698, 0.828, 0.1);
  image->setImage(flaretextureptr, 0);
  label->attach(image);

  label = new Label();
  label->setName("game");
  label->setFont(1, font.base, font.FontTexture);
  label->setLabel(0.308, 0.35, 1.5);
  mainmenu->attach(label);

  image = new Image();
  image->setName("game_image1");
  image->setDim(0.1875, 0.49, 0.688, 0.146);
  image->setImage(sprites.smoketextureptr, 0);
  image->setImage(flaretextureptr, 1);
  image->setColor(Color(1.0, 0.0, 0.0, 1.0));
  label->attach(image);

  image = new Image();
  image->setName("game_image2");
  image->setDim(0.1875, 0.49, 0.719, 0.1);
  image->setImage(sprites.smoketextureptr, 0);
  image->setImage(flaretextureptr, 1);
  image->setColor(Color(1.0, 0.0, 0.0, 1.0));
  label->attach(image);

  label = new Label();
  label->setName("quit");
  label->setFont(1, font.base, font.FontTexture);
  label->setLabel(0.308, 0.181, 1.5);
  mainmenu->attach(label);

  image = new Image();
  image->setName("quit_image1");
  image->setDim(0.1875, 0.233, 0.688, 0.146);
  image->setImage(sprites.smoketextureptr, 0);
  image->setImage(flaretextureptr, 1);
  image->setColor(Color(1.0, 0.0, 0.0, 1.0));
  label->attach(image);

  image = new Image();
  image->setName("quit_image2");
  image->setDim(0.1875, 0.233, 0.719, 0.1);
  image->setImage(sprites.smoketextureptr, 0);
  image->setImage(flaretextureptr, 1);
  image->setColor(Color(1.0, 0.0, 0.0, 1.0));
  label->attach(image);

  WindowMgr::getInstance().getRoot()->attach(mainmenu);

  ///TODO: Add sub windows to easily not draw certain windows
  Window *ingame = new Window();
  ingame->setName("ingame");

  label = new Label();
  label->setName("weapon");
  label->setFont(1, font.base, font.FontTexture);
  label->setColor(Color(1.0, 1.0, 1.0, 1.0));
  label->setLabel(0.016, 0.1875, 0.8);
  ingame->attach(label);

  label = new Label();
  label->setName("magazines");
  label->setFont(0, font.base, font.FontTexture);
  label->setColor(Color(0.0, 1.0, 0.0, 1.0));
  label->setLabel(0.016, 0.140, 0.8);
  ingame->attach(label);

  label = new Label();
  label->setName("magazines_num");
  label->setFont(1, font.base, font.FontTexture);
  label->setColor(Color(1.0, 1.0, 1.0, 1.0));
  label->setLabel(0.016, 0.085, 1.0);
  ingame->attach(label);

  label = new Label();
  label->setName("ammo");
  label->setFont(0, font.base, font.FontTexture);
  label->setColor(Color(0.0, 1.0, 0.0, 1.0));
  label->setLabel(0.016, 0.060, 0.8);
  ingame->attach(label);

  label = new Label();
  label->setName("ammo_num");
  label->setFont(1, font.base, font.FontTexture);
  label->setColor(Color(1.0, 1.0, 1.0, 1.0));
  label->setLabel(0.016, 0.020, 1.0);
  ingame->attach(label);

  label = new Label();
  label->setName("score");
  label->setFont(0, font.base, font.FontTexture);
  label->setColor(Color(1.0, 1.0, 0.0, 1.0));
  label->setLabel(1.133, 1.198, 0.8);
  label->setText("Score");
  ingame->attach(label);

  label = new Label();
  label->setName("score_val");
  label->setFont(1, font.base, font.FontTexture);
  label->setColor(Color(1.0, 1.0, 1.0, 1.0));
  label->setLabel(0.906, 0.917, 1.0);
  ingame->attach(label);

  label = new Label();
  label->setName("time");
  label->setFont(0, font.base, font.FontTexture);
  label->setColor(Color(1.0, 1.0, 0.0, 1.0));
  label->setLabel(0.07, 1.198, 0.8);
  label->setText("Time Remaining");
  ingame->attach(label);

  label = new Label();
  label->setName("time_val");
  label->setFont(1, font.base, font.FontTexture);
  label->setColor(Color(1.0, 1.0, 1.0, 1.0));
  label->setLabel(0.1, 0.917, 1.0);
  ingame->attach(label);

  label = new Label();
  label->setName("help_me");
  label->setFont(1, font.base, font.FontTexture);
  label->setColor(Color(1.0, 1.0, 1.0, 1.0));
  label->setLabel(0.94, 0.02, 0.9);
  label->setText("Help = F1");
  ingame->attach(label);

  label = new Label();
  label->setName("mission");
  label->setFont(1, font.base, font.FontTexture);
  label->setColor(Color(1.0, 1.0, 1.0, 1.0));
  label->setLabel(0.94, 0.12, 0.9);
  ingame->attach(label);

  WindowMgr::getInstance().getRoot()->attach(ingame);

  Window *help_screen = new Window();
  help_screen->setName("helpscreen");

  label = new Label();
  label->setName("controls");
  label->setFont(1, font.base, font.FontTexture);
  label->setLabel(0.01, 1.04, 0.9);
  label->setColor(Color(0.5, 0.5, 0.5, 1.0));
  label->setText("Controls");
  help_screen->attach(label);

  label = new Label();
  label->setName("move");
  label->setFont(1, font.base, font.FontTexture);
  label->setLabel(0.02, 1.12, 0.8);
  label->setColor(Color(0.5, 0.5, 0.5, 1.0));
  label->setText("WASD = walk");
  help_screen->attach(label);

  label = new Label();
  label->setName("run");
  label->setFont(1, font.base, font.FontTexture);
  label->setLabel(0.02, 1.08, 0.8);
  label->setColor(Color(0.5, 0.5, 0.5, 1.0));
  label->setText("shift = run");
  help_screen->attach(label);

  label = new Label();
  label->setName("crouch");
  label->setFont(1, font.base, font.FontTexture);
  label->setLabel(0.02, 1.04, 0.8);
  label->setColor(Color(0.5, 0.5, 0.5, 1.0));
  label->setText("control = crouch/zoom");
  help_screen->attach(label);

  label = new Label();
  label->setName("click");
  label->setFont(1, font.base, font.FontTexture);
  label->setLabel(0.02, 1.00, 0.8);
  label->setColor(Color(0.5, 0.5, 0.5, 1.0));
  label->setText("click = fire (while aiming) or smash (while running) or");
  help_screen->attach(label);

  label = new Label();
  label->setName("click");
  label->setFont(1, font.base, font.FontTexture);
  label->setLabel(0.02, 0.96, 0.8);
  label->setColor(Color(0.5, 0.5, 0.5, 1.0));
  label->setText("pick up gun (crouching, not aim) or disarm (not aim)");
  help_screen->attach(label);

  label = new Label();
  label->setName("aim");
  label->setFont(1, font.base, font.FontTexture);
  label->setLabel(0.02, 0.92, 0.8);
  label->setColor(Color(0.5, 0.5, 0.5, 1.0));
  label->setText("q = aim or un-aim (important for picking up guns)");
  help_screen->attach(label);

  label = new Label();
  label->setName("reload");
  label->setFont(1, font.base, font.FontTexture);
  label->setLabel(0.02, 0.88, 0.8);
  label->setColor(Color(0.5, 0.5, 0.5, 1.0));
  label->setText("r = reload");
  help_screen->attach(label);

  label = new Label();
  label->setName("psychic");
  label->setFont(1, font.base, font.FontTexture);
  label->setLabel(0.02, 0.84, 0.8);
  label->setColor(Color(0.5, 0.5, 0.5, 1.0));
  label->setText("e = psychic aim");
  help_screen->attach(label);

  label = new Label();
  label->setName("soul");
  label->setFont(1, font.base, font.FontTexture);
  label->setLabel(0.02, 0.80, 0.8);
  label->setColor(Color(0.5, 0.5, 0.5, 1.0));
  label->setText("z = toggle soul release");
  help_screen->attach(label);

  label = new Label();
  label->setName("dive");
  label->setFont(1, font.base, font.FontTexture);
  label->setLabel(0.02, 0.76, 0.8);
  label->setColor(Color(0.5, 0.5, 0.5, 1.0));
  label->setText("space = dive (while running forwards)");
  help_screen->attach(label);

  label = new Label();
  label->setName("3rd");
  label->setFont(1, font.base, font.FontTexture);
  label->setLabel(0.02, 0.72, 0.8);
  label->setColor(Color(0.5, 0.5, 0.5, 1.0));
  label->setText("tab = 3rd person toggle");
  help_screen->attach(label);

  label = new Label();
  label->setName("laser");
  label->setFont(1, font.base, font.FontTexture);
  label->setLabel(0.02, 0.68, 0.8);
  label->setColor(Color(0.5, 0.5, 0.5, 1.0));
  label->setText("l = lasersight");
  help_screen->attach(label);

  label = new Label();
  label->setName("screenshot");
  label->setFont(1, font.base, font.FontTexture);
  label->setLabel(0.02, 0.64, 0.8);
  label->setColor(Color(0.5, 0.5, 0.5, 1.0));
  label->setText("F12 = screenshot");
  help_screen->attach(label);

  WindowMgr::getInstance().getRoot()->attach(help_screen);
}

void Game::Dispose()
{
  //Delete sound sources
  alDeleteSources(100, gSourceID);

  //FIXME: dunno if window deleting is done correctly
  //delete WindowMgr::getInstance().getRoot();
}


