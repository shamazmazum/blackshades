/*
 * Viewer.cpp
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

#include "Constants.h"
#include "Viewer.h"

Animation animation[30];
Model skeletonmodels[10];
Model gunmodels[11];
Costume costume[10];
Person person; //just the one model to view
Sprites sprites;
Decals decals;
Camera camera;
//Skeleton testskeleton;
Config config;
Environment environment;

Fog fog;

//#define setupSkel(skel, model)   

Viewer::setup()
{
  if(!initialized) {

    //Foggy env settings
    environment.viewdistance = 500;
    environment.fogcolor.setColor(0.5, 0.5, 0.5);

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

    //Setup camera
    camera.position = 0;
    camera.position.x = num_blocks / 2 * block_spacing + block_spacing / 2;
    camera.position.z = num_blocks / 2 * block_spacing + block_spacing / 2;
    camera.position.y = 30;
    camera.oldposition = camera.position;

    street.load((unsigned char *)":Data:Models:streetsubdivided2.solid");
    street.Rotate(90, 0, 0);
    street.Scale(.01, .01, .01);
    street.CalculateNormals();

    Bigstreet = street;
    Bigstreet.Scale(10000, 10000, 10000);

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

    testskeleton.Load((char *)":Data:Skeleton:Basic Figure");
    animation[idleanim].Load((char *)":Data:Animations:Breathe");
    animation[joganim].Load((char *)":Data:Animations:Run");
    animation[pistolaimanim].Load((char *)":Data:Animations:PistolAim");

    animation[walkanim].Load((char *)":Data:Animations:Walk");
    animation[rifleholdanim].Load((char *)":Data:Animations:Riflehold");
    animation[rifleaimanim].Load((char *)":Data:Animations:Rifleaim");
    animation[assaultrifleaimanim].
        Load((char *)":Data:Animations:AssaultRifleaim");

    animation[crouchanim].Load((char *)":Data:Animations:Crouch");

    animation[headpainanim].Load((char *)":Data:Animations:Headshot");
    animation[chestpainanim].Load((char *)":Data:Animations:Chestshot");
    animation[stomachpainanim].Load((char *)":Data:Animations:Stomachshot");
    animation[rightarmpainanim].Load((char *)":Data:Animations:Rightarmshot");
    animation[leftarmpainanim].Load((char *)":Data:Animations:Leftarmshot");

    animation[rightlegpainanim].Load((char *)":Data:Animations:Rightlegshot");
    animation[leftlegpainanim].Load((char *)":Data:Animations:Leftlegshot");
    animation[riflehitanim].Load((char *)":Data:Animations:Riflehit");
    animation[grenadeaimanim].Load((char *)":Data:Animations:grenadeaim");
    animation[grenadechargeanim].Load((char *)":Data:Animations:grenadecharge");

    animation[grenadethrowanim].Load((char *)":Data:Animations:grenadethrow");
    animation[zombieeatanim].Load((char *)":Data:Animations:Zombiemunch");
    animation[zombiejoganim].Load((char *)":Data:Animations:ZombieRun");
    animation[zombiewalkanim].Load((char *)":Data:Animations:Zombiewalk");

    animation[getupfrontanim].Load((char *)":Data:Animations:Getupfromfront");
    animation[getupbackanim].Load((char *)":Data:Animations:Getupfromback",
                                  180);
    animation[diveanim].Load((char *)":Data:Animations:Dive");
    animation[throwanim].Load((char *)":Data:Animations:Aikidothrow");
    animation[thrownanim].Load((char *)":Data:Animations:Aikidothrown");

    person.skeleton.Load((char *)":Data:Skeleton:Basic Figure");
  }
}

void Viewer::Tick()
{
    //Mouse look
    //if((person[0].aimamount <= 0)) {
      camera.rotation = camera.visrotation;
      camera.rotation2 = camera.visrotation2;

      mousesensitivity = config.usermousesensitivity;
    //}

    GetMouseRel(&mouseloc);

    mouserotation = (mouseloc.h / 1.3888) * mousesensitivity;
    mouserotation2 = (mouseloc.v / 1.3888) * mousesensitivity;

}

void Viewer::DrawGLScene()
{
  glLoadIdentity();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);

  glDisable(GL_CLIP_PLANE0);

  //Visions
  sinefluct = sin(sinefluctprog);
  sinefluctprog += multiplier * 3;

  int visions = config.visions;
  viewdistance = environment.viewdistance;

    fog.fogcolor.setColor(environment.fogcolor);
    fog.SetFog(environment.fogcolor, 0, viewdistance * .8, .2);

    glClearColor(fog.fogcolor.r, fog.fogcolor.g, fog.fogcolor.b, 1);

    if(environment.type == sunny_environment) {

      GLfloat LightAmbient[] =
          { fog.fogcolor.r / 4, fog.fogcolor.g / 4, fog.fogcolor.b / 4, 1.0f };
      GLfloat LightDiffuse[] =
          { fog.fogcolor.r * 1.6, fog.fogcolor.g * 1.6, fog.fogcolor.r * 1.6,
1.0f };

      glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
      glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
    } else {

      GLfloat LightAmbient[] =
          { fog.fogcolor.r * .8, fog.fogcolor.g * .8, fog.fogcolor.b * .8,
1.0f };
      GLfloat LightDiffuse[] =
          { fog.fogcolor.r * .8, fog.fogcolor.g * .8, fog.fogcolor.r * .8,
1.0f };

      glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
      glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
    }

    glEnable(GL_LIGHT0);

    //Don't think this needs to be done every time
    ReSizeGLScene(90, .1, viewdistance);

    config.nocolors = 0;

  //Camera
  camera.Apply();

  glPushMatrix();
  glClipPlane(GL_CLIP_PLANE0, eqn);
  glDisable(GL_CLIP_PLANE0);
  glPopMatrix();

  frustum.GetFrustum();

  GLfloat LightPosition[] = { -.5, 1, -.8, 0.0f };

  glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

  glDisable(GL_TEXTURE_2D);

  glEnable(GL_FOG);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_CULL_FACE);

  glDepthMask(1);

  //Draw street
  glPushMatrix();

  glDepthMask(0);

  glDisable(GL_DEPTH_TEST);

  glEnable(GL_LIGHTING);

  glTranslatef(camera.position.x, 0, camera.position.z);

  glScalef(viewdistance * 0.05, 1, viewdistance * 0.05);

  if(visions == 0)
    street.draw(.22, .22, .22);

  if(visions == 1)
    street.draw(0, 0, 0);

  glEnable(GL_DEPTH_TEST);

  glDepthMask(1);

  glPopMatrix();

  ///Draw person
  glPushMatrix();
  
  glTranslatef(person.playercoords.x,
               person.playercoords.y, person.playercoords.z);

  glRotatef(person.playerrotation, 0, 1, 0);

  person.DoAnimations(1);

  person.DrawSkeleton(1);

  glPopMatrix();
}

void Viewer::EventLoop()
{
  unsigned char theKeyMap[16];

  ///Frame limiting
  maxfps = 90;
  int interval = 1000/maxfps, sleep;
  float last_calc = SDL_GetTicks();

  float sps = 90; //needed?
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
     * Should be about 80-90 per second.
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

    if(DrawGLScene())
      SDL_GL_SwapBuffers();
    else
      gQuit = true;

    oldmult = multiplier;

    GetKeys((unsigned long *)theKeyMap);

    if(IsKeyDown(theKeyMap, MAC_Q_KEY) || IsKeyDown(theKeyMap, MAC_ESCAPE_KEY)) {
      gQuit = true;
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
      std::cout << "sps: " << spscount << std::endl;
      spscount = 0;
      last_calc = end;
    }

    multiplier = 0.0110f; //bizarely correct
    
    if(multiplier > 1)
      multiplier = 1;

    if(multiplier < .00001)
      multiplier = .00001;

    if(paused)
      multiplier = 0;
  }
}

Viewer &Viewer::getInstance()
{
  MutexLocker obtain_lock(m);
  if(instance.get() == 0)
    instance.reset(new Viewer);
  return *instance;
}

std::unique_ptr <Viewer> Viewer::instance;
Mutex Viewer::m;

