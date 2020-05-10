/*
 * GameTick.cpp
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

#include "Weapon.h"
#include "Game.h"

extern double multiplier;

extern Sprites sprites;

extern unsigned int gSourceID[100];
extern unsigned int gSampleSet[100];

extern Camera camera;
extern Fog fog;
extern Config config;
extern Environment environment;
extern Decals decals;

extern float soundscalefactor;

extern int slomo;

extern int forwardskey;
extern int backwardskey;
extern int leftkey;
extern int rightkey;
extern int aimkey;
extern int psychicaimkey;
extern int psychickey;

#define maxfallvel 40

void Game::Splat(int k)
{
  if(k != 0 || config.visions == 0) {
    person[k].health = 0;
    person[k].DoAnimations(k);
    person[k].skeleton.offset = 0;
    person[k].skeleton.free = 1;
    person[k].longdead = 1;

    person[k].bleeding = 1;
    person[k].bleeddelay = 1;
    person[k].bjoint1 =
        &person[k].skeleton.joints[person[k].skeleton.jointlabels[head]];
    person[k].bjoint2 =
        &person[k].skeleton.joints[person[k].skeleton.jointlabels[neck]];

    for(int j = 0; j < person[k].skeleton.num_joints; j++) {
      person[k].skeleton.joints[j].position +=
          person[k].skeleton.joints[j].offset;
      person[k].skeleton.joints[j].position =
          DoRotation(person[k].skeleton.joints[j].position, 0,
                     person[k].playerrotation, 0);
      person[k].skeleton.joints[j].position += person[k].playercoords;
      person[k].skeleton.joints[j].realoldposition =
          person[k].skeleton.joints[j].position;
      person[k].skeleton.joints[j].velocity = 0;
      person[k].skeleton.joints[j].velocity.y += person[k].velocity.y;
    }

    float gLoc[3];

    gLoc[0] =
        person[k].skeleton.joints[person[k].skeleton.jointlabels[head]].
        position.x / soundscalefactor;
    gLoc[1] =
        person[k].skeleton.joints[person[k].skeleton.jointlabels[head]].
        position.y / soundscalefactor;
    gLoc[2] =
        person[k].skeleton.joints[person[k].skeleton.jointlabels[head]].
        position.z / soundscalefactor;

    alSourcefv(gSourceID[headwhacksound], AL_POSITION, gLoc);
    alSourcePlay(gSourceID[headwhacksound]);
  }
}

void Game::MainMenuTick()
{
    float mousex;
    float mousey;
    bool clicked = ButtonClick(1);

    //resume from pause
    if(IsKeyDown(theKeyMap, MAC_SPACE_KEY)) {
      state = GAME;
    }

    GetMouse(&mouseloc);
    mousex = mouseloc.h;
    mousey = mouseloc.v;

    mousex = (float)mouseloc.h / screenwidth;
    mousey = (screenheight - (float)mouseloc.v) / screenheight;

    //printf("mousex: %f, mousey: %f\n", mousex, mousey);

    oldmouseoverbutton = mouseoverbutton;

    mouseoverbutton = 0;

    Button *button1 = 
      (Button*) WindowMgr::getInstance().getRoot()->getWindow("mainmenu")->
      getWindow("button1");
    Button *button2 = 
      (Button*) WindowMgr::getInstance().getRoot()->getWindow("mainmenu")->
      getWindow("button2");
    //FIXME: change these coords (put in new button widget)
    if(button1->isMouseOver(1, mousex, mousey)) {
      mouseoverbutton = 1;
    }

    if(button2->isMouseOver(1, mousex, mousey)) {
      mouseoverbutton = 2;
    }

    if(!mouseoverbutton) {
      return;
    }

    if(!oldbutton) {
    //start new game
    if(clicked && mouseoverbutton == 1 && !gameinprogress) {
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

      alSourcef(gSourceID[whichsong], AL_PITCH, 1);
      alSourcePlay(gSourceID[whichsong]);
      alSourcef(gSourceID[whichsong], AL_MIN_GAIN, 1);
      alSourcef(gSourceID[whichsong], AL_MAX_GAIN, 1);

      flashamount = 1;

      flashr = 1;
      flashg = 1;
      flashb = 1;

      state = GAME;

      alSourcePlay(gSourceID[soulinsound]);

      mission = 0;

      InitGame();

      gameinprogress = 1;
    }

    //resume
    if(clicked && mouseoverbutton == 1 && gameinprogress) {

      flashamount = 1;

      flashr = 1;
      flashg = 1;
      flashb = 1;

      state = GAME;

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

    //quit
    if(clicked && mouseoverbutton == 2 && !gameinprogress) {
      flashamount = 1;
      flashr = 1;
      flashg = 0;
      flashb = 0;

      alSourcePlay(gSourceID[losesound]);

      gQuit = true;

      config.WriteHighScore(score);
    }

    //end game
    if(clicked && mouseoverbutton == 2 && gameinprogress) {
      flashamount = 1;
      flashr = 1;
      flashg = 1;
      flashb = 1;

      alSourcePlay(gSourceID[losesound]);

      gameinprogress = 0;

      config.WriteHighScore(score);
    }
    }

    if(clicked)
      oldbutton = 1;

    if(!clicked)
      oldbutton = 0;
}

//Check collision with buildings, the ground
void Game::checkCollisions()
{
    XYZ collpoint;
    int pointnum;
    float depth;
    bool inblock;

    person[0].playerrotation = 180 - camera.rotation;

    //Check people collisions
    for(int k = 0; k < numpeople; k++) {
      // SBF - backing up the old coordinates makes
      // the most sense here.
      person[k].oldplayercoords = person[k].playercoords;
      person[k].DoStuff(k);

      if(person[k].skeleton.free < 1) {

        if(1 == 1) {

          person[k].onground = 0;

          overpoint = person[k].playercoords;
          overpoint.y += 3000;

          underpoint = person[k].playercoords;
          underpoint.y -= 3000;

          beginx =
              (int)((person[k].playercoords.x +
                     block_spacing / 2) / block_spacing);

          if(beginx < 0)
            beginx = 0;

          beginz =
              (int)((person[k].playercoords.z +
                     block_spacing / 2) / block_spacing);

          if(beginz < 0)
            beginz = 0;

          endx =
              (int)((person[k].playercoords.x +
                     block_spacing / 2) / block_spacing + 1);

          if(endx > num_blocks - 1)
            endx = num_blocks - 1;

          endz =
              (int)((person[k].playercoords.z +
                     block_spacing / 2) / block_spacing + 1);

          if(endz > num_blocks - 1)
            endz = num_blocks - 1;
#if 0
          if(k != 0) {
            /* TODO: huh? */
            beginx == person[k].whichblockx;

            beginz == person[k].whichblocky;

            endx == person[k].whichblockx;

            endz == person[k].whichblocky;
          }
#endif
          if(beginx <= endx && beginz <= endz)

            for(int i = beginx; i <= endx; i++)
              for(int j = beginz; j <= endz; j++) {
                inblock = 0;

                //Ground collision
                move = 0;
                move.x = i * block_spacing;
                move.z = j * block_spacing;

                whichtri =
                    sidewalkcollide.LineCheck2(overpoint, underpoint,
                                               &collpoint, move,
                                               cityrotation[i][j] * 90);

                if(whichtri != -1 && person[k].playercoords.y <= collpoint.y
                   && person[k].velocity.y <= 0) {

                  person[k].playercoords.y = collpoint.y;
                  person[k].onground = 1;

                  if(person[k].velocity.y < -maxfallvel)
                    Splat(k);
                  else
                    person[k].velocity.y = 0;
                }

                if(whichtri != -1) {
                  inblock = 1;

                  if(k == 0) {
                    onblockx = i;
                    onblocky = j;
                  }
                }
                //Wall collision
                if(k == 0) {
                  if(inblock) {
                    for(int l = 0; l < 8; l++) {
                      move = 0;
                      move.x = i * block_spacing;
                      move.z = j * block_spacing;

                      whichtri =
                          blockwalls[citytype[i][j]].LineCheck3(person[k].
                                                                oldplayercoords
                                                                +
                                                                boundingpoints
                                                                [l],
                                                                person[k].
                                                                playercoords +
                                                                boundingpoints
                                                                [l], &collpoint,
                                                                move,
                                                                cityrotation[i]
                                                                [j] * 90,
                                                                &depth);

                      if(whichtri != -1) {

                        normalrotated =
                            DoRotation(blockwalls[citytype[i][j]].
                                       normals[whichtri], 0,
                                       cityrotation[i][j] * 90, 0);

                        person[k].playercoords +=
                            normalrotated *
                            (-
                             (dotproduct
                              (normalrotated,
                               person[k].playercoords -
                               person[k].oldplayercoords)) - depth);
                      }
                    }

                    for(int l = 0; l < 8; l++) {
                      pointnum = k + 1;

                      if(pointnum > 3)
                        pointnum = 0;

                      move = 0;
                      move.x = i * block_spacing;
                      move.z = j * block_spacing;

                      whichtri =
                          blockwalls[citytype[i][j]].LineCheck3(person[k].
                                                                playercoords +
                                                                boundingpoints
                                                                [l],
                                                                person[k].
                                                                playercoords +
                                                                boundingpoints
                                                                [pointnum],
                                                                &collpoint,
                                                                move,
                                                                cityrotation[i]
                                                                [j] * 90,
                                                                &depth);

                      if(whichtri != -1) {

                        normalrotated =
                            DoRotation(blockwalls[citytype[i][j]].
                                       normals[whichtri], 0,
                                       cityrotation[i][j] * 90, 0);

                        person[k].playercoords += normalrotated;
                      }
                    }
                  }
                  //Roof collision
                  if(inblock && person[k].playercoords.y > 30) {

                    if(!person[k].onground) {

                      move = 0;
                      move.x = i * block_spacing;
                      move.z = j * block_spacing;

                      whichtri =
                          blockroofs[citytype[i][j]].LineCheck2(overpoint,
                                                                underpoint,
                                                                &collpoint,
                                                                move,
                                                                cityrotation[i]
                                                                [j] * 90);

                      if(whichtri != -1
                         && person[k].playercoords.y <= collpoint.y
                         && person[k].velocity.y <= 0) {

                        person[k].playercoords.y = collpoint.y;
                        person[k].onground = 1;

                        if(person[k].velocity.y < -maxfallvel)
                          Splat(k);
                        else
                          person[k].velocity.y = 0;
                      }

                      if(whichtri != -1)
                        inblock = 1;
                    }
                  }
                }
              }

          if(person[k].playercoords.y <= 0) {

            person[k].onground = 1;
            person[k].playercoords.y = 0;

            if(person[k].velocity.y < -maxfallvel)
              Splat(k);
            else
              person[k].velocity.y = 0;

            if(k == 0)
              onblockx = -1;
            onblocky = -1;
          }
          // SBF - this is definately in the wrong spot!
          //person[k].oldplayercoords=person[k].playercoords;
        }
      }
    }
}

void Game::Fire()
{
//Fire/wing
    XYZ wallhit;
    XYZ start;
    XYZ finalwallhit;

    bool firing;
    float inaccuracy = 0.0;
    int numshots;

    XYZ hitnorm;
    XYZ hitmove;

    int hitpoly = 0;
    float hitrotation = 0.0f;

    Model *model = NULL;
    bool thirdperson = config.thirdperson;

    for(int j = 0; j < numpeople; j++) {

      if(person[j].type == playertype || person[j].type == eviltype) {

        firing = 0;

        if(person[j].weapon.ammo > 0 && person[j].weapon.reloading <= 0
           && person[j].targetanimation != joganim) {

          if(ButtonClick(1) && !oldbutton && j == 0)
            firing = 1;

          if(j != 0)
            firing = 0;

          if(j != 0 && person[j].weapon.type != nogun
             && person[j].weapon.type != knife && person[j].killtargetvisible
             && person[j].weapon.shotdelay < 0)
            firing = 1;

          if(j != 0 && person[j].weapon.type != nogun
             && person[j].weapon.type != knife && person[j].killtargetvisible
             && person[j].weapon.shotdelay > 0)
            person[j].weapon.shotdelay -= multiplier * .9;

          if(person[j].skeleton.free == 1
             || person[j].targetanimation == getupfrontanim
             || person[j].targetanimation == getupbackanim)
            person[j].weapon.shotdelay = shotdelayamount / difficulty;

          if(j != 0 && person[j].weapon.type != nogun
             && person[j].weapon.type != knife && person[j].killtargetvisible)
            person[j].aiming = 1;

          if(person[0].weapon.type != assaultrifle)
            oldbutton = 1;

        }

        if(person[j].weapon.ammo == 0 && person[j].reloads[person[j].weapon.type] > 0) {
          person[j].weapon.ammo = -1;
          person[j].aiming = 0;
          firing = 0;
        }

        if(person[0].weapon.ammo <= 0 && ButtonClick(1) && !oldbutton
           && person[0].targetanimation != joganim
           && person[0].weapon.type != nogun && person[0].weapon.type != knife
           && person[0].weapon.type != grenade && person[0].aiming) {

          oldbutton = 1;

          float gLoc[3];
          gLoc[0] = person[0].playercoords.x / soundscalefactor;
          gLoc[1] = person[0].playercoords.y / soundscalefactor;
          gLoc[2] = person[0].playercoords.z / soundscalefactor;

          alSourcefv(gSourceID[clicksound], AL_POSITION, gLoc);
          alSourcePlay(gSourceID[clicksound]);
        }

        if(j == 0 && config.visions == 1 && firing) {
          firing = 0;
          alSourcePlay(gSourceID[soulinsound]);
        }

        if(person[j].aimamount < 1)
          firing = 0;

        if(firing) {
          switch(person[j].weapon.type) {
            case handgun1:
            case handgun2:
              inaccuracy = 8;
              break;

            case assaultrifle:
              inaccuracy = 6;
              break;

            case sniperrifle:
              inaccuracy = 2;
              break;

            case shotgun:
              inaccuracy = 6;
              break;
          }

          if(person[person[j].killtarget].skeleton.free == 1)
            inaccuracy *= 3;

          person[j].weapon.shotdelay = shotdelayamount / difficulty;

          if(person[j].aiming >= 1 && person[j].weapon.recoil <= 0) {

            //Firing
            person[j].litup = 1;
            person[j].weapon.recoil = 1;

            XYZ end, aim, oldend;

            HitStruct hitstruct, temphitstruct;
            hitstruct.hitlocation = 0.0f;

            float olddistance = 0.0f;
            float distance = 0.0f;
            float totalarea = 0.0f;

            int whichhit = -1;

            if(person[j].weapon.type == shotgun)
              numshots = 7;

            if(person[j].weapon.type != shotgun)
              numshots = 1;

            if(person[j].weapon.type != grenade)
              person[j].weapon.ammo--;

            for(int p = 0; p < numshots; p++) {

              if(!zoom || j != 0) {

                if(person[j].weapon.type == assaultrifle)
                  aim =
                      DoRotation(person[j].skeleton.
                                 joints[(person[j].skeleton.
                                         jointlabels[lefthand])].position -
                                 person[j].skeleton.
                                 joints[(person[j].skeleton.
                                         jointlabels[righthand])].position, 0,
                                 person[j].playerrotation - 2.5, 0);

                if(person[j].weapon.type == sniperrifle)
                  aim =
                      DoRotation(person[j].skeleton.
                                 joints[(person[j].skeleton.
                                         jointlabels[lefthand])].position -
                                 person[j].skeleton.
                                 joints[(person[j].skeleton.
                                         jointlabels[righthand])].position, 0,
                                 person[j].playerrotation + 4, 0);

                if(person[j].weapon.type == shotgun)
                  aim =
                      DoRotation(person[j].skeleton.
                                 joints[(person[j].skeleton.
                                         jointlabels[lefthand])].position -
                                 person[j].skeleton.
                                 joints[(person[j].skeleton.
                                         jointlabels[righthand])].position,
                                 2 + (float)(Random() % 1000) / 500, 0, 0);

                if(person[j].weapon.type == shotgun)
                  aim =
                      DoRotation(aim, 0,
                                 person[j].playerrotation - 1 +
                                 (float)(Random() % 1000) / 500, 0);

                if(person[j].weapon.type == handgun1 && !thirdperson && j == 0)
                  aim =
                      DoRotation(person[j].skeleton.
                                 joints[(person[j].skeleton.
                                         jointlabels[righthand])].position -
                                 (person[j].skeleton.
                                  joints[person[j].skeleton.jointlabels[head]].
                                  position * .65 +
                                  person[j].skeleton.joints[person[j].skeleton.
                                                            jointlabels[neck]].
                                  position * .35), 0,
                                 person[j].playerrotation - .9, 0);

                if(person[j].weapon.type == handgun1 && (thirdperson || j != 0))
                  aim =
                      DoRotation(person[j].skeleton.
                                 joints[(person[j].skeleton.
                                         jointlabels[righthand])].position -
                                 (person[j].skeleton.
                                  joints[person[j].skeleton.jointlabels[head]].
                                  position * .35 +
                                  person[j].skeleton.joints[person[j].skeleton.
                                                            jointlabels[neck]].
                                  position * .65), 0,
                                 person[j].playerrotation - .9, 0);

                if(person[j].weapon.type == handgun2 && !thirdperson && j == 0)
                  aim =
                      DoRotation(person[j].skeleton.
                                 joints[(person[j].skeleton.
                                         jointlabels[righthand])].position -
                                 (person[j].skeleton.
                                  joints[person[j].skeleton.jointlabels[head]].
                                  position * .65 +
                                  person[j].skeleton.joints[person[j].skeleton.
                                                            jointlabels[neck]].
                                  position * .35), 0,
                                 person[j].playerrotation - .9, 0);

                if(person[j].weapon.type == handgun2 && (thirdperson || j != 0))
                  aim =
                      DoRotation(person[j].skeleton.
                                 joints[(person[j].skeleton.
                                         jointlabels[righthand])].position -
                                 (person[j].skeleton.
                                  joints[person[j].skeleton.jointlabels[head]].
                                  position * .35 +
                                  person[j].skeleton.joints[person[j].skeleton.
                                                            jointlabels[neck]].
                                  position * .65), 0,
                                 person[j].playerrotation - .9, 0);

                if(j != 0 && person[person[j].killtarget].skeleton.free == 0)
                  aim =
                      (DoRotation
                       (person[person[j].killtarget].skeleton.
                        joints[person[person[j].killtarget].skeleton.
                               jointlabels[abdomen]].position, 0,
                        person[person[j].killtarget].playerrotation,
                        0) + person[person[j].killtarget].playercoords) -
                      (DoRotation
                       (person[j].skeleton.
                        joints[person[j].skeleton.jointlabels[lefthand]].
                        position, 0, person[j].playerrotation,
                        0) + person[j].playercoords);

                if(j != 0 && person[person[j].killtarget].skeleton.free != 0)
                  aim =
                      person[person[j].killtarget].skeleton.
                      joints[person[person[j].killtarget].skeleton.
                             jointlabels[abdomen]].position -
                      (DoRotation
                       (person[j].skeleton.
                        joints[person[j].skeleton.jointlabels[lefthand]].
                        position, 0, person[j].playerrotation,
                        0) + person[j].playercoords);

                if(j != 0)
                  aim =
                      DoRotation(DoRotation
                                 (DoRotation
                                  (aim, 0, -person[j].playerrotation, 0),
                                  (float)(abs(Random() % 2000)) / 2000 *
                                  inaccuracy - inaccuracy / 2,
                                  (float)(abs(Random() % 2000)) / 2000 *
                                  inaccuracy - inaccuracy / 2, 0), 0,
                                 person[j].playerrotation, 0);

              }

              if(zoom && j == 0) {
                start = camera.position;
                aim = facing;
              }

              Normalise(&aim);

              if(person[j].weapon.type == sniperrifle) {

                start =
                    person[j].playercoords +
                    DoRotation(person[j].skeleton.
                               joints[(person[j].skeleton.
                                       jointlabels[lefthand])].position, 0,
                               person[j].playerrotation, 0);

                float gLoc[3];

                start -=
                    DoRotation(DoRotation
                               (DoRotation
                                (aim, 0, -person[j].playerrotation, 0), 90, 0,
                                0), 0, person[j].playerrotation, 0) * .35;

                gLoc[0] =
                    (camera.position.x +
                     (start.x - camera.position.x) / 4) / soundscalefactor;

                gLoc[1] =
                    (camera.position.y +
                     (start.y - camera.position.y) / 4) / soundscalefactor;

                gLoc[2] =
                    (camera.position.z +
                     (start.z - camera.position.z) / 4) / soundscalefactor;

                alSourcefv(gSourceID[riflesound], AL_POSITION, gLoc);

                alSourcePlay(gSourceID[riflesound]);

                if(j == 0) {

                  if(person[j].currentanimation != crouchanim)
                    camera.rotation2 -= 7;

                  if(person[j].currentanimation == crouchanim)
                    camera.rotation2 -= 3;
                }
              }

              if(person[j].weapon.type == shotgun) {

                start =
                    person[j].playercoords +
                    DoRotation(person[j].skeleton.
                               joints[(person[j].skeleton.
                                       jointlabels[lefthand])].position, 0,
                               person[j].playerrotation, 0);

                float gLoc[3];

                start -=
                    DoRotation(DoRotation
                               (DoRotation
                                (aim, 0, -person[j].playerrotation, 0), 90, 0,
                                0), 0, person[j].playerrotation, 0) * .35;

                if(p == numshots - 1) {

                  gLoc[0] =
                      (camera.position.x +
                       (start.x - camera.position.x) / 4) / soundscalefactor;

                  gLoc[1] =
                      (camera.position.y +
                       (start.y - camera.position.y) / 4) / soundscalefactor;

                  gLoc[2] =
                      (camera.position.z +
                       (start.z - camera.position.z) / 4) / soundscalefactor;

                  alSourcefv(gSourceID[shotgunsound], AL_POSITION, gLoc);

                  alSourcePlay(gSourceID[shotgunsound]);

                  if(j == 0) {

                    if(person[j].currentanimation != crouchanim)
                      camera.rotation2 -= 7;

                    if(person[j].currentanimation == crouchanim)
                      camera.rotation2 -= 3;
                  }
                }
              }

              if(person[j].weapon.type == handgun1) {

                start =
                    person[j].playercoords +
                    DoRotation(person[j].skeleton.
                               joints[(person[j].skeleton.
                                       jointlabels[rightwrist])].position, 0,
                               person[j].playerrotation, 0);

                start -=
                    DoRotation(DoRotation
                               (DoRotation
                                (aim, 0, -person[j].playerrotation, 0), 90, 0,
                                0), 0, person[j].playerrotation, 0) * .55;

                float gLoc[3];

                gLoc[0] =
                    (camera.position.x +
                     (start.x - camera.position.x) / 4) / soundscalefactor;

                gLoc[1] =
                    (camera.position.y +
                     (start.y - camera.position.y) / 4) / soundscalefactor;

                gLoc[2] =
                    (camera.position.z +
                     (start.z - camera.position.z) / 4) / soundscalefactor;

                alSourcefv(gSourceID[pistol1sound], AL_POSITION, gLoc);

                alSourcePlay(gSourceID[pistol1sound]);

                if(j == 0) {

                  if(person[j].currentanimation != crouchanim)
                    camera.rotation2 -= 6;

                  if(person[j].currentanimation == crouchanim)
                    camera.rotation2 -= 4;
                }
              }

              if(person[j].weapon.type == handgun2) {

                start =
                    person[j].playercoords +
                    DoRotation(person[j].skeleton.
                               joints[(person[j].skeleton.
                                       jointlabels[rightwrist])].position, 0,
                               person[j].playerrotation, 0);

                start -=
                    DoRotation(DoRotation
                               (DoRotation
                                (aim, 0, -person[j].playerrotation, 0), 90, 0,
                                0), 0, person[j].playerrotation, 0) * .55;



                float gLoc[3];

                gLoc[0] =
                    (camera.position.x +
                     (start.x - camera.position.x) / 4) / soundscalefactor;

                gLoc[1] =
                    (camera.position.y +
                     (start.y - camera.position.y) / 4) / soundscalefactor;

                gLoc[2] =
                    (camera.position.z +
                     (start.z - camera.position.z) / 4) / soundscalefactor;

                alSourcefv(gSourceID[pistol2sound], AL_POSITION, gLoc);
                alSourcePlay(gSourceID[pistol2sound]);

                if(j == 0) {
                  if(person[j].currentanimation != crouchanim)
                    camera.rotation2 -= 5;

                  if(person[j].currentanimation == crouchanim)
                    camera.rotation2 -= 3;
                }
              }

              if(person[j].weapon.type == assaultrifle) {

                start =
                    person[j].playercoords +
                    DoRotation(person[j].skeleton.
                               joints[(person[j].skeleton.
                                       jointlabels[lefthand])].position, 0,
                               person[j].playerrotation, 0);

                start -=
                    DoRotation(DoRotation
                               (DoRotation
                                (aim, 0, -person[j].playerrotation, 0), 90, 0,
                                0), 0, person[j].playerrotation, 0) * .25;

                float gLoc[3];

                gLoc[0] =
                    (camera.position.x +
                     (start.x - camera.position.x) / 4) / soundscalefactor;

                gLoc[1] =
                    (camera.position.y +
                     (start.y - camera.position.y) / 4) / soundscalefactor;

                gLoc[2] =
                    (camera.position.z +
                     (start.z - camera.position.z) / 4) / soundscalefactor;

                alSourcefv(gSourceID[machinegunsound], AL_POSITION, gLoc);
                alSourcePlay(gSourceID[machinegunsound]);

                if(j == 0) {

                  if(person[j].currentanimation != crouchanim) {
                    camera.rotation2 -= 2.3;
                    camera.rotation += (float)(Random() % 100) / 50;
                  }

                  if(person[j].currentanimation == crouchanim) {
                    camera.rotation2 -= 1.5;
                    camera.rotation += (float)(Random() % 100) / 60;
                  }
                }
              }

              end = start + aim * 1000;

              if(config.debug) {
                if(IsKeyDown(theKeyMap, MAC_G_KEY) && j == 0) {
                  sprites.MakeSprite(grenadesprite, 1, 1, 1, 1, start,
                                     aim * 200, 1.01);
                }
              }

              if(!IsKeyDown(theKeyMap, MAC_G_KEY) || j != 0 || !config.debug) {
                int bulletstrength = 1;
                int firstpass = -1;
                bool penetrate;

                for(int m = 0; m < bulletstrength; m++) {
                  //People
                  whichhit = -1;

                  for(int i = 0; i < numpeople; i++) {
                    if(i != j && i != firstpass && person[i].existing) {

                      temphitstruct =
                          person[i].BulletCollideWithPlayer(i, start, end);

                      if(temphitstruct.collision) {

                        distance =
                            findDistancefast(start, temphitstruct.hitlocation);

                        if(distance < olddistance || whichhit == -1) {

                          end = temphitstruct.hitlocation;

                          olddistance = distance;

                          hitstruct = temphitstruct;

                          whichhit = i;
                        }
                      }
                    }
                  }

                  //Blocks
                  wallhit = 0;

                  beginx =
                      (int)(person[j].playercoords.x +
                            block_spacing / 2) / block_spacing - 3;

                  if(beginx < 0)
                    beginx = 0;

                  beginz =
                      (int)(person[j].playercoords.z +
                            block_spacing / 2) / block_spacing - 3;

                  if(beginz < 0)
                    beginz = 0;

                  endx =
                      (int)(person[j].playercoords.x +
                            block_spacing / 2) / block_spacing + 3;

                  if(endx > num_blocks - 1)
                    endx = num_blocks - 1;

                  endz =
                      (int)(person[j].playercoords.z +
                            block_spacing / 2) / block_spacing + 3;

                  if(endz > num_blocks - 1)
                    endz = num_blocks - 1;

                  if(beginx < endx && beginz < endz)

                    finalwallhit = 0;

                  for(int i = beginx; i <= endx; i++)

                    for(int j = beginz; j <= endz; j++) {

                      move = 0;
                      move.x = i * block_spacing;
                      move.z = j * block_spacing;

                      whichtri =
                          blocks[citytype[i][j]].LineCheck2(start, end,
                                                            &wallhit, move,
                                                            cityrotation[i][j] *
                                                            90);

                      if(whichtri != -1) {
                        whichhit = -1;
                        end = wallhit;
                        finalwallhit = wallhit;

                        hitnorm =
                            DoRotation(blocks[citytype[i][j]].normals[whichtri],
                                       0, cityrotation[i][j] * 90, 0);

                        hitmove = move;
                        hitrotation = cityrotation[i][j] * 90;
                        hitpoly = whichtri;
                        model = &blocks[citytype[i][j]];

                        if(j == 0
                           && blocks[citytype[i][j]].normals[whichtri].y > .9)
                          bulletstrength = 2;
                      }
                    }

                  wallhit = 0;
                  wallhit.x = camera.position.x;
                  wallhit.z = camera.position.z;

                  whichtri =
                      Bigstreet.LineCheck2(start, end, &wallhit, wallhit, 0);

                  if(whichtri != -1) {

                    end.y -= .5;
                    end = wallhit;
                    finalwallhit = wallhit;

                    bulletstrength = 2;

                    hitnorm = 0;
                    hitnorm.y = 1;
                    hitmove = 0;
                    hitrotation = 0;
                  }

                  if(m == 0) {
                    if(j == 0 && slomo == 2) {
                      soundscalefactor = soundscalefactordefault;

                      alSourceStop(gSourceID[whichsong]);

                      alSourcef(gSourceID[whichsong], AL_MIN_GAIN, 0);
                      alSourcef(gSourceID[whichsong], AL_MAX_GAIN, 0);

                      if(person[0].weapon.type == knife)
                        whichsong = knifesong;

                      if(person[0].weapon.type != knife)
                        whichsong = shootsong;

                      if(type == zombie_type)
                        whichsong = zombiesong;

                      alSourcef(gSourceID[whichsong], AL_PITCH, 1);
                      alSourcePlay(gSourceID[whichsong]);
                      alSourcef(gSourceID[whichsong], AL_MIN_GAIN, 1);
                      alSourcef(gSourceID[whichsong], AL_MAX_GAIN, 1);

                      slomo = 0;

                      if(whichhit == -1)
                        alSourcePlay(gSourceID[disguisekillsound]);

                      flashamount = .5;

                      flashr = 1;
                      flashg = 1;
                      flashb = 1;
                    }
                  }
                  //Impact

                  oldend = end;

                  //with person
                  if(whichhit != -1 && whichhit != firstpass) {
                    if(j == 0) {
                      person[whichhit].dead = 1;
                    }

                    if(whichhit == 1) {
                      murderer = j;
                    }

                    if(person[whichhit].health == 100 && j == 0) {

                      if(person[whichhit].type == civiliantype)
                        civkills++;

                      if(person[whichhit].type == eviltype)
                        goodkills++;
                    }

                    if(person[whichhit].health == 100 && j != 0) {
                      badkills++;
                    }
                    //penetrate
                    penetrate = abs(Random() % 2) == 1;

                    if(numshots > 1)
                      penetrate = 0;

                    if(penetrate) {
                      bulletstrength = 2;
                      firstpass = whichhit;
                      end = start + aim * 1000;
                    }

                    if(person[j].weapon.type == assaultrifle)
                      person[whichhit].health -= 20;

                    if(person[j].weapon.type == assaultrifle
                       && person[whichhit].type == zombietype)
                      person[whichhit].health -= 60;

                    if(person[j].weapon.type == handgun1) {

                      if(person[whichhit].type != zombietype)
                        person[whichhit].health -= 100;

                      if(person[whichhit].type == zombietype)
                        person[whichhit].health -= 100;

                      person[whichhit].DoAnimations(whichhit);
                    }

                    if(person[j].weapon.type == handgun2)
                      person[whichhit].health -= 20;

                    if(person[j].weapon.type == handgun2
                       && person[whichhit].type == zombietype)
                      person[whichhit].health -= 60;

                    if(person[j].weapon.type == sniperrifle && m != 0)
                      person[whichhit].health -= 30;

                    if(person[j].weapon.type == shotgun)
                      person[whichhit].health -= 60;

                    if(person[j].weapon.type == sniperrifle && m == 0) {

                      if(person[whichhit].type != zombietype)
                        person[whichhit].health -= 100;

                      if(person[whichhit].type == zombietype)
                        person[whichhit].health -= 120;

                      person[whichhit].DoAnimations(whichhit);
                    }

                    if(hitstruct.joint1->modelnum == headmodel
                       && person[whichhit].type != zombietype) {

                      person[whichhit].health -= 60;
                    }

                    if(person[whichhit].type == zombietype)
                      person[whichhit].speedmult -= .05;

                    if(person[whichhit].type == zombietype)
                      person[whichhit].maxhealth -= 10;

                    if(whichhit == 0) {
                      bulletstrength = 1;

                      person[0].health = 100;

                      flashr = 0;
                      flashg = 0;
                      flashb = 0;

                      flashamount = 1;

                      float gLoc[3];
                      gLoc[0] = hitstruct.hitlocation.x / soundscalefactor;
                      gLoc[1] = hitstruct.hitlocation.y / soundscalefactor;
                      gLoc[2] = hitstruct.hitlocation.z / soundscalefactor;

                      alSourcefv(gSourceID[bodywhacksound], AL_POSITION, gLoc);

                      alSourcePlay(gSourceID[bodywhacksound]);
                    }

                    person[whichhit].longdead = 1;

                    if(person[whichhit].health <= 0) {

                      person[whichhit].skeleton.offset = 0;

                      if(person[whichhit].skeleton.free != 1) {

                        person[whichhit].skeleton.free = 1;

                        totalarea = 0;

                        for(int j = 0; j < person[whichhit].skeleton.num_joints;
                            j++) {

                          person[whichhit].skeleton.joints[j].position =
                              DoRotation(person[whichhit].skeleton.joints[j].
                                         position, 0,
                                         person[whichhit].playerrotation, 0);

                          person[whichhit].skeleton.joints[j].position +=
                              person[whichhit].playercoords;

                          person[whichhit].skeleton.joints[j].realoldposition =
                              person[whichhit].skeleton.joints[j].position;

                          person[whichhit].skeleton.joints[j].velocity =
                              person[whichhit].velocity;

                          person[whichhit].skeleton.joints[j].velocity.x +=
                              (float)(abs(Random() % 20) - 10) / 2;

                          person[whichhit].skeleton.joints[j].velocity.y +=
                              (float)(abs(Random() % 20) - 10) / 2;

                          person[whichhit].skeleton.joints[j].velocity.z +=
                              (float)(abs(Random() % 20) - 10) / 2;
                        }
                      }

                      for(int j = 0; j < person[whichhit].skeleton.num_joints;
                          j++) {

                        if(findDistancefast
                           (person[whichhit].skeleton.joints[j].position,
                            hitstruct.hitlocation) < 200) {

                          totalarea +=
                              (200 /
                               findDistancefast(person[whichhit].skeleton.
                                                joints[j].position,
                                                hitstruct.hitlocation));
                        }
                      }

                      for(int j = 0; j < person[whichhit].skeleton.num_joints;
                          j++) {

                        if(findDistancefast
                           (person[whichhit].skeleton.joints[j].position,
                            hitstruct.hitlocation) < 200) {

                          person[whichhit].skeleton.joints[j].velocity +=
                              aim *
                              ((200 /
                                findDistancefast(person[whichhit].skeleton.
                                                 joints[j].position,
                                                 hitstruct.hitlocation)) /
                               totalarea * 200);

                        }
                      }
                    }

                    if(person[whichhit].health > 0) {

                      if(person[whichhit].killtargetvisible == 0
                         && person[whichhit].type != zombietype
                         && person[whichhit].currentanimation !=
                         getupfrontanim & person[whichhit].currentanimation !=
                         getupbackanim) {

                        if(hitstruct.joint1->modelnum == headmodel)
                          person[whichhit].targetanimation = headpainanim;

                        if(hitstruct.joint1->modelnum == chestmodel)
                          person[whichhit].targetanimation = chestpainanim;

                        if(hitstruct.joint1->modelnum == abdomenmodel)
                          person[whichhit].targetanimation = stomachpainanim;

                        if(hitstruct.joint1->label == rightelbow
                           || hitstruct.joint1->label == rightshoulder
                           || hitstruct.joint1->label == rightwrist
                           || hitstruct.joint1->label == righthand)
                          person[whichhit].targetanimation = rightarmpainanim;

                        if(hitstruct.joint1->label == leftelbow
                           || hitstruct.joint1->label == leftshoulder
                           || hitstruct.joint1->label == leftwrist
                           || hitstruct.joint1->label == lefthand)
                          person[whichhit].targetanimation = leftarmpainanim;

                        if(hitstruct.joint1->label == rightknee
                           || hitstruct.joint1->label == righthip
                           || hitstruct.joint1->label == rightankle
                           || hitstruct.joint1->label == rightfoot)
                          person[whichhit].targetanimation = rightlegpainanim;

                        if(hitstruct.joint1->label == leftknee
                           || hitstruct.joint1->label == lefthip
                           || hitstruct.joint1->label == leftankle
                           || hitstruct.joint1->label == leftfoot)
                          person[whichhit].targetanimation = leftlegpainanim;

                        person[whichhit].targetframe = 0;
                        person[whichhit].target = 0;
                      }

                      person[whichhit].skeleton.offset = 1;

                      for(int j = 0; j < person[whichhit].skeleton.num_joints;
                          j++) {

                        if(findDistancefast
                           (DoRotation
                            (person[whichhit].skeleton.joints[j].position, 0,
                             person[whichhit].playerrotation,
                             0) + person[whichhit].playercoords,
                            hitstruct.hitlocation) < 200) {

                          totalarea +=
                              (200 /
                               findDistancefast(DoRotation
                                                (person[whichhit].skeleton.
                                                 joints[j].position, 0,
                                                 person[whichhit].
                                                 playerrotation,
                                                 0) +
                                                person[whichhit].playercoords,
                                                hitstruct.hitlocation));
                        }
                      }

                      float offsetlength;

                      for(int j = 0; j < person[whichhit].skeleton.num_joints;
                          j++) {

                        if(findDistancefast
                           (DoRotation
                            (person[whichhit].skeleton.joints[j].position, 0,
                             person[whichhit].playerrotation,
                             0) + person[whichhit].playercoords,
                            hitstruct.hitlocation) < 200) {

                          person[whichhit].skeleton.joints[j].offset +=
                              DoRotation(aim *
                                         ((200 /
                                           findDistancefast(DoRotation
                                                            (person[whichhit].
                                                             skeleton.joints[j].
                                                             position, 0,
                                                             person[whichhit].
                                                             playerrotation,
                                                             0) +
                                                            person[whichhit].
                                                            playercoords,
                                                            hitstruct.
                                                            hitlocation)) /
                                          totalarea * 10), 0,
                                         -person[whichhit].playerrotation, 0);

                        }

                        offsetlength =
                            findLengthfast(person[whichhit].skeleton.joints[j].
                                           offset);

                        if(offsetlength > 36) {

                          Normalise(&person[whichhit].skeleton.joints[j].
                                    offset);

                          person[whichhit].skeleton.joints[j].offset *= 6;

                        }
                      }
                    }

                    if(hitstruct.joint1->modelnum == headmodel
                       && person[whichhit].health <= 0) {

                      for(int j = 0; j < person[whichhit].skeleton.num_joints;
                          j++) {

                        if(&person[whichhit].skeleton.joints[j] ==
                           hitstruct.joint1
                           || &person[whichhit].skeleton.joints[j] ==
                           hitstruct.joint2) {

                          if(j != person[whichhit].skeleton.jointlabels[abdomen]
                             && j !=
                             person[whichhit].skeleton.jointlabels[groin]
                             && j !=
                             person[whichhit].skeleton.jointlabels[neck]) {

                            sprites.MakeSprite(bloodspritedown, .8, 1, .2, .2,
                                               person[whichhit].skeleton.
                                               joints[j].position,
                                               person[whichhit].skeleton.
                                               joints[j].velocity / 3, 9);

                            sprites.MakeSprite(bloodspritedown, .8, 1, .2, .2,
                                               person[whichhit].skeleton.
                                               joints[j].position,
                                               DoRotation(person[whichhit].
                                                          skeleton.joints[j].
                                                          velocity / 3,
                                                          Random() % 360,
                                                          Random() % 360,
                                                          0) / 5, 5);

                            sprites.MakeSprite(bloodspritedown, .8, 1, .2, .2,
                                               person[whichhit].skeleton.
                                               joints[j].position,
                                               DoRotation(person[whichhit].
                                                          skeleton.joints[j].
                                                          velocity / 3,
                                                          Random() % 360,
                                                          Random() % 360,
                                                          0) / 5, 5);

                            sprites.MakeSprite(bloodspritedown, .8, 1, .2, .2,
                                               person[whichhit].skeleton.
                                               joints[j].position,
                                               DoRotation(person[whichhit].
                                                          skeleton.joints[j].
                                                          velocity / 3,
                                                          Random() % 360,
                                                          Random() % 360,
                                                          0) / 5, 5);

                            sprites.MakeSprite(bloodspritedown, .8, 1, .2, .2,
                                               person[whichhit].skeleton.
                                               joints[j].position,
                                               DoRotation(person[whichhit].
                                                          skeleton.joints[j].
                                                          velocity / 3,
                                                          Random() % 360,
                                                          Random() % 360,
                                                          0) / 5, 5);

                            person[whichhit].skeleton.DeleteJoint(j);
                            person[whichhit].skeleton.broken = 1;
                            person[whichhit].health = -10000;
                            person[whichhit].skeleton.joints[j].existing = 0;

                            if(person[whichhit].type == zombietype)
                              score += 300;
                          }
                        }
                      }
                    }

                    XYZ velocity;
                    velocity = aim * -8;
                    float size = 0;

                    //blood
                    if(!hitstruct.joint1->modelnum == headmodel) {

                      switch(person[j].weapon.type) {
                        case sniperrifle:
                        case shotgun:
                          if(penetrate)
                            size = 7;
                          else
                            size = 5;
                          break;

                        case assaultrifle:
                          if(penetrate)
                            size = 7;
                          else
                            size = 3;
                          break;

                        case handgun1:
                        case handgun2:
                          if(penetrate)
                            size = 4;
                          else
                            size = 3;
                          break;

                        default:
                          size = 0;
                          break;
                      }

                      if(size) {
                        sprites.MakeSprite(bloodspritenoup, 1, 1, 0, 0,
                                           hitstruct.hitlocation, velocity * 0,
                                           size);

                        if(penetrate) {
                          sprites.MakeSprite(bloodspritenoup, 1, 1, 0, 0,
                                             hitstruct.hitlocation, velocity * -3,
                                             size);
                        }
                      }
                    } else {

                      sprites.MakeSprite(bloodspritenoup, 1, 1, .2, .2,
                                         hitstruct.hitlocation, velocity * 0,
                                         6);

                      sprites.MakeSprite(bloodspritenoup, 1, 1, .5, .5,
                                         hitstruct.hitlocation, velocity * -2,
                                         7);

                      sprites.MakeSprite(bloodspritenoup, 1, 1, .2, .2,
                                         hitstruct.hitlocation, velocity * -3,
                                         10);
                    }

                    person[whichhit].bjoint1 = hitstruct.joint1;
                    person[whichhit].bjoint2 = hitstruct.joint2;
                    person[whichhit].bleeding = 1;
                    person[whichhit].bleeddelay = 1;

                    float gLoc[3];

                    gLoc[0] =
                        (camera.position.x +
                         (hitstruct.hitlocation.x -
                          camera.position.x) / 4) / soundscalefactor;

                    gLoc[1] =
                        (camera.position.y +
                         (hitstruct.hitlocation.y -
                          camera.position.y) / 4) / soundscalefactor;

                    gLoc[2] =
                        (camera.position.z +
                         (hitstruct.hitlocation.z -
                          camera.position.z) / 4) / soundscalefactor;

                    if(!hitstruct.joint1->modelnum == headmodel) {

                      if(!thirdperson)
                        alSourcef(gSourceID[bodyhitsound], AL_MIN_GAIN, 1);

                      if(thirdperson)
                        alSourcef(gSourceID[bodyhitsound], AL_MIN_GAIN, .1);

                      alSourcefv(gSourceID[bodyhitsound], AL_POSITION, gLoc);
                      alSourcePlay(gSourceID[bodyhitsound]);
                    }

                    if(hitstruct.joint1->modelnum == headmodel) {

                      if(!thirdperson)
                        alSourcef(gSourceID[headshotsound], AL_MIN_GAIN, 1);

                      if(thirdperson)
                        alSourcef(gSourceID[headshotsound], AL_MIN_GAIN, .1);

                      alSourcefv(gSourceID[headshotsound], AL_POSITION, gLoc);
                      alSourcePlay(gSourceID[headshotsound]);
                    }
                  }             //with wall

                  if(oldend == finalwallhit) {

                    decals.MakeDecal(bullethole, finalwallhit, .7, hitnorm,
                                     hitpoly, model, hitmove, hitrotation);

                    XYZ velocity;
                    //velocity = aim * -4;
                    velocity = hitnorm * 3;
                    float smokesize, muzzleflashsize;

                    switch(person[j].weapon.type) {
                      case sniperrifle:
                        smokesize = 10;
                        muzzleflashsize = 2;
                        break;

                      case shotgun:
                        smokesize = 5;
                        muzzleflashsize = 0.8;
                        break;

                      case assaultrifle:
                        smokesize = 6;
                        muzzleflashsize = 1;
                        break;

                      case handgun1:
                      case handgun2:
                        smokesize = 6;
                        muzzleflashsize = 1;
                        break;

                      default:
                        smokesize = muzzleflashsize = 0;
                        break;
                    }

                    if(smokesize && muzzleflashsize) {
                      sprites.MakeSprite(smokesprite, .4, 1, 1, 1, finalwallhit,
                                         velocity, smokesize);

                      sprites.MakeSprite(muzzleflashsprite, 1, 1, 1, 1,
                                         finalwallhit, velocity, muzzleflashsize);
                    }

                    float gLoc[3];

                    gLoc[0] = finalwallhit.x / soundscalefactor;
                    gLoc[1] = finalwallhit.y / soundscalefactor;
                    gLoc[2] = finalwallhit.z / soundscalefactor;

                    alSourcefv(gSourceID[wallhitsound], AL_POSITION, gLoc);

                    alSourcePlay(gSourceID[wallhitsound]);
                  }

                  lastshot[0] = start;
                  lastshot[1] = oldend;

                  XYZ velocity = aim * 8;

                  if(person[j].weapon.type != sniperrifle
                     && person[j].weapon.type != shotgun && p == numshots - 1)
                    sprites.MakeSprite(smokesprite, .3, 1, 1, 1,
                                       start + aim * 1.5, velocity, 3);

                  if(person[j].weapon.type == shotgun && p == numshots - 1)
                    sprites.MakeSprite(smokesprite, .4, 1, 1, 1,
                                       start + aim * 1.5, velocity, 5);

                  if(person[j].weapon.type == sniperrifle && !zoom)
                    sprites.MakeSprite(smokesprite, .3, 1, 1, 1,
                                       start + aim * 2.2, velocity, 4);

                  if(j != 0 || zoom == 0)
                    sprites.MakeSprite(bullet, .07, 1, 1, .7,
                                       lastshot[0] + aim * 1, lastshot[1], .2);

                  //Nearby bullet whoosh          
                  long dot_ta, dot_tb;
                  XYZ *a, *b, *c, nearest;

                  a = &lastshot[0];
                  *a += aim * 1;

                  b = &lastshot[1];

                  c = &camera.position;

                  nearest = 0;

                  dot_ta = (long)
                      ((c->x - a->x) * (b->x - a->x) + (c->y - a->y) * (b->y -
                                                                       a->y) +
                      (c->z - a->z) * (b->z - a->z));

                  dot_tb = (long)
                      ((c->x - b->x) * (a->x - b->x) + (c->y - b->y) * (a->y -
                                                                       b->y) +
                      (c->z - b->z) * (a->z - b->z));

                  if(!dot_ta <= 0 && !dot_tb <= 0) {
                    nearest.x =
                        a->x + ((b->x - a->x) * dot_ta) / (dot_ta + dot_tb);

                    nearest.y =
                        a->y + ((b->y - a->y) * dot_ta) / (dot_ta + dot_tb);

                    nearest.z =
                        a->z + ((b->z - a->z) * dot_ta) / (dot_ta + dot_tb);
                  }

                  if(nearest.x) {

                    if(findDistancefast(nearest, camera.position) < 10
                       && (thirdperson == 2 || j != 0)) {

                      float gLoc[3];

                      gLoc[0] =
                          (camera.position.x +
                           (nearest.x - camera.position.x)) / soundscalefactor;

                      gLoc[1] =
                          (camera.position.y +
                           (nearest.y - camera.position.y)) / soundscalefactor;

                      gLoc[2] =
                          (camera.position.z +
                           (nearest.z - camera.position.z)) / soundscalefactor;

                      alSourcefv(gSourceID[nearbulletsound], AL_POSITION, gLoc);
                      alSourcePlay(gSourceID[nearbulletsound]);
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
}

void Game::SpawnPeople()
{
    //Spawn people
    spawndelay -= multiplier;

    int blockspawnx = 0, blockspawny = 0;
    int cyclenum = 0;

    while((citypeoplenum[blockspawnx][blockspawny] >= max_people_block
           && cyclenum < 10) || blockspawnx == 0) {

      blockspawnx =
          (int)(((person[0].playercoords.x +
                  block_spacing / 2) / block_spacing) + Random() % 2);

      blockspawny =
          (int)(((person[0].playercoords.z +
                  block_spacing / 2) / block_spacing) + Random() % 2);

      cyclenum++;
    }

    if(cyclenum < 10) {

      if(spawndelay < 0 && numpeople < max_people) {
        if(type == randomshoot_type) {
          if(abs(Random() % evilprobability) == 0)
            person[numpeople].type = eviltype;
          else
            person[numpeople].type = civiliantype;
        }

        if(type == zombie_type) {
          person[numpeople].type = zombietype;
        }

        if(person[numpeople].type != civiliantype
           && blockspawnx == person[1].whichblockx
           && blockspawny == person[1].whichblocky) {

          while((citypeoplenum[blockspawnx][blockspawny] >= max_people_block
                 && cyclenum < 10) || blockspawnx == 0
                || (blockspawnx == person[1].whichblockx
                    && blockspawny == person[1].whichblocky)) {

            blockspawnx =
                (int)((person[0].playercoords.x +
                       block_spacing / 2) / block_spacing) + Random() % 2;

            blockspawny =
                (int)((person[0].playercoords.z +
                       block_spacing / 2) / block_spacing) + Random() % 2;

            cyclenum++;
          }
        }

        person[numpeople].playerrotation = 0;
        person[numpeople].whichcostume =
            casualcostumes + abs(Random()) % numcasual;

        person[numpeople].whichblockx = blockspawnx;
        person[numpeople].whichblocky = blockspawny;

        person[numpeople].pathnum = -1;
        person[numpeople].oldpathnum = -1;
        person[numpeople].oldoldpathnum = -1;
        person[numpeople].oldoldoldpathnum = -1;

        while(person[numpeople].pathnum < 0
              || person[numpeople].pathnum >= path.vertexNum
              || person[numpeople].pathnum == 1) {

          person[numpeople].pathnum = Random() % path.vertexNum;
        }

        person[numpeople].pathtarget.x =
            path.vertex[person[numpeople].pathnum].x;
        person[numpeople].pathtarget.z =
            path.vertex[person[numpeople].pathnum].z;
        person[numpeople].pathsize = .98 + float (abs(Random() % 20)) / 400;
        person[numpeople].pathtarget *= person[numpeople].pathsize;
        person[numpeople].pathtarget.x +=
            person[numpeople].whichblockx * block_spacing;
        person[numpeople].pathtarget.z +=
            person[numpeople].whichblocky * block_spacing;

        person[numpeople].playercoords = person[numpeople].pathtarget;
        person[numpeople].oldplayercoords = person[numpeople].playercoords;
        person[numpeople].skeleton.free = 0;

        person[numpeople].targetanimation = walkanim;

        if(person[numpeople].type == zombietype)
          person[numpeople].targetanimation = zombiewalkanim;

        person[numpeople].speed = 1;
        person[numpeople].existing = 0;
        person[numpeople].speedmult = .8 + float (abs(Random() % 20)) / 50;

        person[numpeople].health = 100;
        person[numpeople].maxhealth = 100;
        person[numpeople].playerrotation2 = 0;

        person[numpeople].lastdistancevictim = 200000;

        if(person[numpeople].skeleton.broken)
          person[numpeople].skeleton.
              Load((char *)":Data:Skeleton:Basic Figure");

        if(numpeople == 1)
          person[numpeople].type = viptype;

        ///killtarget appears to be the target for the AI to attack
        // 1 - target
        // -1 - not target
        person[numpeople].killtarget = -1;

        if(person[numpeople].type == eviltype) {
          person[numpeople].existing = 1;
          person[numpeople].pathsize = 1.04;
          person[numpeople].weapon.type = nogun;
          person[numpeople].aiming = 1;
          person[numpeople].killtarget = -1;
          person[numpeople].speedmult = 1 + .3 * difficulty;
        }

        if(person[numpeople].type == zombietype) {
          person[numpeople].existing = 1;
          person[numpeople].pathsize = 1.04;
          person[numpeople].weapon.type = nogun;
          person[numpeople].aiming = 0;
          person[numpeople].killtarget = -1;
          person[numpeople].speedmult = 0.7 + .2 * difficulty;
        } else {
          person[numpeople].weapon.type = nogun;
          person[numpeople].aiming = 0;
          person[numpeople].killtarget = -1;
        }

        if(person[numpeople].type == viptype) {
          person[numpeople].existing = 1;
        }

        if(enemystate == 2)
          person[numpeople].killtarget = 1;

        numpeople++;
        citypeoplenum[blockspawnx][blockspawny]++;
        spawndelay = .1;
      }

      if(spawndelay < 0 && numpeople >= max_people) {

        if(cycle >= max_people)
          cycle = 0;

        cyclenum = 0;

        while(cyclenum < max_people
              && (person[cycle].type == playertype
                  || person[cycle].type == viptype
                  ||
                  (abs
                   (person[cycle].whichblockx -
                    ((person[0].playercoords.x +
                      block_spacing / 2) / block_spacing)) < 2
                   && abs(person[cycle].whichblocky -
                          ((person[0].playercoords.z +
                            block_spacing / 2) / block_spacing)) < 2))) {

          cycle++;
          cyclenum++;

          if(cycle >= max_people)
            cycle = 0;
        }

        if(cycle < max_people && cyclenum < max_people) {
          if(type == randomshoot_type) {
            if(abs(Random() % evilprobability) == 0)
              person[cycle].type = eviltype;
            else
              person[cycle].type = civiliantype;
          }

          if(type == zombie_type) {
            person[cycle].type = zombietype;
          }

          if(person[cycle].type != civiliantype
             && blockspawnx == person[1].whichblockx
             && blockspawny == person[1].whichblocky) {

            while((citypeoplenum[blockspawnx][blockspawny] >= max_people_block
                   && cyclenum < 10) || blockspawnx == 0
                  || (blockspawnx == person[1].whichblockx
                      && blockspawny == person[1].whichblocky)) {

              blockspawnx =
                  (int)((person[0].playercoords.x +
                         block_spacing / 2) / block_spacing) + Random() % 2;

              blockspawny =
                  (int)((person[0].playercoords.z +
                         block_spacing / 2) / block_spacing) + Random() % 2;

              cyclenum++;
            }
          }

          person[cycle].playerrotation = 0;

          person[cycle].whichcostume =
              casualcostumes + abs(Random()) % numcasual;

          citypeoplenum[person[cycle].whichblockx][person[cycle].whichblocky]--;

          person[cycle].whichblockx = blockspawnx;
          person[cycle].whichblocky = blockspawny;

          person[cycle].pathnum = 
            person[cycle].oldpathnum = 
            person[cycle].oldoldpathnum = 
            person[cycle].oldoldoldpathnum = -1;

          while(person[cycle].pathnum < 0
                || person[cycle].pathnum >= path.vertexNum
                || person[cycle].pathnum == 1) {

            person[cycle].pathnum = Random() % path.vertexNum;

          }

          person[cycle].pathtarget.x = path.vertex[person[cycle].pathnum].x;
          person[cycle].pathtarget.z = path.vertex[person[cycle].pathnum].z;
          person[cycle].pathsize = .98 + float (abs(Random() % 20)) / 400;
          person[cycle].pathtarget *= person[cycle].pathsize;
          person[cycle].pathtarget.x +=
              person[cycle].whichblockx * block_spacing;
          person[cycle].pathtarget.z +=
              person[cycle].whichblocky * block_spacing;


          person[cycle].playercoords = person[cycle].pathtarget;
          person[cycle].oldplayercoords = person[cycle].playercoords;
          person[cycle].skeleton.free = 0;


          person[cycle].targetanimation = walkanim;

          if(person[cycle].type == zombietype)
            person[cycle].targetanimation = zombiewalkanim;

          person[cycle].speed = 1;
          person[cycle].existing = 0;
          person[cycle].speedmult = .8 + float (abs(Random() % 20)) / 50;

          person[cycle].health = 100;
          person[cycle].maxhealth = 100;
          person[cycle].playerrotation2 = 0;

          if(person[cycle].skeleton.broken)
            person[cycle].skeleton.Load((char *)":Data:Skeleton:Basic Figure");

          if(enemystate == 1)
            person[cycle].killtarget = -1;

          if(person[cycle].type == eviltype) {
            person[cycle].existing = 1;
            person[cycle].pathsize = 1.04;
            person[cycle].weapon.type = nogun;
            person[cycle].aiming = 1;
            person[cycle].killtarget = -1;
            person[cycle].speedmult = 1 + .3 * difficulty;
          }

          if(person[cycle].type == zombietype) {
            person[cycle].existing = 1;
            person[cycle].pathsize = 1.04;
            person[cycle].weapon.type = nogun;
            person[cycle].aiming = 0;
            person[cycle].killtarget = -1;
            person[cycle].speedmult = .7 + .2 * difficulty;
          } else {
            person[cycle].weapon.type = nogun;
            person[cycle].aiming = 0;
            person[cycle].killtarget = -1;
          }

          person[cycle].lastdistancevictim = 200000;

          if(enemystate == 2)
            person[cycle].killtarget = 1;

          if(numpeople < max_people)
            numpeople++;

          citypeoplenum[blockspawnx][blockspawny]++;

          cycle++;

          spawndelay = .1;
        }
      }
    }
}

void Game::GameTick()
{
    int visions = config.visions;
    bool killedinnocent = 0;           //Haven't shot any civilians yet...

    XYZ flatfacing;
    murderer = 0;

    if(person[1].health <= 0 || person[0].health <= 0 || killedinnocent) {
      losedelay -= multiplier / 6;
    }

    if(person[1].health > 0 && person[0].health > 0 && !killedinnocent)
      timeremaining -= multiplier * 25 / 40;

    if(timeremaining <= 0) {
      flashamount = 1;
      flashr = 0;
      flashg = 1;
      flashb = 0;

      mission++;

      alSourcePlay(gSourceID[souloutsound]);

      score += 100 + (mission * 50);

      if(mission >= nummissions) {
        config.beatgame = 1;
        state = MAINMENU;
        gameinprogress = 0;

        alSourcePause(gSourceID[rainsound]);

        alSourceStop(gSourceID[visionsound]);
        alSourceStop(gSourceID[whichsong]);

        alSourcef(gSourceID[whichsong], AL_MIN_GAIN, 0);
        alSourcef(gSourceID[whichsong], AL_MAX_GAIN, 0);

        whichsong = mainmenusong;

        alSourceStop(gSourceID[whichsong]);

        alSourcef(gSourceID[whichsong], AL_PITCH, 1);

        alSourcePlay(gSourceID[whichsong]);

        alSourcef(gSourceID[whichsong], AL_MIN_GAIN, 1);
        alSourcef(gSourceID[whichsong], AL_MAX_GAIN, 1);

        config.WriteHighScore(score);
      }

      if(state != MAINMENU) {
        InitGame();

        if(environment.type == rainy_environment)
          alSourcePlay(gSourceID[rainsound]);

        if(environment.type != rainy_environment)
          alSourcePause(gSourceID[rainsound]);

        alSourceStop(gSourceID[visionsound]);
        alSourceStop(gSourceID[whichsong]);

        if(person[0].weapon.type == knife)
          whichsong = knifesong;
        else
          whichsong = shootsong;

        if(type == zombie_type)
          whichsong = zombiesong;

        alSourcef(gSourceID[whichsong], AL_PITCH, 1);
        alSourcePlay(gSourceID[whichsong]);
        alSourcef(gSourceID[whichsong], AL_MIN_GAIN, 1);
        alSourcef(gSourceID[whichsong], AL_MAX_GAIN, 1);
      }
    }

    // when you die or kill an innocent
    if(losedelay <= 0) {
      flashamount = 1;
      flashr = 0;
      flashg = 0;
      flashb = 0;

      if(person[murderer].health > 0)
        score = oldscore - 200; //kill innocent
      else
        score = oldscore - 100; //die

      InitGame();

      alSourceStop(gSourceID[whichsong]);

      if(person[0].weapon.type == knife)
        whichsong = knifesong;
      else
        whichsong = shootsong;

      if(type == zombie_type)
        whichsong = zombiesong;

      alSourcef(gSourceID[whichsong], AL_PITCH, 1);

      alSourcePlay(gSourceID[whichsong]);

      alSourcef(gSourceID[whichsong], AL_MIN_GAIN, 1);
      alSourcef(gSourceID[whichsong], AL_MAX_GAIN, 1);

      alSourcePlay(gSourceID[soulinsound]);
    }

    //unsigned char theKeyMap[16];
    //GetKeys((unsigned long *)theKeyMap);

    //Sprites
    sprites.DoStuff();

    //Decals
    decals.DoStuff();

    //Facing
    facing = 0;
    facing.z = -1;

    facing = DoRotation(facing, -camera.rotation2, 0, 0);
    facing = DoRotation(facing, 0, 0 - camera.rotation, 0);

    flatfacing = facing;
    flatfacing.y = 0;
    Normalise(&flatfacing);

    if(IsKeyDown(theKeyMap, psychickey) && !oldvisionkey) {
      oldvisionkey = 1;
      visions = 1 - visions;

      if(visions == 0) {
        flashamount = 1;
        flashr = 1;
        flashg = 1;
        flashb = 1;

        //if the player hasn't moved?
        if(person[0].playercoords.x != bodycoords.x
           || person[0].playercoords.z != bodycoords.z) {

          XYZ towards;

          towards = person[0].playercoords - bodycoords;

          Normalise(&towards);

          camera.rotation = -asin(0 - towards.x);
          camera.rotation *= 360 / 6.28;

          if(towards.z > 0)
            camera.rotation = 180 - camera.rotation;

          camera.visrotation = camera.rotation;
          camera.oldrotation = camera.rotation;
          camera.oldoldrotation = camera.rotation;
        }

        person[0].playercoords = bodycoords;
        person[0].oldplayercoords = bodycoords;
        person[0].velocity = 0;

        alSourcePlay(gSourceID[soulinsound]);
        alSourceStop(gSourceID[visionsound]);
        alSourcef(gSourceID[knifesong], AL_PITCH, (ALfloat) (1));
        alSourcef(gSourceID[shootsong], AL_PITCH, (ALfloat) (1));
        alSourcef(gSourceID[zombiesong], AL_PITCH, (ALfloat) (1));
      }

      if(visions == 1) {
        alSourceStop(gSourceID[visionsound]);
        flashamount = 1;
        flashr = 1;
        flashg = 0;
        flashb = 0;

        bodycoords = person[0].oldplayercoords;

        alSourcePlay(gSourceID[souloutsound]);

        alSourcef(gSourceID[knifesong], AL_PITCH, (ALfloat) (.5));
        alSourcef(gSourceID[shootsong], AL_PITCH, (ALfloat) (.5));
        alSourcef(gSourceID[zombiesong], AL_PITCH, (ALfloat) (.5));

        alSourcePlay(gSourceID[visionsound]);
      }
    }

    if(!IsKeyDown(theKeyMap, psychickey)) {
      oldvisionkey = 0;
    }

    int thirdperson = config.thirdperson;

    if(IsKeyDown(theKeyMap, MAC_TAB_KEY) && !tabkeydown) {
      thirdperson = 1 - thirdperson;
      config.thirdperson = thirdperson;
      if(thirdperson) {
        lasersight = 1;
      } else {
        lasersight = 0;
      }

      tabkeydown = 1;
    }

    if(!IsKeyDown(theKeyMap, MAC_TAB_KEY))
      tabkeydown = 0;

    if(IsKeyDown(theKeyMap, aimkey) && !aimtoggle) {
      person[0].aiming = 1 - person[0].aiming;
      aimtoggle = 1;
    }

    if(!IsKeyDown(theKeyMap, aimkey))
      aimtoggle = 0;

    if(IsKeyDown(theKeyMap, MAC_R_KEY) && !reloadtoggle) {

      if(person[0].reloads[person[0].weapon.type] > 0 && person[0].weapon.reloading <= 0)
        person[0].weapon.ammo = -1;

      reloadtoggle = 1;
    }

    if(!IsKeyDown(theKeyMap, MAC_R_KEY))
      reloadtoggle = 0;

    if(IsKeyDown(theKeyMap, psychicaimkey) && !slomokeydown && slomo == 0) {
      alSourcePlay(gSourceID[souloutsound]);

      slomo = 2;
      slomokeydown = 1;

      flashamount = .5;
      flashr = 1;
      flashg = 0;
      flashb = 0;

      score -= 20;

      alSourcef(gSourceID[knifesong], AL_PITCH, (ALfloat) (.5));
      alSourcef(gSourceID[shootsong], AL_PITCH, (ALfloat) (.5));
      alSourcef(gSourceID[zombiesong], AL_PITCH, (ALfloat) (.5));
    }

    if(!IsKeyDown(theKeyMap, psychicaimkey))
      slomokeydown = 0;

    //Mouse look
    if((person[0].aimamount <= 0 && person[0].targetanimation != crouchanim)) {
      camera.rotation = camera.visrotation;
      camera.rotation2 = camera.visrotation2;

      mousesensitivity = config.usermousesensitivity;
    }

    if(person[0].aimamount >= 1 && zoom == 0) {
      mousesensitivity = config.usermousesensitivity * .8;
    }

    if(slomo == 2) {
      mousesensitivity *= .6;
    }

    camera.MouseRotate(mousesensitivity);
    camera.Smooth();

    if(zoom || person[0].aimamount <= 0 || person[0].weapon.type == nogun
       || visions || person[0].weapon.type == grenade
       || person[0].weapon.type == knife) {

      camera.visrotation = camera.rotation;
      camera.visrotation2 = camera.rotation2;
    }

    oldzoom = zoom;

    checkCollisions();

    // Tackle (run + space)
    if(IsKeyDown(theKeyMap, MAC_SPACE_KEY)) {
      if(visions == 0 && person[0].targetanimation == joganim
         && person[0].currentanimation == joganim
         && person[0].backwardsanim == 0
         && person[0].playerrotation == person[0].playerlowrotation) {

        person[0].targetanimation = diveanim;
        person[0].targetframe = 0;
        person[0].target = 0;
        person[0].aimamount = 0;
      }
    }

    camera.Update();

    SpawnPeople();

    XYZ towards;
    XYZ finaltarget;
    finaltarget = 0.0f;
    XYZ blah;

    int closesttarget = 0;

    float leastdistance = 0.0f;

    XYZ bleedloc;
    XYZ vel;

    float tooclose;
    float toofar;

    //People
    for(int i = 0; i < numpeople; i++) {
      if(person[i].type != zombietype &&
          person[i].health < 0 && person[i].longdead < 0
         && person[i].firstlongdead == 0) {

          overpoint =
              person[i].skeleton.joints[person[i].skeleton.
                                        jointlabels[abdomen]].position;

          overpoint.y += 3000;

          underpoint =
              person[i].skeleton.joints[person[i].skeleton.
                                        jointlabels[abdomen]].position;

          underpoint.y -= 3000;

          move = 0;
          move.x = person[i].whichblockx * block_spacing;
          move.z = person[i].whichblocky * block_spacing;

          XYZ temp;

          whichtri =
              sidewalkcollide.LineCheck2(overpoint, underpoint, &temp, move,
                                         cityrotation[person[i].
                                                      whichblockx][person[i].
                                                                   whichblocky]
                                         * 90);

          XYZ normish;

          normish = 0;
          normish.y = 1;
          // blood pool near where they were hit
          if(whichtri >= 0) {
            decals.MakeDecal(bloodpool, temp, 12, normish, whichtri,
                             &sidewalkcollide, move,
                             cityrotation[person[i].whichblockx][person[i].
                                                                 whichblocky] *
                             90);
          }

          if(whichtri == -1) {
            temp =
                person[i].skeleton.joints[person[i].skeleton.
                                          jointlabels[abdomen]].position;

            temp.y = -.5;
            move = 0;

            decals.MakeDecal(bloodpool, temp, 12, normish, 0, &sidewalkcollide,
                             move, 0);
          }

          person[i].firstlongdead = 1;
      }

      if(person[i].health < 100 && person[i].type != zombietype) {
        person[i].health -= multiplier * 120;
      }

      if(person[i].health < 100 && person[i].type == zombietype) {
        person[i].health += multiplier * 10;

        if(person[i].health > person[i].maxhealth)
          person[i].health = person[i].maxhealth;

      }
      ///alter zombie properties bases on health
      if(person[i].health < 100 && person[i].type == zombietype
         && person[i].skeleton.free == 1) {
        /// multiplier ~0.01 * 10 = 0.1 health per tick
        person[i].health += multiplier * 10;

        if(person[i].health > person[i].maxhealth)
          person[i].health = person[i].maxhealth;

      }
     
      if(person[i].health < 40 && person[i].type == zombietype) {

        person[i].speedmult -= multiplier / 20;

        if(person[i].speedmult < .6) {
          person[i].speedmult = .6;
          person[i].killtarget = -1;
        }
      }

      if(person[i].health >= 40 && person[i].type == zombietype) {

        person[i].speedmult += multiplier / 40;

        if(person[i].speedmult > .7 + difficulty * .2) {
          person[i].speedmult = .7 + difficulty * .2;
          person[i].killtarget = 1;
        }
      }

      if(person[i].maxhealth < 100 && person[i].type == zombietype) {
        person[i].maxhealth += multiplier * 2;

        if(person[i].maxhealth > 100)
          person[i].maxhealth = 100;
      }

      if(person[i].bleeding > 0) {
        person[i].bleeding -= multiplier;
        person[i].bleeddelay -= multiplier * 10;

        if(person[i].bleeddelay <= 0) {
          person[i].bleeddelay = 1;

          if(person[i].skeleton.free == 0) {

            bleedloc =
                DoRotation((person[i].bjoint1->position +
                            person[i].bjoint2->position) / 2, 0,
                           person[i].playerrotation,
                           0) + person[i].playercoords;

          }

          if(person[i].skeleton.free > 0) {
            bleedloc =
                (person[i].bjoint1->position + person[i].bjoint2->position) / 2;
          }

          vel = 0;

          sprites.MakeSprite(bloodspritedown, .6, 1, .2, .2, bleedloc, vel,
                             3 * person[i].bleeding);
        }
      }

      if(person[i].skeleton.free == 0) {

        //Gun
        if(person[i].type == playertype || person[i].type == eviltype) {

          if(i == 0) { //bodyguard
            switch(person[i].weapon.type) {
              case shotgun:
                person[i].weapon.recoil -= multiplier * 4;
                break;
              case sniperrifle:
                person[i].weapon.recoil -= multiplier * 2;
                break;
              case handgun1:
              case handgun2:
                person[i].weapon.recoil -= multiplier * 5;
                break;
              case assaultrifle:
                person[i].weapon.recoil -= multiplier * 10;
                break;
            }
          } else {
            switch(person[i].weapon.type) {
              case shotgun:
              case sniperrifle:
                person[i].weapon.recoil -= multiplier * 1;
                break;
              case handgun1:
              case handgun2:
                person[i].weapon.recoil -= multiplier * 2;
                break;
              case assaultrifle:
                person[i].weapon.recoil -= multiplier * 10;
                break;
            }
          }

          if(person[i].weapon.recoil < 0)
            person[i].weapon.recoil = 0;

          if(i == 0) {
            if(zoom) {
              mousesensitivity = .05 * config.usermousesensitivity;

              if(person[i].targetanimation != crouchanim
                 || person[i].currentanimation != crouchanim
                 || person[i].aiming < 1) {

                zoom = 0;
              }

              if(visions == 1)
                zoom = 0;
            }

            if(person[i].currentanimation == crouchanim
               && person[i].targetanimation == crouchanim
               && person[i].aiming >= 1 && person[i].weapon.type == sniperrifle) {

              zoom = 1;

              if(zoom && !oldzoom)
                camera.rotation2 -= 6;

            }

            if(!zoom)
              mousesensitivity = 1 * config.usermousesensitivity;

            if(person[i].weapon.type != sniperrifle)
              zoom = 0;

          }
        }
        //Zombie eat
        if(i > 0 && person[person[i].killtarget].eaten == i
           && person[i].targetanimation == zombieeatanim) {

          person[person[i].killtarget].skeleton.
              joints[person[person[i].killtarget].skeleton.jointlabels[head]].
              locked = 1;

          person[person[i].killtarget].skeleton.
              joints[person[person[i].killtarget].skeleton.
                     jointlabels[rightshoulder]].locked = 1;

          for(int k = 0; k < 2; k++) {

            person[person[i].killtarget].skeleton.
                joints[person[person[i].killtarget].skeleton.jointlabels[head]].
                position =
                DoRotation(person[i].skeleton.
                           joints[person[i].skeleton.jointlabels[righthand]].
                           position, 0, person[i].playerrotation,
                           0) + person[i].playercoords;

            person[person[i].killtarget].skeleton.
                joints[person[person[i].killtarget].skeleton.jointlabels[head]].
                velocity = 0;

            person[person[i].killtarget].skeleton.
                joints[person[person[i].killtarget].skeleton.
                       jointlabels[rightshoulder]].position =
                DoRotation(person[i].skeleton.
                           joints[person[i].skeleton.jointlabels[lefthand]].
                           position, 0, person[i].playerrotation,
                           0) + person[i].playercoords;

            person[person[i].killtarget].skeleton.
                joints[person[person[i].killtarget].skeleton.
                       jointlabels[rightshoulder]].velocity = 0;

            person[person[i].killtarget].skeleton.DoConstraints();

            person[person[i].killtarget].skeleton.
                DoConstraints(&blocksimplecollide
                              [citytype[person[i].whichblockx]
                               [person[i].whichblocky]], &move,
                              cityrotation[person[i].whichblockx][person[i].
                                                                  whichblocky] *
                              90);
          }

          person[person[i].killtarget].skeleton.
              joints[person[person[i].killtarget].skeleton.jointlabels[head]].
              locked = 0;

          person[person[i].killtarget].skeleton.
              joints[person[person[i].killtarget].skeleton.
                     jointlabels[rightshoulder]].locked = 0;

          person[person[i].killtarget].longdead = 1;
        }

        if(i > 0 && enemystate != 1 && person[i].type == zombietype
           && person[i].speedmult > .7) {

          if(findDistancefast(person[i].playercoords, person[1].playercoords) <
             20000)
            person[i].killtarget = 1; //vip
          else
            person[i].killtarget = -1; //none
        }

        if(i > 0 && enemystate != 1 && person[i].type == zombietype
           && person[i].speedmult < .7) {

          person[i].killtarget = -1;
        }

        bool realcheck = false;

        //Pathfinding
        if(i > 0 && person[i].targetanimation != getupfrontanim
           && person[i].targetanimation != thrownanim
           && person[i].targetanimation != getupbackanim
           && person[i].currentanimation != getupfrontanim
           && person[i].currentanimation != getupbackanim) {

          person[i].pathcheckdelay -= multiplier;

          // Realcheck tells us
          // a) we've got close to the end of our path or
          // b) we're moving away from our target
          realcheck =
              (abs(person[i].playercoords.x - person[i].pathtarget.x) < 1
               && abs(person[i].playercoords.z - person[i].pathtarget.z) < 1)
              || findDistancefast(person[i].playercoords,
                                  person[i].pathtarget) >
              findDistancefast(person[i].oldplayercoords, person[i].pathtarget);

          if(person[i].targetanimation == idleanim
             && person[i].killtargetvisible == 0) {

            person[i].targetanimation = walkanim;

            if(person[i].type == zombietype)
              person[i].targetanimation = zombiewalkanim;

            realcheck = 1;
          }

          if((realcheck
              || ((person[i].killtarget > -1 && person[i].type != civiliantype)
                  && person[i].pathcheckdelay <= 0)
              || person[i].killtargetvisible)) {

            person[i].pathcheckdelay = 1.2;

            if((person[i].killtarget == -1 || person[i].type == civiliantype)
               && !person[i].running) {

              person[i].killtargetvisible = 0;

              leastdistance = 2000000;

              for(int j = 0; j < path.vertexNum; j++) {

                person[i].pathtarget.x = path.vertex[j].x;
                person[i].pathtarget.z = path.vertex[j].z;
                person[i].pathtarget.y = path.vertex[j].y;

                person[i].pathtarget *= person[i].pathsize;
                person[i].pathtarget.x += person[i].whichblockx * block_spacing;
                person[i].pathtarget.z += person[i].whichblocky * block_spacing;

                if(findDistancefast
                   (person[i].playercoords,
                    person[i].pathtarget) < leastdistance && j != 1
                   && j != person[i].oldpathnum && j != person[i].oldoldpathnum
                   && j != person[i].oldoldoldpathnum) {

                  leastdistance =
                      findDistancefast(person[i].playercoords,
                                       person[i].pathtarget);

                  closesttarget = j;
                }
              }

              if(closesttarget >= 0 && closesttarget < path.vertexNum) {

                person[i].oldoldoldpathnum = person[i].oldoldpathnum;
                person[i].oldoldpathnum = person[i].oldpathnum;
                person[i].oldpathnum = person[i].pathnum;
                person[i].pathnum = closesttarget;

                person[i].pathtarget.x = path.vertex[person[i].pathnum].x;
                person[i].pathtarget.z = path.vertex[person[i].pathnum].z;
                person[i].pathtarget *= person[i].pathsize;
                person[i].pathtarget.x += person[i].whichblockx * block_spacing;
                person[i].pathtarget.z += person[i].whichblocky * block_spacing;
              }
            }

            if(person[i].running && realcheck) {

              person[i].killtargetvisible = 0;

              person[i].targetanimation = joganim;

              //Dead target?
              if(person[person[i].killtarget].health <= 0) {
                person[i].running = 0;
              }

              person[i].killtarget = 1;

              //If pathfind
              if(realcheck) {

                leastdistance = 2000000;

                person[i].lastdistancevictim = 0;

                closesttarget = -1;

                //Check best path
                closesttarget = person[i].pathnum;

                //Check other blocks?
                if((closesttarget == person[i].pathnum)) {

                  beginx = person[i].whichblockx - 2;

                  if(beginx < 0)
                    beginx = 0;

                  beginz = person[i].whichblocky - 2;

                  if(beginz < 0)
                    beginz = 0;

                  endx = person[i].whichblockx + 2;

                  if(endx > num_blocks - 1)
                    endx = num_blocks - 1;

                  endz = person[i].whichblocky + 2;

                  if(endz > num_blocks - 1)
                    endz = num_blocks - 1;

                  leastdistance = 2000000;

                  for(int l = beginx; l <= endx; l++) {

                    for(int m = beginx; m <= endx; m++) {

                      for(int j = 0; j < path.vertexNum; j++) {

                        person[i].pathtarget.x = path.vertex[j].x;
                        person[i].pathtarget.y = path.vertex[j].y;
                        person[i].pathtarget.z = path.vertex[j].z;

                        person[i].pathtarget *= person[i].pathsize;
                        person[i].pathtarget.x += l * block_spacing;
                        person[i].pathtarget.z += m * block_spacing;

                        if(findDistancefast
                           (person[i].playercoords,
                            person[i].pathtarget) < leastdistance
                           && findDistancefast(person[i].pathtarget,
                                               person[person[i].killtarget].
                                               playercoords) >
                           findDistancefast(person[i].playercoords,
                                            person[person[i].killtarget].
                                            playercoords) && j != 1
                           && blocksimple.LineCheck2(person[i].playercoords,
                                                     person[i].pathtarget,
                                                     &blah, move,
                                                     cityrotation[person[i].
                                                                  whichblockx]
                                                     [person[i].whichblocky]) ==
                           -1
                           && blocksimple.LineCheck2(person[i].playercoords,
                                                     person[i].pathtarget,
                                                     &blah, move,
                                                     cityrotation[l][m]) ==
                           -1) {

                          person[i].lastdistancevictim =
                              findDistancefast(person[i].pathtarget,
                                               person[person[i].killtarget].
                                               playercoords);

                          leastdistance =
                              findDistancefast(person[i].playercoords,
                                               person[i].pathtarget);

                          closesttarget = j;

                          finaltarget = person[i].pathtarget;

                          person[i].whichblockx = l;

                          person[i].whichblocky = m;
                        }
                      }
                    }
                  }
                }

                if(closesttarget != -1) {

                  person[i].pathnum = closesttarget;

                  person[i].pathtarget = finaltarget;
                }
              }
            }
            //Assassin
            if((person[i].killtarget > -1 && person[i].type != civiliantype)
               && !person[i].running) {

              //Dead target?
              if(person[person[i].killtarget].health <= 0
                 && person[i].type == eviltype) {

                person[i].playerrotation2 = 0;
                person[i].weapon.type = nogun;
                person[i].targetanimation = walkanim;
                person[i].lastdistancevictim = 200000;
                person[i].pathnum = -1;

                enemystate = 1;
                person[i].killtarget = -1;
                realcheck = 1;
              }

              if(person[i].type == zombietype
                 && person[person[i].killtarget].health <= 0) {

                if(person[person[i].killtarget].eaten != i) {

                  person[i].playerrotation2 = 0;
                  person[i].targetanimation = zombiewalkanim;
                  person[i].lastdistancevictim = 200000;
                  person[i].pathnum = -1;

                  realcheck = 1;

                  person[i].killtarget = -1;
                }

                if(person[person[i].killtarget].eaten == i
                   && person[i].targetanimation != zombieeatanim) {

                  person[i].targetanimation = zombieeatanim;
                  person[i].targetframe = 0;
                  person[i].target = 0;
                }

                enemystate = 1;
              }

              if(person[person[i].killtarget].health > 0) {

                if(person[person[i].killtarget].skeleton.free) {

                  person[person[i].killtarget].playercoords =
                      person[person[i].killtarget].averageloc;
                }

                //If pathfind
                if(realcheck) {

                  leastdistance = 2000000;
                  person[i].lastdistancevictim = 2000000;
                  closesttarget = -1;

                  //Check best path
                  for(int j = 0; j < path.vertexNum; j++) {

                    person[i].pathtarget.x = path.vertex[j].x;
                    person[i].pathtarget.z = path.vertex[j].z;
                    person[i].pathtarget.y = path.vertex[j].y;

                    person[i].pathtarget *= person[i].pathsize;

                    person[i].pathtarget.x +=
                        person[i].whichblockx * block_spacing;

                    person[i].pathtarget.z +=
                        person[i].whichblocky * block_spacing;

                    if(findDistancefast
                       (person[i].playercoords,
                        person[i].pathtarget) < leastdistance
                       && findDistancefast(person[i].pathtarget,
                                           person[person[i].killtarget].
                                           playercoords) <
                       person[i].lastdistancevictim && j != 1
                       && blocksimple.LineCheck2(person[i].playercoords,
                                                 person[i].pathtarget, &blah,
                                                 move,
                                                 cityrotation[person[i].
                                                              whichblockx]
                                                 [person[i].whichblocky]) == -1) {

                      leastdistance =
                          findDistancefast(person[i].playercoords,
                                           person[i].pathtarget);

                      person[i].lastdistancevictim =
                          findDistancefast(person[i].pathtarget,
                                           person[person[i].killtarget].
                                           playercoords);

                      closesttarget = j;

                      finaltarget = person[i].pathtarget;
                    }
                  }

                  leastdistance = 2000000;

                  for(int j = 0; j < path.vertexNum; j++) {

                    person[i].pathtarget.x = path.vertex[j].x;
                    person[i].pathtarget.z = path.vertex[j].z;
                    person[i].pathtarget.y = path.vertex[j].y;

                    person[i].pathtarget *= person[i].pathsize;

                    person[i].pathtarget.x +=
                        person[i].whichblockx * block_spacing;

                    person[i].pathtarget.z +=
                        person[i].whichblocky * block_spacing;

                    if(findDistancefast
                       (person[i].playercoords,
                        person[i].pathtarget) < leastdistance
                       && findDistancefast(person[i].pathtarget,
                                           person[person[i].killtarget].
                                           playercoords) <
                       person[i].lastdistancevictim && j != 1
                       && blocksimple.LineCheck2(person[i].playercoords,
                                                 person[i].pathtarget, &blah,
                                                 move,
                                                 cityrotation[person[i].
                                                              whichblockx]
                                                 [person[i].whichblocky]) == -1) {

                      leastdistance =
                          findDistancefast(person[i].playercoords,
                                           person[i].pathtarget);

                      person[i].lastdistancevictim =
                          findDistancefast(person[i].pathtarget,
                                           person[person[i].killtarget].
                                           playercoords);

                      closesttarget = j;

                      finaltarget = person[i].pathtarget;
                    }
                  }

                  //Check other blocks?
                  if((closesttarget == person[i].pathnum)
                     || closesttarget == -1) {

                    beginx = person[i].whichblockx - 2;

                    if(beginx < 0)
                      beginx = 0;

                    beginz = person[i].whichblocky - 2;

                    if(beginz < 0)
                      beginz = 0;

                    endx = person[i].whichblockx + 2;

                    if(endx > num_blocks - 1)
                      endx = num_blocks - 1;

                    endz = person[i].whichblocky + 2;

                    if(endz > num_blocks - 1)
                      endz = num_blocks - 1;

                    leastdistance = 2000000;

                    for(int l = beginx; l <= endx; l++) {

                      for(int m = beginx; m <= endx; m++) {

                        if(l != person[i].whichblockx
                           || m != person[i].whichblocky) {

                          for(int j = 0; j < path.vertexNum; j++) {

                            person[i].pathtarget.x = path.vertex[j].x;
                            person[i].pathtarget.y = path.vertex[j].y;
                            person[i].pathtarget.z = path.vertex[j].z;

                            person[i].pathtarget *= person[i].pathsize;
                            person[i].pathtarget.x += l * block_spacing;
                            person[i].pathtarget.z += m * block_spacing;

                            if(findDistancefast
                               (person[i].playercoords,
                                person[i].pathtarget) < leastdistance
                               && findDistancefast(person[i].pathtarget,
                                                   person[person[i].killtarget].
                                                   playercoords) <
                               findDistancefast(person[i].playercoords,
                                                person[person[i].killtarget].
                                                playercoords) && j != 1
                               && blocksimple.LineCheck2(person[i].playercoords,
                                                         person[i].pathtarget,
                                                         &blah, move,
                                                         cityrotation[l][m]) ==
                               -1
                               && blocksimple.LineCheck2(person[i].playercoords,
                                                         person[i].pathtarget,
                                                         &blah, move,
                                                         cityrotation[person[i].
                                                                      whichblockx]
                                                         [person[i].
                                                          whichblocky]) == -1) {

                              leastdistance =
                                  findDistancefast(person[i].playercoords,
                                                   person[i].pathtarget);

                              closesttarget = j;

                              finaltarget = person[i].pathtarget;

                              person[i].whichblockx = l;
                              person[i].whichblocky = m;
                            }
                          }
                        }
                      }
                    }
                  }

                  if(closesttarget != -1) {
                    person[i].onpath = 1;
                    person[i].pathnum = closesttarget;
                    person[i].pathtarget = finaltarget;
                  }
                }
                //Check killtargetvisible
                person[i].killtargetvisible = 1;

                if(person[person[i].killtarget].health <= 0)
                  person[i].killtargetvisible = 0;

                if(closesttarget != -1
                   && findDistancefast(person[i].playercoords,
                                       person[person[i].killtarget].
                                       playercoords) > 30000)
                  person[i].killtargetvisible = 0;

                if(person[i].killtarget == 0 && visions == 1)
                  person[i].killtargetvisible = 0;

                if(person[i].killtargetvisible) {

                  beginx = person[i].whichblockx - 2;

                  if(beginx < 0)
                    beginx = 0;

                  beginz = person[i].whichblocky - 2;

                  if(beginz < 0)
                    beginz = 0;

                  endx = person[i].whichblockx + 2;

                  if(endx > num_blocks - 1)
                    endx = num_blocks - 1;

                  endz = person[i].whichblocky + 2;

                  if(endz > num_blocks - 1)
                    endz = num_blocks - 1;

                  for(int l = beginx; l <= endx; l++) {

                    for(int m = beginx; m <= endx; m++) {

                      move.x = l * block_spacing;
                      move.z = m * block_spacing;
                      move.y = -3;

                      if(person[i].killtargetvisible) {

                        if(blocksimple.
                           LineCheck2(person[i].playercoords,
                                      person[person[i].killtarget].playercoords,
                                      &blah, move, cityrotation[l][m]) != -1) {

                          person[i].killtargetvisible = 0;
                        }
                      }
                    }
                  }
                }

                if(person[i].type == eviltype) {

                  if(!person[i].killtargetvisible
                     && person[i].targetanimation == idleanim) {

                    person[i].targetanimation = joganim;
                  }

                  if(!person[i].killtargetvisible) {
                    person[i].aiming = 0;
                  }

                  if(person[i].killtargetvisible) {

                    person[i].onpath = 0;
                    person[i].lastdistancevictim = 200000;
                    person[i].pathnum = -1;
                    //if no weapon, arm them
                    if(person[i].weapon.type == nogun) {

                      person[i].weapon.type =
                          possiblegun[abs(Random() % numpossibleguns)];

                      person[i].reloads[person[i].weapon.type] = 1;

                      if(person[i].weapon.type == knife)
                        person[i].speedmult = .8 + .5 * difficulty;
                    }

                    if(person[i].aiming == 0)
                      person[i].weapon.shotdelay = shotdelayamount / difficulty;

                    person[i].aiming = 1;

                    if(person[i].weapon.reloading > 0)
                      person[i].aiming = 0;

                    if(person[i].weapon.type == handgun1
                       || person[i].weapon.type == handgun2)
                      person[i].playerrotation2 = -10;

                    if(person[i].weapon.type == assaultrifle
                       || person[i].weapon.type == sniperrifle
                       || person[i].weapon.type == shotgun)
                      person[i].playerrotation2 = 20;

                    tooclose = 1300;
                    toofar = 3000;

                    switch(person[i].weapon.type) {
                      case shotgun:
                        tooclose = 1400;
                        toofar = 5000;
                        break;

                      case assaultrifle:
                        tooclose = 5000;
                        toofar = 9000;
                        break;

                      case sniperrifle:
                        tooclose = 10000;
                        toofar = 20000;
                        break;

                      case knife:
                        tooclose = 20;
                        toofar = 20000;
                        break;
                    }

                    if(findDistancefast
                       (person[i].playercoords,
                        person[person[i].killtarget].playercoords) > toofar)

                      person[i].targetanimation = joganim;

                    if((findDistancefast
                        (person[i].playercoords,
                         person[person[i].killtarget].playercoords) <= tooclose
                        && person[person[i].killtarget].skeleton.free == 0)
                       || (tooclose > 200
                           && findDistancefast(person[i].playercoords,
                                               person[person[i].killtarget].
                                               playercoords) <= 200)
                       || (tooclose <= 200
                           && findDistancefast(person[i].playercoords,
                                               person[person[i].killtarget].
                                               playercoords) < tooclose)) {

                      if(person[i].targetanimation != idleanim) {
                        person[i].targetanimation = idleanim;
                        person[i].targetframe = 0;
                        person[i].target = 0;
                      }

                      if(person[i].weapon.type == knife
                         && person[person[i].killtarget].health == 100) {

                        murderer = i;

                        person[i].attacktarget = 0;
                        person[i].attackframe = 0;

                        float gLoc[3];

                        gLoc[0] =
                            (camera.position.x +
                             ((person[i].playercoords.x + flatfacing.x) -
                              camera.position.x) / 2) / soundscalefactor;

                        gLoc[1] =
                            (camera.position.y +
                             ((person[i].playercoords.y + flatfacing.y) -
                              camera.position.y) / 2) / soundscalefactor;

                        gLoc[2] =
                            (camera.position.z +
                             ((person[i].playercoords.z + flatfacing.z) -
                              camera.position.z) / 2) / soundscalefactor;

                        if(person[person[i].killtarget].type != zombietype) {

                          alSourcefv(gSourceID[knifeslashsound], AL_POSITION,
                                     gLoc);

                          alSourcePlay(gSourceID[knifeslashsound]);

                          person[person[i].killtarget].bjoint1 =
                              &person[person[i].killtarget].skeleton.
                              joints[person[person[i].killtarget].skeleton.
                                     jointlabels[neck]];

                          person[person[i].killtarget].bjoint2 =
                              &person[person[i].killtarget].skeleton.
                              joints[person[person[i].killtarget].skeleton.
                                     jointlabels[neck]];

                          person[person[i].killtarget].bleeding = 1;
                          person[person[i].killtarget].bleeddelay = 1;
                          person[person[i].killtarget].health -= 20;

                          person[person[i].killtarget].targetanimation =
                              chestpainanim;

                          person[person[i].killtarget].targetframe = 0;
                          person[person[i].killtarget].target = 0;
                          person[person[i].killtarget].longdead = 1;
                        }
                      }
                    }

                    finaltarget = person[person[i].killtarget].playercoords;
                  }
                }

                if(person[i].type == zombietype
                   && person[person[i].killtarget].health > 0) {

                  if(!person[i].killtargetvisible
                     && person[i].targetanimation == idleanim) {

                    person[i].targetanimation = zombiejoganim;
                  }

                  if(!person[i].killtargetvisible) {
                    person[i].aiming = 0;
                  }

                  if(person[i].killtargetvisible) {
                    person[i].onpath = 0;
                    person[i].lastdistancevictim = 200000;
                    person[i].pathnum = -1;

                    if(person[i].aiming == 0)
                      person[i].weapon.shotdelay = shotdelayamount / difficulty;

                    if(findDistancefast
                       (person[i].playercoords,
                        person[person[i].killtarget].playercoords) > 20
                       || person[i].targetanimation != idleanim)

                      person[i].targetanimation = zombiejoganim;

                    if(findDistancefast
                       (person[i].playercoords,
                        person[person[i].killtarget].playercoords) <= 20) {

                      murderer = i;

                      person[person[i].killtarget].health = 0;
                      person[person[i].killtarget].eaten = i;
                    }

                    finaltarget = person[person[i].killtarget].playercoords;
                  }
                }

                if(person[i].killtargetvisible || realcheck)
                  person[i].pathtarget = finaltarget;

                if(realcheck)
                  person[i].lastdistancevictim =
                      findDistancefast(person[i].pathtarget,
                                       person[person[i].killtarget].
                                       playercoords);

              }
            }

            if(person[i].targetanimation != zombieeatanim
               || person[i].type != zombietype) {

              towards = person[i].playercoords - person[i].pathtarget;

              Normalise(&towards);

              person[i].playerrotation = asin(0 - towards.x) * 360 / 6.28;

              if(towards.z > 0)
                person[i].playerrotation = 180 - person[i].playerrotation;

            }
          }
        }

        person[i].whichblockx =
            (int)((person[i].playercoords.x +
                   block_spacing / 2) / block_spacing);

        person[i].whichblocky =
            (int)((person[i].playercoords.z +
                   block_spacing / 2) / block_spacing);

        if(!person[i].onground)
          person[i].velocity.y += multiplier * gravity;

        if(!person[i].onground && (i != 0 || visions != 1))
          person[i].playercoords += person[i].velocity * multiplier;

        //Death by bleeding/shock
        if(person[i].health <= 0) {
          person[i].skeleton.offset = 0;
          person[i].skeleton.free = 1;
          person[i].longdead = 1;

          for(int j = 0; j < person[i].skeleton.num_joints; j++) {

            person[i].skeleton.joints[j].position +=
                person[i].skeleton.joints[j].offset;

            person[i].skeleton.joints[j].position =
                DoRotation(person[i].skeleton.joints[j].position, 0,
                           person[i].playerrotation, 0);

            person[i].skeleton.joints[j].position += person[i].playercoords;

            person[i].skeleton.joints[j].realoldposition =
                person[i].skeleton.joints[j].position;

            person[i].skeleton.joints[j].velocity =
                DoRotation(person[i].skeleton.joints[j].velocity, 0,
                           person[i].playerrotation, 0);

            person[i].skeleton.joints[j].velocity += person[i].velocity;
            person[i].skeleton.joints[j].velocity += person[i].facing * 4;
          }
        }
      }
      //Rag doll
      if(person[i].skeleton.free == 1 && person[i].longdead > 0) {

        person[i].whichblockx =
            (int)((person[i].skeleton.joints[0].position.x +
                   block_spacing / 2) / block_spacing);

        person[i].whichblocky =
            (int)((person[i].skeleton.joints[0].position.z +
                   block_spacing / 2) / block_spacing);

        move = 0;
        move.x = person[i].whichblockx * block_spacing;
        move.z = person[i].whichblocky * block_spacing;

        person[i].skeleton.DoGravity();

        if(person[i].averageloc.y <= 50)
          person[i].skeleton.
              DoConstraints(&blocksimplecollide[citytype[person[i].whichblockx]
                                                [person[i].whichblocky]], &move,
                            cityrotation[person[i].whichblockx][person[i].
                                                                whichblocky] *
                            90);

        if(person[i].averageloc.y > 50)
          person[i].skeleton.
              DoConstraints(&blockcollide[citytype[person[i].whichblockx]
                                          [person[i].whichblocky]], &move,
                            cityrotation[person[i].whichblockx][person[i].
                                                                whichblocky] *
                            90);

        person[i].oldaverageloc = person[i].averageloc;
        person[i].averageloc = 0;

        for(int j = 0; j < person[i].skeleton.num_joints; j++) {
          person[i].averageloc += person[i].skeleton.joints[j].position;
        }

        person[i].averageloc /= person[i].skeleton.num_joints;
        person[i].playercoords = person[i].averageloc;

        if(person[i].longdead < multiplier / 2 && person[i].longdead > 0)
          person[i].DrawSkeleton(i);

        if(findDistancefast(person[i].averageloc, person[i].oldaverageloc) <
           .2 * multiplier)
          person[i].longdead -= multiplier / 2;
      }

      if(person[i].skeleton.free == 1 && person[i].longdead <= 0
         && person[i].health > 0 && person[i].longdead != -1) {

        person[i].longdead = 1;
        person[i].skeleton.free = 0;
        person[i].currentanimation = lyinganim;
        person[i].target = 0;
        person[i].targetframe = 0;

        //Get up from front or back?
        if(person[i].skeleton.forward.y > 0)
          person[i].targetanimation = getupfrontanim;
        else
          person[i].targetanimation = getupbackanim;

        //Find playercoords
        person[i].playercoords = person[i].averageloc;

        for(int j = 0; j < person[i].skeleton.num_joints; j++) {
          if(person[i].skeleton.joints[j].position.y > person[i].playercoords.y)
            person[i].playercoords.y = person[i].skeleton.joints[j].position.y;
        }

        //Find orientation
        XYZ firsttop =
            person[i].skeleton.joints[person[i].skeleton.jointlabels[neck]].
            position -
            person[i].skeleton.joints[person[i].skeleton.jointlabels[groin]].
            position;

        Normalise(&firsttop);

        person[i].playerrotation = acos(0 - firsttop.z);
        person[i].playerrotation *= 360 / 6.28;

        if(0 > firsttop.x)
          person[i].playerrotation = 360 - person[i].playerrotation;

        person[i].playerrotation *= -1;
        person[i].playervelocity = 0;

        if(person[i].targetanimation == getupfrontanim)
          person[i].playerrotation += 180;

        for(int j = 0; j < person[i].skeleton.num_joints; j++) {

          person[i].tempanimation.position[j][0] =
              person[i].skeleton.joints[j].position - person[i].playercoords;

          person[i].tempanimation.position[j][0] =
              DoRotation(person[i].tempanimation.position[j][0], 0,
                         -person[i].playerrotation, 0);

        }
      }
    }

    //Grenade
    if(ButtonClick(1) && person[0].weapon.type == grenade && person[0].weapon.ammo > 0
       && person[0].weapon.reloading <= 0 && person[0].attackframe < 0
       && person[0].targetanimation != crouchanim) {

      if(person[0].grenphase == 0) {

        XYZ soundsource =
            DoRotation(person[0].skeleton.
                       joints[person[0].skeleton.jointlabels[righthand]].
                       position, 0, person[0].playerrotation,
                       0) + person[0].playercoords;

        float gLoc[3];
        gLoc[0] = soundsource.x / soundscalefactor;
        gLoc[1] = soundsource.y / soundscalefactor;
        gLoc[2] = soundsource.z / soundscalefactor;

        alSourcefv(gSourceID[pinpullsound], AL_POSITION, gLoc);
        alSourcePlay(gSourceID[pinpullsound]);

        person[0].grenphase = 1;
      }
    }

    if((!ButtonClick(1)) && person[0].weapon.type == grenade) {

      if(person[0].grenphase == 1) {
        person[0].grenphase = 0;
        person[0].attackframe = 0;
        person[0].attacktarget = 0;
        person[0].killtarget = 0;
      }
    }

    if(person[0].targetanimation == crouchanim) {

      if(person[0].grenphase == 1) {
        person[0].grenphase = 0;

        XYZ soundsource =
            DoRotation(person[0].skeleton.
                       joints[person[0].skeleton.jointlabels[righthand]].
                       position, 0, person[0].playerrotation,
                       0) + person[0].playercoords;

        float gLoc[3];
        gLoc[0] = soundsource.x / soundscalefactor;
        gLoc[1] = soundsource.y / soundscalefactor;
        gLoc[2] = soundsource.z / soundscalefactor;

        alSourcefv(gSourceID[pinreplacesound], AL_POSITION, gLoc);

        alSourcePlay(gSourceID[pinreplacesound]);
      }
    }
    //Get gun
    int temp;
    int temp2;
    bool switched = 0;

    if(ButtonClick(1) && !oldbutton
       && (person[0].aiming == 0 || person[0].weapon.type == grenade
           || person[0].weapon.type == nogun || person[0].weapon.type == knife)
       && person[0].currentanimation == crouchanim) {

      for(int i = 0; i < max_people; i++) {

        if(!switched && person[i].skeleton.free == 1
           && findDistancefast(person[0].playercoords,
                               person[i].averageloc) < 200) {

          float gLoc[3];
          gLoc[0] = person[0].playercoords.x / soundscalefactor;
          gLoc[1] = person[0].playercoords.y / soundscalefactor;
          gLoc[2] = person[0].playercoords.z / soundscalefactor;

          alSourcefv(gSourceID[clicksound], AL_POSITION, gLoc);
          alSourcePlay(gSourceID[clicksound]);

          temp = person[0].weapon.type;
          temp2 = person[0].weapon.ammo;

          person[0].weapon.type = person[i].weapon.type;
          person[0].weapon.ammo = person[i].weapon.ammo;

          person[i].weapon.type = temp;
          person[i].weapon.ammo = temp2;

          person[0].aiming = 1;
          person[0].aimamount = 0;

          switched = 1;
        }
      }
    }
    //Throw
    Person *bg = &person[0]; //main player 
    if(ButtonClick(1) && bg->attackframe < 0
       && ((bg->weapon.type == nogun || bg->aiming == 0)
           && bg->weapon.type != knife)
       && bg->currentanimation != crouchanim
       && bg->targetanimation != crouchanim
       && bg->targetanimation != throwanim && config.visions == 0) {

      if(bg->targetanimation == idleanim
         || bg->targetanimation == walkanim) {

        bool attacking = 0;
        float closedistance = -1;
        bg->killtarget = -1;
        Person *person1;

        for(int i = 1; i < numpeople; i++) {
          person1 = &person[i];
          if(person1->skeleton.free < 1 && (person1->weapon.type != nogun)
             && findDistancefast(person1->playercoords,
                                 bg->playercoords + flatfacing) < 12) {

            attacking = 1;

            if(bg->killtarget == -1
               || findDistancefast(person1->playercoords,
                                   bg->playercoords) < closedistance) {

              bg->killtarget = i;

              closedistance =
                  findDistancefast(person1->playercoords,
                                   bg->playercoords);

            }
          }
        }
        //bodyguard target
        Person *bg_target = &person[bg->killtarget];

        if(attacking) {

          score += 150;

          bg->aiming = 1;
          bg->weapon.type = bg_target->weapon.type;
          bg->weapon.ammo = bg_target->weapon.ammo;

          bg_target->weapon.type = nogun;
          bg_target->killtarget = -1;

          bg->targetframe = 0;
          bg->targetanimation = throwanim;
          bg->target = 1;
          bg->speed = 1;

          bg_target->targetframe = 0;
          bg_target->targetanimation = thrownanim;
          bg_target->target = 1;
          bg_target->playercoords = bg->playercoords;
          bg_target->playerrotation =
              bg->playerrotation;

          bg_target->speed = bg->speed;
          bg_target->speedmult = 1;
        }
      }
    }

    //Gun whacking
    bool whacked = 0;
    if(ButtonClick(1)
       && (person[0].aiming == 0 || person[0].weapon.ammo <= 0
           || person[0].weapon.type == nogun || person[0].weapon.type == knife
           || person[0].targetanimation == joganim)
       && person[0].currentanimation != crouchanim
       && person[0].targetanimation != throwanim
       && person[0].weapon.type != grenade
       && person[0].targetanimation != crouchanim && visions == 0) {

      if(person[0].attackframe == -1
         || person[person[0].killtarget].skeleton.free == 1) {

        bool attacking = 0;
        float closedistance = -1;
        person[0].killtarget = -1;

        for(int i = 1; i < numpeople; i++) {

          if(person[i].existing && person[i].type != viptype
             && person[i].skeleton.free < 1
             && findDistancefast(person[i].playercoords,
                                 person[0].playercoords + flatfacing) <
             12 + (person[0].weapon.type == knife) * 10) {

            if(person[0].killtarget == -1
               || findDistancefast(person[i].playercoords,
                                   person[0].playercoords) < closedistance) {

              attacking = 1;

              person[0].killtarget = i;

              closedistance =
                  findDistancefast(person[i].playercoords,
                                   person[0].playercoords);
            }
          }
        }

        if(attacking) {
          person[0].attacktarget = 0;
          person[0].attackframe = 0;
        }

        whacked = 0;
      }
    }

    XYZ velocity;

    if(person[0].attackframe > 1
       || (person[0].attackframe >= 0
           && person[0].currentanimation == joganim)) {

      if(person[person[0].killtarget].skeleton.free < 1
         && person[0].killtarget != 0 && (person[0].aiming < 1
                                          || person[0].weapon.type == nogun
                                          || person[0].weapon.type == knife
                                          || person[0].targetanimation ==
                                          joganim)) {

        float gLoc[3];

        gLoc[0] =
            (camera.position.x +
             ((person[0].playercoords.x + flatfacing.x) -
              camera.position.x) / 2) / soundscalefactor;

        gLoc[1] =
            (camera.position.y +
             ((person[0].playercoords.y + flatfacing.y) -
              camera.position.y) / 2) / soundscalefactor;

        gLoc[2] =
            (camera.position.z +
             ((person[0].playercoords.z + flatfacing.z) -
              camera.position.z) / 2) / soundscalefactor;

        if(person[person[0].killtarget].type != zombietype) {

          if(person[0].weapon.type != knife) {

            alSourcefv(gSourceID[headwhacksound], AL_POSITION, gLoc);

            alSourcePlay(gSourceID[headwhacksound]);

          }

          if(person[0].weapon.type == knife) {

            alSourcefv(gSourceID[knifeslashsound], AL_POSITION, gLoc);

            alSourcePlay(gSourceID[knifeslashsound]);

            person[person[0].killtarget].bjoint1 =
                &person[person[0].killtarget].skeleton.
                joints[person[person[0].killtarget].skeleton.jointlabels[neck]];

            person[person[0].killtarget].bjoint2 =
                &person[person[0].killtarget].skeleton.
                joints[person[person[0].killtarget].skeleton.jointlabels[neck]];

            person[person[0].killtarget].bleeding = 1;
            person[person[0].killtarget].bleeddelay = 1;

            person[0].bjoint1 =
                &person[0].skeleton.joints[person[0].skeleton.
                                           jointlabels[righthand]];

            person[0].bjoint2 =
                &person[0].skeleton.joints[person[0].skeleton.
                                           jointlabels[righthand]];

            person[0].bleeding = 1;
            person[0].bleeddelay = 1;

            velocity =
                DoRotation(flatfacing, 0, 70, 0) * 50 + person[0].velocity * 2;

            velocity.y += 30;

            sprites.MakeSprite(bloodspritedown, .8, 1, .2, .2,
                               DoRotation(person[person[0].killtarget].skeleton.
                                          joints[person[person[0].killtarget].
                                                 skeleton.jointlabels[neck]].
                                          position, 0,
                                          person[person[0].killtarget].
                                          playerrotation,
                                          0) +
                               person[person[0].killtarget].playercoords,
                               velocity * .3, 2);

            sprites.MakeSprite(bloodspritedown, .8, 1, .2, .2,
                               DoRotation(person[person[0].killtarget].skeleton.
                                          joints[person[person[0].killtarget].
                                                 skeleton.jointlabels[neck]].
                                          position, 0,
                                          person[person[0].killtarget].
                                          playerrotation,
                                          0) +
                               person[person[0].killtarget].playercoords,
                               velocity * .2, 3);

            sprites.MakeSprite(bloodspritedown, .8, 1, .2, .2,
                               DoRotation(person[person[0].killtarget].skeleton.
                                          joints[person[person[0].killtarget].
                                                 skeleton.jointlabels[neck]].
                                          position, 0,
                                          person[person[0].killtarget].
                                          playerrotation,
                                          0) +
                               person[person[0].killtarget].playercoords,
                               velocity * .1, 4);

          }

          person[person[0].killtarget].health -= 100;
          person[person[0].killtarget].skeleton.free = 1;
          person[person[0].killtarget].longdead = 1;

          for(int j = 0; j < person[person[0].killtarget].skeleton.num_joints;
              j++) {

            person[person[0].killtarget].skeleton.joints[j].position =
                DoRotation(person[person[0].killtarget].skeleton.joints[j].
                           position, 0,
                           person[person[0].killtarget].playerrotation, 0);

            person[person[0].killtarget].skeleton.joints[j].position +=
                person[person[0].killtarget].playercoords;

            person[person[0].killtarget].skeleton.joints[j].realoldposition =
                person[person[0].killtarget].skeleton.joints[j].position;

            person[person[0].killtarget].skeleton.joints[j].velocity =
                person[person[0].killtarget].velocity;

            person[person[0].killtarget].skeleton.joints[j].velocity.x +=
                abs(Random() % 10) - 5;

            person[person[0].killtarget].skeleton.joints[j].velocity.y +=
                abs(Random() % 10) - 5;

            person[person[0].killtarget].skeleton.joints[j].velocity.z +=
                abs(Random() % 10) - 5;

          }

          if(person[0].weapon.type != knife) {

            person[person[0].killtarget].skeleton.
                joints[person[person[0].killtarget].skeleton.jointlabels[head]].
                velocity += DoRotation(flatfacing, 0, 40, 0) * 50;

            person[person[0].killtarget].skeleton.
                joints[person[person[0].killtarget].skeleton.jointlabels[head]].
                velocity += person[0].velocity * 2;
          }
        } else {
          if(whacked == 0) {
            whacked = 1;

            if(person[0].weapon.type != knife) {
              alSourcefv(gSourceID[headwhacksound], AL_POSITION, gLoc);
              alSourcePlay(gSourceID[headwhacksound]);
            }

            if(person[0].weapon.type == knife) {

              alSourcefv(gSourceID[knifeslashsound], AL_POSITION, gLoc);
              alSourcePlay(gSourceID[knifeslashsound]);

              person[person[0].killtarget].bjoint1 =
                  &person[person[0].killtarget].skeleton.
                  joints[person[person[0].killtarget].skeleton.
                         jointlabels[neck]];

              person[person[0].killtarget].bjoint2 =
                  &person[person[0].killtarget].skeleton.
                  joints[person[person[0].killtarget].skeleton.
                         jointlabels[neck]];

              person[person[0].killtarget].bleeding = 1;
              person[person[0].killtarget].bleeddelay = 1;

              person[0].bjoint1 =
                  &person[0].skeleton.joints[person[0].skeleton.
                                             jointlabels[righthand]];

              person[0].bjoint2 =
                  &person[0].skeleton.joints[person[0].skeleton.
                                             jointlabels[righthand]];

              person[0].bleeding = 1;
              person[0].bleeddelay = 1;

              velocity =
                  DoRotation(flatfacing, 0, 70,
                             0) * 50 + person[0].velocity * 2;

              velocity.y += 30;

              sprites.MakeSprite(bloodspritedown, .8, 1, .2, .2,
                                 DoRotation(person[person[0].killtarget].
                                            skeleton.
                                            joints[person[person[0].killtarget].
                                                   skeleton.jointlabels[neck]].
                                            position, 0,
                                            person[person[0].killtarget].
                                            playerrotation,
                                            0) +
                                 person[person[0].killtarget].playercoords,
                                 velocity * .3, 2);

              sprites.MakeSprite(bloodspritedown, .8, 1, .2, .2,
                                 DoRotation(person[person[0].killtarget].
                                            skeleton.
                                            joints[person[person[0].killtarget].
                                                   skeleton.jointlabels[neck]].
                                            position, 0,
                                            person[person[0].killtarget].
                                            playerrotation,
                                            0) +
                                 person[person[0].killtarget].playercoords,
                                 velocity * .2, 3);

              sprites.MakeSprite(bloodspritedown, .8, 1, .2, .2,
                                 DoRotation(person[person[0].killtarget].
                                            skeleton.
                                            joints[person[person[0].killtarget].
                                                   skeleton.jointlabels[neck]].
                                            position, 0,
                                            person[person[0].killtarget].
                                            playerrotation,
                                            0) +
                                 person[person[0].killtarget].playercoords,
                                 velocity * .1, 4);

            }

            person[person[0].killtarget].health -= 200;
            person[person[0].killtarget].maxhealth -= 20;
            person[person[0].killtarget].skeleton.free = 1;
            person[person[0].killtarget].longdead = 1;

            for(int j = 0; j < person[person[0].killtarget].skeleton.num_joints;
                j++) {

              person[person[0].killtarget].skeleton.joints[j].position =
                  DoRotation(person[person[0].killtarget].skeleton.joints[j].
                             position, 0,
                             person[person[0].killtarget].playerrotation, 0);

              person[person[0].killtarget].skeleton.joints[j].position +=
                  person[person[0].killtarget].playercoords;

              person[person[0].killtarget].skeleton.joints[j].realoldposition =
                  person[person[0].killtarget].skeleton.joints[j].position;

              person[person[0].killtarget].skeleton.joints[j].velocity =
                  person[person[0].killtarget].velocity;

              person[person[0].killtarget].skeleton.joints[j].velocity.x +=
                  abs(Random() % 10) - 5;

              person[person[0].killtarget].skeleton.joints[j].velocity.y +=
                  abs(Random() % 10) - 5;

              person[person[0].killtarget].skeleton.joints[j].velocity.z +=
                  abs(Random() % 10) - 5;
            }

            if(person[0].weapon.type != knife) {

              person[person[0].killtarget].skeleton.
                  joints[person[person[0].killtarget].skeleton.
                         jointlabels[head]].velocity +=
                  DoRotation(flatfacing, 0, 40, 0) * 50;

              person[person[0].killtarget].skeleton.
                  joints[person[person[0].killtarget].skeleton.
                         jointlabels[head]].velocity += person[0].velocity * 2;
            }
          }
        }
      }
    }
    //Tackle
    if(person[0].currentanimation == diveanim && visions == 0) {

      for(int i = 1; i < numpeople; i++) {

        if(person[i].skeleton.free < 1
           && findDistancefast(person[i].playercoords,
                               person[0].playercoords + flatfacing) < 22) {

          float gLoc[3];

          gLoc[0] =
              (camera.position.x +
               ((person[0].playercoords.x + flatfacing.x) -
                camera.position.x) / 2) / soundscalefactor;

          gLoc[1] =
              (camera.position.y +
               ((person[0].playercoords.y + flatfacing.y) -
                camera.position.y) / 2) / soundscalefactor;

          gLoc[2] =
              (camera.position.z +
               ((person[0].playercoords.z + flatfacing.z) -
                camera.position.z) / 2) / soundscalefactor;

          alSourcefv(gSourceID[headwhacksound], AL_POSITION, gLoc);
          alSourcePlay(gSourceID[headwhacksound]);

          person[i].skeleton.free = 1;
          person[i].longdead = 1;

          for(int j = 0; j < person[i].skeleton.num_joints; j++) {

            person[i].skeleton.joints[j].position =
                DoRotation(person[i].skeleton.joints[j].position, 0,
                           person[i].playerrotation, 0);

            person[i].skeleton.joints[j].position += person[i].playercoords;

            person[i].skeleton.joints[j].realoldposition =
                person[i].skeleton.joints[j].position;

            person[i].skeleton.joints[j].velocity = person[0].velocity;
            person[i].skeleton.joints[j].velocity.y = -10;
            person[i].skeleton.joints[j].velocity.x += abs(Random() % 10) - 5;
            person[i].skeleton.joints[j].velocity.y += abs(Random() % 10) - 5;
            person[i].skeleton.joints[j].velocity.z += abs(Random() % 10) - 5;
          }
        }
      }
    }

    Fire(); //fire weapon and hit something, hopefully...

    XYZ start;
    XYZ wallhit;

    if(!ButtonClick(1))
      oldbutton = 0;

    if(lasersight && person[0].weapon.type != grenade) {

      for(int j = 0; j < numpeople; j++) {
        if(person[j].type == playertype) {
          if(person[j].aiming >= 1) {

            //Firing
            XYZ end, aim;
            XYZ finalwallhit;
            HitStruct hitstruct, temphitstruct;
            float olddistance = 0.0;
            float distance;
            int whichhit = -1;

            if(!zoom || j != 0) {

              if(person[j].weapon.type == assaultrifle)
                aim =
                    DoRotation(person[j].skeleton.
                               joints[(person[j].skeleton.
                                       jointlabels[lefthand])].position -
                               person[j].skeleton.
                               joints[(person[j].skeleton.
                                       jointlabels[righthand])].position, 0,
                               person[j].playerrotation - 2.5, 0);

              if(person[j].weapon.type == sniperrifle)
                aim =
                    DoRotation(person[j].skeleton.
                               joints[(person[j].skeleton.
                                       jointlabels[lefthand])].position -
                               person[j].skeleton.
                               joints[(person[j].skeleton.
                                       jointlabels[righthand])].position, 0,
                               person[j].playerrotation + 4, 0);

              if(person[j].weapon.type == shotgun)
                aim =
                    DoRotation(person[j].skeleton.
                               joints[(person[j].skeleton.
                                       jointlabels[lefthand])].position -
                               person[j].skeleton.
                               joints[(person[j].skeleton.
                                       jointlabels[righthand])].position, 0,
                               person[j].playerrotation + 4, 0);

              if(person[j].weapon.type == handgun1 && !thirdperson && j == 0)
                aim =
                    DoRotation(person[j].skeleton.
                               joints[(person[j].skeleton.
                                       jointlabels[righthand])].position -
                               (person[j].skeleton.
                                joints[person[j].skeleton.jointlabels[head]].
                                position * .65 +
                                person[j].skeleton.joints[person[j].skeleton.
                                                          jointlabels[neck]].
                                position * .35), 0,
                               person[j].playerrotation - .9, 0);

              if(person[j].weapon.type == handgun1 && (thirdperson || j != 0))
                aim =
                    DoRotation(person[j].skeleton.
                               joints[(person[j].skeleton.
                                       jointlabels[righthand])].position -
                               (person[j].skeleton.
                                joints[person[j].skeleton.jointlabels[head]].
                                position * .35 +
                                person[j].skeleton.joints[person[j].skeleton.
                                                          jointlabels[neck]].
                                position * .65), 0,
                               person[j].playerrotation - .9, 0);

              if(person[j].weapon.type == handgun2 && !thirdperson && j == 0)
                aim =
                    DoRotation(person[j].skeleton.
                               joints[(person[j].skeleton.
                                       jointlabels[righthand])].position -
                               (person[j].skeleton.
                                joints[person[j].skeleton.jointlabels[head]].
                                position * .65 +
                                person[j].skeleton.joints[person[j].skeleton.
                                                          jointlabels[neck]].
                                position * .35), 0,
                               person[j].playerrotation - .9, 0);

              if(person[j].weapon.type == handgun2 && (thirdperson || j != 0))
                aim =
                    DoRotation(person[j].skeleton.
                               joints[(person[j].skeleton.
                                       jointlabels[righthand])].position -
                               (person[j].skeleton.
                                joints[person[j].skeleton.jointlabels[head]].
                                position * .35 +
                                person[j].skeleton.joints[person[j].skeleton.
                                                          jointlabels[neck]].
                                position * .65), 0,
                               person[j].playerrotation - .9, 0);

            }

            if(zoom && j == 0) {
              start = camera.position;
              aim = facing;
            }

            Normalise(&aim);

            if(person[j].weapon.type == sniperrifle) {

              start =
                  person[j].playercoords +
                  DoRotation(person[j].skeleton.
                             joints[(person[j].skeleton.jointlabels[lefthand])].
                             position, 0, person[j].playerrotation, 0);

              start -=
                  DoRotation(DoRotation
                             (DoRotation(aim, 0, -person[j].playerrotation, 0),
                              90, 0, 0), 0, person[j].playerrotation,
                             0) * (0 - .4);

            }

            if(person[j].weapon.type == shotgun) {

              start =
                  person[j].playercoords +
                  DoRotation(person[j].skeleton.
                             joints[(person[j].skeleton.jointlabels[lefthand])].
                             position, 0, person[j].playerrotation, 0);

              start -=
                  DoRotation(DoRotation
                             (DoRotation(aim, 0, -person[j].playerrotation, 0),
                              90, 0, 0), 0, person[j].playerrotation,
                             0) * (0 - .4);

            }

            if(person[j].weapon.type == handgun1) {

              start =
                  person[j].playercoords +
                  DoRotation(person[j].skeleton.
                             joints[(person[j].skeleton.
                                     jointlabels[rightwrist])].position, 0,
                             person[j].playerrotation, 0);

              start -=
                  DoRotation(DoRotation
                             (DoRotation(aim, 0, -person[j].playerrotation, 0),
                              90, 0, 0), 0, person[j].playerrotation,
                             0) * (.55 - .4);

            }

            if(person[j].weapon.type == handgun2) {

              start =
                  person[j].playercoords +
                  DoRotation(person[j].skeleton.
                             joints[(person[j].skeleton.
                                     jointlabels[rightwrist])].position, 0,
                             person[j].playerrotation, 0);

              start -=
                  DoRotation(DoRotation
                             (DoRotation(aim, 0, -person[j].playerrotation, 0),
                              90, 0, 0), 0, person[j].playerrotation,
                             0) * (.55 - .4);

            }

            if(person[j].weapon.type == assaultrifle) {

              start =
                  person[j].playercoords +
                  DoRotation(person[j].skeleton.
                             joints[(person[j].skeleton.jointlabels[lefthand])].
                             position, 0, person[j].playerrotation, 0);

              start -=
                  DoRotation(DoRotation
                             (DoRotation(aim, 0, -person[j].playerrotation, 0),
                              90, 0, 0), 0, person[j].playerrotation,
                             0) * (.25 - .4);

            }

            end = start + aim * 1000;

            //Blocks
            wallhit = 0;

            beginx =
                (int)(person[j].playercoords.x +
                      block_spacing / 2) / block_spacing - 2;

            if(beginx < 0)
              beginx = 0;

            beginz =
                (int)(person[j].playercoords.z +
                      block_spacing / 2) / block_spacing - 2;

            if(beginz < 0)
              beginz = 0;

            endx =
                (int)(person[j].playercoords.x +
                      block_spacing / 2) / block_spacing + 2;

            if(endx > num_blocks - 1)
              endx = num_blocks - 1;

            endz =
                (int)(person[j].playercoords.z +
                      block_spacing / 2) / block_spacing + 2;

            if(endz > num_blocks - 1)
              endz = num_blocks - 1;

            if(beginx < endx && beginz < endz)

              finalwallhit = 0;

            for(int i = beginx; i <= endx; i++)
              for(int j = beginz; j <= endz; j++) {
                move = 0;
                move.x = i * block_spacing;
                move.z = j * block_spacing;

                whichtri =
                    blocks[citytype[i][j]].LineCheck2(start, end, &wallhit,
                                                      move,
                                                      cityrotation[i][j] * 90);

                if(whichtri != -1) {
                  end = wallhit;
                  finalwallhit = wallhit;
                }
              }

            wallhit = 0;
            wallhit.x = camera.position.x;
            wallhit.z = camera.position.z;

            whichtri = Bigstreet.LineCheck2(start, end, &wallhit, wallhit, 0);

            if(whichtri != -1) {
              end = wallhit;
              finalwallhit = wallhit;
            }
            //People
            whichhit = -1;

            for(int i = 0; i < numpeople; i++) {

              if(i != j
                 && findDistancefast(person[j].playercoords,
                                     person[i].playercoords) < 20000) {

                temphitstruct =
                    person[i].BulletCollideWithPlayer(i, start, end);

                if(temphitstruct.collision) {
                  distance = findDistancefast(start, temphitstruct.hitlocation);

                  if(distance < olddistance || whichhit == -1) {
                    end = temphitstruct.hitlocation;
                    olddistance = distance;
                    hitstruct = temphitstruct;
                    whichhit = i;
                  }
                }
              }
            }

            aimer[0] = start;
            aimer[1] = end;

            sprites.MakeSprite(bulletinstant, .4, 1, 0, 0, aimer[0], aimer[1],
                               .2);

          }
        }
      }
    }
    //Snow
    float snowdelay = environment.snowdelay;
    snowdelay -= multiplier;

    float precipitationhorz = environment.precipitationhorz;
    float precipitationvert = environment.precipitationvert;
    float precipitationdensity = environment.precipitationdensity;

    while(snowdelay < 0 && environment.type == snowy_environment) {
      snowdelay += 1 / precipitationdensity * 2;

      velocity = 0;
      velocity.y = -5;

      start = camera.position;
      start.y += precipitationvert;
      start.x += Random() % (int)precipitationhorz;
      start.z += Random() % (int)precipitationhorz;

      sprites.MakeSprite(snowsprite, 1, 1, 1, 1, start, velocity, 1.01);

    }

    while(snowdelay < 0 && environment.type == rainy_environment) {
      snowdelay += 1 / precipitationdensity / 4;

      velocity = 0;
      velocity.y = -100;

      start = camera.position;
      start.y += precipitationvert;
      start.x += Random() % (int)precipitationhorz *.5;
      start.z += Random() % (int)precipitationhorz *.5;

      sprites.MakeSprite(rainsprite, .5, 1, 1, 1, start, velocity, 2.00);

    }

    //Grenade collision
    int wherex, wherey, whichsound;
    bool impact;

    for(int i = 0; i < sprites.howmanysprites; i++) {

      if(sprites.type[i] == grenadesprite || sprites.type[i] == spoonsprite
         || sprites.type[i] == pinsprite) {

        impact = 0;

        if(sprites.type[i] != grenadesprite) {
          sprites.brightness[i] -= multiplier * .2;
        }

        if(findLengthfast(sprites.velocity[i]) > 0) {

          wherex =
              (int)(sprites.location[i].x + block_spacing / 2) / block_spacing;

          wherey =
              (int)(sprites.location[i].z + block_spacing / 2) / block_spacing;

          move = 0;
          move.x = wherex * block_spacing;
          move.z = wherey * block_spacing;

          whichtri =
              blocks[citytype[wherex][wherey]].LineCheck2(sprites.
                                                          oldlocation[i],
                                                          sprites.location[i],
                                                          &wallhit, move,
                                                          cityrotation[wherex]
                                                          [wherey] * 90);

          if(whichtri != -1) {

            impact = 1;

            normalrotated =
                DoRotation(blocks[citytype[wherex][wherey]].normals[whichtri],
                           0, cityrotation[wherex][wherey] * 90, 0);

            if(sprites.size[i] > 1)
              decals.MakeDecal(crater, wallhit, 9, normalrotated, whichtri,
                               &blocks[citytype[wherex][wherey]], move,
                               cityrotation[wherex][wherey] * 90);

            sprites.location[i] = wallhit + normalrotated * .02;

            ReflectVector(&sprites.velocity[i], &normalrotated);

            sprites.velocity[i] *= .3;

            if(sprites.type[i] == grenadesprite) {

              float gLoc[3];

              gLoc[0] =
                  (((sprites.location[i].x) -
                    camera.position.x) / findLengthfast(sprites.velocity[i]) *
                   5 + camera.position.x) / soundscalefactor;

              gLoc[1] =
                  (((sprites.location[i].y) -
                    camera.position.y) / findLengthfast(sprites.velocity[i]) *
                   5 + camera.position.y) / soundscalefactor;

              gLoc[2] =
                  (((sprites.location[i].z) -
                    camera.position.z) / findLengthfast(sprites.velocity[i]) *
                   5 + camera.position.z) / soundscalefactor;

              whichsound = abs(Random() % 2);

              alSourcefv(gSourceID[bouncesound + whichsound], AL_POSITION,
                         gLoc);

              if(sprites.size[i] <= 1)
                alSourcePlay(gSourceID[bouncesound + whichsound]);

            }

            if(findLengthfast(sprites.velocity[i]) <= 10)
              sprites.velocity[i] = 0;

          }

          if(sprites.location[i].y < 0) {
            impact = 1;

            sprites.velocity[i].y *= -1;
            sprites.velocity[i] *= .3;
            sprites.location[i].y = 0;

            if(sprites.type[i] == grenadesprite) {

              if(sprites.size[i] > 1) {
                move = 0;

                sprites.location[i].y = -.5;

                XYZ normish;
                normish = 0;
                normish.y = 1;

                decals.MakeDecal(crater, sprites.location[i], 9, normish, 0,
                                 &blocks[citytype[wherex][wherey]], move, 0);

              }

              float gLoc[3];

              gLoc[0] =
                  (((sprites.location[i].x) -
                    camera.position.x) / findLengthfast(sprites.velocity[i]) *
                   5 + camera.position.x) / soundscalefactor;

              gLoc[1] =
                  (((sprites.location[i].y) -
                    camera.position.y) / findLengthfast(sprites.velocity[i]) *
                   5 + camera.position.y) / soundscalefactor;

              gLoc[2] =
                  (((sprites.location[i].z) -
                    camera.position.z) / findLengthfast(sprites.velocity[i]) *
                   5 + camera.position.z) / soundscalefactor;

              whichsound = abs(Random() % 2);

              alSourcefv(gSourceID[bouncesound + whichsound], AL_POSITION,
                         gLoc);

              if(sprites.size[i] <= 1)
                alSourcePlay(gSourceID[bouncesound + whichsound]);

            }

            if(findLengthfast(sprites.velocity[i]) <= 10)
              sprites.velocity[i] = 0;

          }

          if(sprites.type[i] == grenadesprite
             && findLengthfast(sprites.velocity[i]) > 20) {

            HitStruct hitstruct;

            for(int j = 0; j < numpeople; j++) {

              if((j != 0 || sprites.brightness[i] < .9) && person[j].existing) {

                hitstruct =
                    person[j].BulletCollideWithPlayer(j, sprites.oldlocation[i],
                                                      sprites.location[i]);

                if(hitstruct.collision) {

                  impact = 1;

                  sprites.location[i] = hitstruct.hitlocation;

                  float gLoc[3];

                  gLoc[0] =
                      (((sprites.location[i].x) -
                        camera.position.x) /
                       findLengthfast(sprites.velocity[i]) * 5 +
                       camera.position.x) / soundscalefactor;

                  gLoc[1] =
                      (((sprites.location[i].y) -
                        camera.position.y) /
                       findLengthfast(sprites.velocity[i]) * 5 +
                       camera.position.y) / soundscalefactor;

                  gLoc[2] =
                      (((sprites.location[i].z) -
                        camera.position.z) /
                       findLengthfast(sprites.velocity[i]) * 5 +
                       camera.position.z) / soundscalefactor;

                  if(person[j].skeleton.free < 1) {

                    if((hitstruct.joint1->label == head
                        || hitstruct.joint2->label == head)
                       && person[j].type != zombietype) {

                      alSourcefv(gSourceID[headwhacksound], AL_POSITION, gLoc);

                      if(sprites.size[i] <= 1)
                        alSourcePlay(gSourceID[headwhacksound]);

                      person[j].skeleton.free = 1;

                      person[j].longdead = 1;

                      for(int k = 0; k < person[j].skeleton.num_joints; k++) {
                        person[j].skeleton.joints[k].position =
                            DoRotation(person[j].skeleton.joints[k].position, 0,
                                       person[j].playerrotation, 0);
                        person[j].skeleton.joints[k].position +=
                            person[j].playercoords;
                        person[j].skeleton.joints[k].realoldposition =
                            person[j].skeleton.joints[k].position;

                        person[j].skeleton.joints[k].velocity =
                            person[j].velocity;
                        person[j].skeleton.joints[k].velocity.x +=
                            abs(Random() % 10) - 5;
                        person[j].skeleton.joints[k].velocity.y +=
                            abs(Random() % 10) - 5;
                        person[j].skeleton.joints[k].velocity.z +=
                            abs(Random() % 10) - 5;

                      }

                      hitstruct.joint1->velocity += sprites.velocity[i];
                      hitstruct.joint2->velocity += sprites.velocity[i];

                      if(person[j].type == civiliantype)
                        civkills++;

                      if(person[j].type == eviltype)
                        goodkills++;

                    } else {

                      float totalarea = 0.0f;

                      alSourcefv(gSourceID[bodywhacksound], AL_POSITION, gLoc);

                      if(sprites.size[i] <= 1)
                        alSourcePlay(gSourceID[bodywhacksound]);

                      person[j].skeleton.offset = 1;

                      for(int k = 0; k < person[j].skeleton.num_joints; k++) {

                        if(findDistancefast
                           (DoRotation
                            (person[j].skeleton.joints[k].position, 0,
                             person[j].playerrotation,
                             0) + person[j].playercoords,
                            hitstruct.hitlocation) < 200) {

                          totalarea +=
                              (200 /
                               findDistancefast(DoRotation
                                                (person[j].skeleton.joints[k].
                                                 position, 0,
                                                 person[j].playerrotation,
                                                 0) + person[j].playercoords,
                                                hitstruct.hitlocation));
                        }
                      }

                      float offsetlength;

                      for(int k = 0; k < person[j].skeleton.num_joints; k++) {

                        if(findDistancefast
                           (DoRotation
                            (person[j].skeleton.joints[k].position, 0,
                             person[j].playerrotation,
                             0) + person[j].playercoords,
                            hitstruct.hitlocation) < 200) {

                          person[j].skeleton.joints[k].offset +=
                              DoRotation(sprites.velocity[i] * .1 *
                                         ((200 /
                                           findDistancefast(DoRotation
                                                            (person[j].skeleton.
                                                             joints[k].position,
                                                             0,
                                                             person[j].
                                                             playerrotation,
                                                             0) +
                                                            person[j].
                                                            playercoords,
                                                            hitstruct.
                                                            hitlocation)) /
                                          totalarea * 10), 0,
                                         -person[j].playerrotation, 0);

                        }

                        offsetlength =
                            findLengthfast(person[j].skeleton.joints[k].offset);

                        if(offsetlength > 9) {

                          Normalise(&person[j].skeleton.joints[k].offset);

                          person[j].skeleton.joints[k].offset *= 3;

                        }
                      }
                    }
                  }

                  sprites.velocity[i] *= -.3;

                }
              }
            }
          }

          sprites.oldlocation[i] = sprites.location[i];

        }
        //Explode
        if(sprites.type[i] == grenadesprite) {

          sprites.brightness[i] -= multiplier * .3;

          if(sprites.brightness[i] <= 0 || (impact && sprites.size[i] > 1)) {

            sprites.brightness[i] = 0;

            sprites.MakeSprite(smokesprite, 1, 1, 1, 1, sprites.location[i],
                               facing * 0, 60);

            sprites.MakeSprite(muzzleflashsprite, 1, 1, 1, 1,
                               sprites.location[i], facing * 0, 9);

            float gLoc[3];

            gLoc[0] =
                (((sprites.location[i].x) - camera.position.x) / 3 +
                 camera.position.x) / soundscalefactor;

            gLoc[1] =
                (((sprites.location[i].y) - camera.position.y) / 3 +
                 camera.position.y) / soundscalefactor;

            gLoc[2] =
                (((sprites.location[i].z) - camera.position.z) / 3 +
                 camera.position.z) / soundscalefactor;

            alSourcefv(gSourceID[explosionsound], AL_POSITION, gLoc);

            alSourcePlay(gSourceID[explosionsound]);

            XYZ relation;

            camera.camerashake =
                1 - findDistance(person[0].playercoords,
                                 sprites.location[i]) / 200;

            //if(!sprites.size[i]>1){

            overpoint = sprites.location[i];
            overpoint.y += 3000;

            underpoint = sprites.location[i];
            underpoint.y -= 3000;

            move = 0;

            wherex =
                (int)(sprites.location[i].x +
                      block_spacing / 2) / block_spacing;

            wherey =
                (int)(sprites.location[i].z +
                      block_spacing / 2) / block_spacing;

            move.x = wherex * block_spacing;
            move.z = wherey * block_spacing;

            XYZ temp;

            whichtri =
                sidewalkcollide.LineCheck2(overpoint, underpoint, &temp, move,
                                           cityrotation[wherex][wherey] * 90);

            XYZ normish;
            normish = 0;
            normish.y = 1;

            if(whichtri >= 0) {

              decals.MakeDecal(crater, sprites.location[i], 9, normish, 0,
                               &sidewalkcollide, move,
                               cityrotation[wherex][wherey] * 90);

            }

            if(whichtri == -1) {
              temp = sprites.location[i];
              temp.y = -.5;

              move = 0;

              decals.MakeDecal(crater, sprites.location[i], 9, normish, 0,
                               &sidewalkcollide, move, 0);

            }
            //}

            for(int k = 0; k < numpeople; k++) {

              if(person[k].existing
                 && (person[k].longdead != -1 || person[k].skeleton.free < 1)) {

                if((findDistancefast
                    (person[k].playercoords, sprites.location[i]) < 700
                    && person[k].skeleton.free < 1)
                   ||
                   (findDistancefast(person[k].averageloc, sprites.location[i])
                    < 700 && person[k].skeleton.free >= 1)) {

                  if(person[k].skeleton.free != 1) {

                    if(person[k].type == civiliantype)
                      civkills++;

                    if(person[k].type == eviltype)
                      goodkills++;

                    person[k].skeleton.free = 1;

                    person[k].killtargetvisible = 0;

                    if((findDistancefast
                        (person[k].playercoords, sprites.location[i]) < 600
                        && person[k].skeleton.free < 1)
                       ||
                       (findDistancefast
                        (person[k].averageloc, sprites.location[i]) < 600
                        && person[k].skeleton.free >= 1)
                       || person[k].type == playertype) {

                      person[k].health -= 100;
                      person[k].bleeding = 1;
                    }

                    person[k].DoAnimations(k);
                    person[k].longdead = 1;
                    person[k].bleeddelay = 1;

                    person[k].bjoint1 =
                        &person[k].skeleton.joints[person[k].skeleton.
                                                   jointlabels[head]];

                    person[k].bjoint2 =
                        &person[k].skeleton.joints[person[k].skeleton.
                                                   jointlabels[neck]];

                    for(int j = 0; j < person[k].skeleton.num_joints; j++) {

                      person[k].skeleton.joints[j].position =
                          DoRotation(person[k].skeleton.joints[j].position, 0,
                                     person[k].playerrotation, 0);

                      person[k].skeleton.joints[j].position +=
                          person[k].playercoords;

                      person[k].skeleton.joints[j].realoldposition =
                          person[k].skeleton.joints[j].position;

                      person[k].skeleton.joints[j].velocity =
                          DoRotation(person[k].skeleton.joints[j].velocity, 0,
                                     person[k].playerrotation, 0);

                      person[k].skeleton.joints[j].velocity +=
                          person[k].velocity;

                      person[k].skeleton.joints[j].velocity.x +=
                          abs(Random() % 20) - 10;

                      person[k].skeleton.joints[j].velocity.y +=
                          abs(Random() % 20) - 10;

                      person[k].skeleton.joints[j].velocity.z +=
                          abs(Random() % 20) - 10;

                  }}

                  for(int j = 0; j < person[k].skeleton.num_joints; j++) {

                    relation =
                        person[k].skeleton.joints[j].position -
                        sprites.location[i];

                    Normalise(&relation);

                    if(findDistance
                       (person[k].skeleton.joints[j].position,
                        sprites.location[i]) > 1)
                      person[k].skeleton.joints[j].velocity +=
                          relation /
                          findDistance(person[k].skeleton.joints[j].position,
                                       sprites.location[i]) * 300;

                    else
                      person[k].skeleton.joints[j].velocity += relation * 300;

                  }

                  person[k].longdead = 1;

                  for(int j = 0; j < person[k].skeleton.num_joints; j++) {

                    //Sever stuff
                    if(findLengthfast(person[k].skeleton.joints[j].velocity) >
                       1500 && person[k].skeleton.joints[j].existing == 1
                       && abs(Random() % 3) != 1) {

                      sprites.MakeSprite(bloodspritedown, .8, 1, .2, .2,
                                         person[k].skeleton.joints[j].position,
                                         person[k].skeleton.joints[j].velocity /
                                         3, 9);

                      person[k].skeleton.DeleteJoint(j);
                      person[k].skeleton.broken = 2;
                      person[k].skeleton.joints[j].existing = 0;
                      person[k].health = -10000;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

    //camera shake
    camera.camerashake -= multiplier;

    if(camera.camerashake < 0)
      camera.camerashake = 0;

    //camera position
    XYZ average;

    if(!zoom)
      average =
          person[0].skeleton.joints[(person[0].skeleton.jointlabels[head])].
          position * (person[0].aimamount / 2 + .5) +
          person[0].skeleton.joints[(person[0].skeleton.jointlabels[neck])].
          position * ((1 - person[0].aimamount) / 2);

    if(zoom)
      average =
          person[0].skeleton.
          joints[(person[0].skeleton.jointlabels[righthand])].position;

    if(person[0].skeleton.free == 0 && thirdperson != 2)
      camera.position =
          person[0].playercoords + DoRotation(average, 0,
                                              person[0].playerrotation, 0);

    if(person[0].skeleton.free == 1 && thirdperson != 2)
      camera.position = average;

    //Restraints
    if(camera.position.y < .1)
      camera.position.y = .1;

    if(thirdperson != 2) {
      oldrot = camera.visrotation;
      oldrot2 = camera.visrotation2;
    }
    //Kill count
    for(int i = 0; i < numpeople; i++) {

      if(person[i].oldhealth > 0 && person[i].health <= 0) {

        if(i == 1)
          alSourcePlay(gSourceID[losesound]);

        if(person[i].type == civiliantype) {

          alSourcePlay(gSourceID[disguisekillsound]);

          score -= 300;
        }

        if(person[i].type == eviltype) {

          alSourcePlay(gSourceID[soulinsound]);

          score += 75;

          if(person[i].weapon.type == knife)
            score += 50;

        }

        person[i].firstlongdead = 0;
      }

      person[i].oldhealth = person[i].health;
    }

    if(slomo == 2) {
      psychicpower -= multiplier * 15;

      if(psychicpower < 0) {

        soundscalefactor = soundscalefactordefault;

        alSourceStop(gSourceID[whichsong]);

        alSourcef(gSourceID[whichsong], AL_MIN_GAIN, 0);
        alSourcef(gSourceID[whichsong], AL_MAX_GAIN, 0);

        if(person[0].weapon.type == knife)
          whichsong = knifesong;

        if(person[0].weapon.type != knife)
          whichsong = shootsong;

        if(type == zombie_type)
          whichsong = zombiesong;

        alSourcef(gSourceID[whichsong], AL_PITCH, 1);
        alSourcePlay(gSourceID[whichsong]);
        alSourcef(gSourceID[whichsong], AL_MIN_GAIN, 1);
        alSourcef(gSourceID[whichsong], AL_MAX_GAIN, 1);

        slomo = 0;

        alSourcePlay(gSourceID[soulinsound]);

        psychicpower = 0;

        flashamount = .5;

        flashr = 1;
        flashg = 1;
        flashb = 1;
      }
    }

    psychicpower += multiplier * 5;

    if(psychicpower > 10)
      psychicpower = 10;

    //3d sound
    float gLoc[3];

    gLoc[0] = camera.position.x / soundscalefactor;
    gLoc[1] = camera.position.y / soundscalefactor;
    gLoc[2] = camera.position.z / soundscalefactor;

    //Set listener position
    alListenerfv(AL_POSITION, gLoc);

    //Set orientation with forward and up vectors
    XYZ upvector;

    upvector = 0;
    upvector.z = -1;

    upvector = DoRotation(upvector, -camera.rotation2 + 90, 0, 0);
    upvector = DoRotation(upvector, 0, 0 - camera.rotation, 0);

    float ori[6];

    ori[0] = -facing.x;
    ori[1] = facing.y;
    ori[2] = facing.z;
    ori[3] = -upvector.x;
    ori[4] = upvector.y;
    ori[5] = upvector.z;

    alListenerfv(AL_ORIENTATION, ori);

    if(person[0].currentanimation == throwanim
       || person[0].currentanimation == diveanim || paused) {

      MoveMouse(oldmouseloc.h, oldmouseloc.v, &mouseloc);

      GetMouse(&mouseloc);
    }

    oldmouseloc = mouseloc;

    if(score < 0)
      score = 0;
}

void Game::Tick()
{
  GetKeys((unsigned long *)theKeyMap);

  if(state == MAINMENU) {
    MainMenuTick();
  } else if(state == GAME) {
    GameTick();
  }
}

