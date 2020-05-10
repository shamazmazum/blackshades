/*
 * GameLoop.cpp
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

#include "SDL_funcs.h"
#include "Game.h"

extern unsigned int gSourceID[100];
extern unsigned int gSampleSet[100];

extern Camera camera;
extern Fog fog;
extern Config config;
extern Environment environment;

extern double multiplier;
extern int slomo;

enum game_state prev_state = INIT;

void Game::HandleKeyDown(char theChar)
{
  XYZ facing;
  bool debug = config.debug;

  if(state == GAME) {

    switch (theChar) {

      case 'l':
        if(!lasersight == 1) {
          lasersight = 1;
        } else {
          lasersight = 0;
        }

        break;

      case 'k':
        if(debug)
          timeremaining = 0;

        break;

      case 'b':

        if(debug) {

          alSourcePlay(gSourceID[soulinsound]);

          if(!slomo)
            slomo = 1;
          else
            slomo = 0;
        }

        if(slomo) {
          alSourcef(gSourceID[knifesong], AL_PITCH, (ALfloat) (.5));
          alSourcef(gSourceID[shootsong], AL_PITCH, (ALfloat) (.5));
          alSourcef(gSourceID[zombiesong], AL_PITCH, (ALfloat) (.5));
        } else {
          alSourcef(gSourceID[knifesong], AL_PITCH, (ALfloat) (1));
          alSourcef(gSourceID[shootsong], AL_PITCH, (ALfloat) (1));
          alSourcef(gSourceID[zombiesong], AL_PITCH, (ALfloat) (1));
        }

        break;

      case 'B':

        if(debug) {
          alSourcePlay(gSourceID[soulinsound]);
          paused = 1 - paused;
        }

        break;

      case 'f':

        if(debug) {
          alSourcePlay(gSourceID[souloutsound]);

          //Facing
          facing = 0;
          facing.z = -1;
          facing = DoRotation(facing, -camera.rotation2, 0, 0);
          facing = DoRotation(facing, 0, 0 - camera.rotation, 0);

          for(int i = 1; i < numpeople; i++) {

            if(person[i].skeleton.free != 1) {

              if(findDistancefast
                 (person[i].playercoords, person[0].playercoords) < 1000) {

                person[i].skeleton.free = 1;
                person[i].longdead = 1;

                for(int j = 0; j < person[i].skeleton.num_joints; j++) {

                  person[i].skeleton.joints[j].position =
                      DoRotation(person[i].skeleton.joints[j].position, 0,
                                 person[i].playerrotation, 0);

                  person[i].skeleton.joints[j].position +=
                      person[i].playercoords;

                  person[i].skeleton.joints[j].realoldposition =
                      person[i].skeleton.joints[j].position;

                  person[i].skeleton.joints[j].velocity =
                      DoRotation(person[i].skeleton.joints[j].velocity, 0,
                                 person[i].playerrotation, 0);

                  person[i].skeleton.joints[j].velocity += person[i].velocity;

                  person[i].skeleton.joints[j].velocity += facing * 50;

                  person[i].skeleton.joints[j].velocity.x +=
                      abs(Random() % 20) - 10;

                  person[i].skeleton.joints[j].velocity.y +=
                      abs(Random() % 20) - 10;

                  person[i].skeleton.joints[j].velocity.z +=
                      abs(Random() % 20) - 10;

                }
              }
            }
          }
        }

        break;

      case 'X':

        if(debug) {

          if(person[0].grenphase == 0) {
            person[0].weapon.ammo = -1;
            person[0].weapon.type++;
            person[0].grenphase = 0;
            person[0].reloads[person[0].weapon.type] = 3;

            if(person[0].weapon.type > 7)
              person[0].weapon.type = 0;
          }
        }

        break;
    }
  }
}

void Game::EventLoop(void)
{
  unsigned char theKeyMap[16];

  ///Frame limiting
  maxfps = 90;
  int interval = 1000/maxfps, sleep;
  float last_calc = SDL_GetTicks();

  float sps = 90.0; //needed?
  float sps_interval = 1000/sps;
  float last_tick = SDL_GetTicks();
  float diff_tick = 0;
  int cur_tick;

  GLfloat oldmult;
  gQuit = false;
  int step = 1, framecount = 0, spscount = 0;

  while(!gQuit) {
    start = SDL_GetTicks();

    ProcessSDLEvents();
    //Tick() changes the value of "multiplier"
    oldmult = multiplier;

    /** This regulates ticks (separate from FPS)
     * Should be about 90 per second.
     */
    cur_tick = SDL_GetTicks();
    diff_tick += (float)(cur_tick - last_tick);
    while(diff_tick >= sps_interval) {
      Tick();
      diff_tick -= sps_interval;
      spscount++;
    }
    last_tick = cur_tick;

    multiplier = oldmult;

    if(DrawGLScene()) {
      SDL_GL_SwapWindow (main_window);
    } else {
      printf("Eventloop(): couldn't draw scene, exiting...");
      gQuit = true;
    }

    oldmult = multiplier;

    GetKeys((unsigned long *)theKeyMap);

    if(IsKeyDown(theKeyMap, MAC_COMMAND_KEY) && IsKeyDown(theKeyMap, MAC_Q_KEY)) {
      gQuit = true;

      config.WriteHighScore(score);
    }

    if(IsKeyDown(theKeyMap, MAC_ESCAPE_KEY)) {
      alSourcePause(gSourceID[rainsound]);

      state = MAINMENU;

      alSourcePlay(gSourceID[souloutsound]);

      flashamount = 1;

      flashr = 1;
      flashg = 1;
      flashb = 1;

      alSourceStop(gSourceID[visionsound]);

      whichsong = mainmenusong;

      alSourceStop(gSourceID[knifesong]);
      alSourceStop(gSourceID[shootsong]);
      alSourceStop(gSourceID[zombiesong]);
      alSourceStop(gSourceID[mainmenusong]);

      alSourcef(gSourceID[knifesong], AL_MIN_GAIN, 0);
      alSourcef(gSourceID[shootsong], AL_MIN_GAIN, 0);
      alSourcef(gSourceID[zombiesong], AL_MIN_GAIN, 0);
      alSourcef(gSourceID[mainmenusong], AL_MIN_GAIN, 0);

      alSourcePlay(gSourceID[whichsong]);

      alSourcef(gSourceID[whichsong], AL_MIN_GAIN, 1);
    }

    if(IsKeyDown(theKeyMap, MAC_F1_KEY)) { //Help screen
      if(state != HELP) {
        prev_state = state;
        state = HELP;

        alSourcePause(gSourceID[rainsound]);

        alSourcePlay(gSourceID[souloutsound]);

        alSourceStop(gSourceID[visionsound]);

        whichsong = mainmenusong;

        alSourceStop(gSourceID[knifesong]);
        alSourceStop(gSourceID[shootsong]);
        alSourceStop(gSourceID[zombiesong]);
        alSourceStop(gSourceID[mainmenusong]);

        alSourcef(gSourceID[knifesong], AL_MIN_GAIN, 0);
        alSourcef(gSourceID[shootsong], AL_MIN_GAIN, 0);
        alSourcef(gSourceID[zombiesong], AL_MIN_GAIN, 0);
        alSourcef(gSourceID[mainmenusong], AL_MIN_GAIN, 0);

        alSourcePlay(gSourceID[whichsong]);

        alSourcef(gSourceID[whichsong], AL_MIN_GAIN, 1);

      } else { //Back to game or mainmenu
        state = prev_state;

        if(state == GAME) {
          MoveMouse(oldmouseloc.h, oldmouseloc.v, &mouseloc);

          if(environment.type == rainy_environment)
            alSourcePlay(gSourceID[rainsound]);

          if(environment.type != rainy_environment)
            alSourcePause(gSourceID[rainsound]);

          alSourceStop(gSourceID[whichsong]);

          alSourcef(gSourceID[whichsong], AL_MIN_GAIN, 0);
          alSourcef(gSourceID[whichsong], AL_MAX_GAIN, 0);

          if(person[0].weapon.type == knife)
            whichsong = knifesong;
          else
            whichsong = shootsong;

          if(type == zombie_type)
            whichsong = zombiesong;

          alSourcePlay(gSourceID[whichsong]);

          alSourcef(gSourceID[whichsong], AL_MIN_GAIN, 1);
          alSourcef(gSourceID[whichsong], AL_MAX_GAIN, 1);

          alSourcePlay(gSourceID[soulinsound]);

          if(config.visions)
            alSourcePlay(gSourceID[visionsound]);
        }
      }
      //FIXME: this probably isn't the best way to do this...
      SDL_Delay(200); //fix key press issue
    }

    if(IsKeyDown(theKeyMap, MAC_F12_KEY)) {
      char file[20];
      sprintf(file, "screenshot%d.tga", shotcount); //TODO: timestamp
      printf("Saving screenshot: %s\n", file);
      shotcount++;
      SaveScreenshot(file);
      SDL_Delay(100); //FIXME: hack - so only one shot per key press
    }

    framecount++;

    end = SDL_GetTicks();
    sleep = (int)(interval - (end - start)) + step;
    if (sleep > 0) {
      SDL_Delay(sleep);
    }

    if(end > last_calc + 1000) {
      if(framecount < maxfps) {
        step--;
      } else {
        step++;
      }

      framespersecond = framecount;
      std::cout << "fps: " << framespersecond << std::endl;
      framecount = 0;
      //std::cout << "sps: " << spscount << std::endl;
      spscount = 0;
      last_calc = end;
    }

    multiplier = 0.0110f; //bizarely correct
    
    if(multiplier > 1)
      multiplier = 1;

    if(multiplier < .00001)
      multiplier = .00001;

    if(config.visions == 1 && state == GAME)
      multiplier /= 3;

    if(slomo)
      multiplier *= .2;

    if(paused)
      multiplier = 0;
  }
}
