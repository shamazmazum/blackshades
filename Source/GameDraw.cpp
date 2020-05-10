/*
 * GameDraw.cpp
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
#include "TGALoader.h"
#include "Game.h"

extern double multiplier;

extern unsigned int gSourceID[100];
extern unsigned int gSampleSet[100];

extern Camera camera;
extern Sprites sprites;
extern Fog fog;
extern Decals decals;
extern Config config;
extern Environment environment;
extern Font font;

extern float sinefluct;
extern float sinefluctprog;

void Game::SaveScreenshot(const char *path)
{
  int width = config.screenwidth;
  int height = config.screenheight;
  int bpp = config.bpp;

  unsigned int size = width*height*3; //3 = bbp 24
  unsigned char *pixels = (unsigned char *)malloc(size);
  if(!pixels) {
    printf("SaveScreenshot(): malloc failed");
  }

  glFinish(); //finish any drawing

  glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, (GLvoid*)pixels);

  TGAImageRec image;
  image.data = pixels;
  image.bpp = bpp;
  image.sizeX = width;
  image.sizeY = height;

  WriteTGA(path, &image, size);

  free(pixels);
}

void Game::LoadingScreen(float percent)
{
  glLoadIdentity();

  //Clear to black
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Background
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_DEPTH_TEST);     // Disables Depth Testing
  glDisable(GL_CULL_FACE);
  glDisable(GL_LIGHTING);

  glDepthMask(0);

  glMatrixMode(GL_PROJECTION);  // Select The Projection Matrix

  glPushMatrix();               // Store The Projection Matrix

  glLoadIdentity();             // Reset The Projection Matrix

  glOrtho(0, screenwidth, 0, screenheight, -100, 100);  // Set Up An Ortho Screen

  glMatrixMode(GL_MODELVIEW);   // Select The Modelview Matrix

  glPushMatrix();               // Store The Modelview Matrix

  for(int i = 19; i >= 0; i--) {
    glLoadIdentity();           // Reset The Modelview Matrix
    glTranslatef((int)(screenwidth * 0.1875) - i * 1,
                 (int)(screenheight * 0.396) - i * 1, 0);
    glScalef((int)(screenwidth * 0.625) + i * 2, 30 + i * 2, 1);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_BLEND);

    if(i) {
      glColor4f(1 - (float)i / 20 - percent / 100,
                1 - (float)i / 20 - percent / 100,
                1 - (float)i / 20 - percent / 100, 1);
    }

    if(!i)
      glColor4f(0, 0, 0, 1);

    glBegin(GL_QUADS);
    glVertex3f(0, 0, 0.0f);
    glVertex3f(1, 0, 0.0f);
    glVertex3f(1, 1, 0.0f);
    glVertex3f(0, 1, 0.0f);
    glEnd();
  }

  glMatrixMode(GL_PROJECTION);  // Select The Projection Matrix

  glPopMatrix();                // Restore The Old Projection Matrix

  glMatrixMode(GL_MODELVIEW);   // Select The Modelview Matrix

  glPopMatrix();                // Restore The Old Projection Matrix

  glDisable(GL_BLEND);

  glDepthMask(1);

  //Progress
  glDisable(GL_DEPTH_TEST);     // Disables Depth Testing
  glDisable(GL_CULL_FACE);
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);

  glDepthMask(0);

  glMatrixMode(GL_PROJECTION);  // Select The Projection Matrix

  glPushMatrix();               // Store The Projection Matrix

  glLoadIdentity();             // Reset The Projection Matrix

  glOrtho(0, screenwidth, 0, screenheight, -100, 100);  // Set Up An Ortho Screen

  glMatrixMode(GL_MODELVIEW);   // Select The Modelview Matrix

  glPushMatrix();               // Store The Modelview Matrix

  for(int i = 19; i >= 0; i--) {
    glLoadIdentity();           // Reset The Modelview Matrix

    glTranslatef((int)(screenwidth * 0.1875), (int)(screenheight * 0.396), 0);

    if(4 * percent + i * 2 < 400)
      glScalef((int)(screenwidth * 0.006) * percent + i * 2, 30, 1);

    if(4 * percent + i * 2 >= 400)
      glScalef(400, 30, 1);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_BLEND);

    glColor4f(1, 0, 0, .1);

    glBegin(GL_QUADS);
    glVertex3f(0, 0, 0.0f);
    glVertex3f(1, 0, 0.0f);
    glVertex3f(1, 1, 0.0f);
    glVertex3f(0, 1, 0.0f);
    glEnd();
  }

  glMatrixMode(GL_PROJECTION);  // Select The Projection Matrix

  glPopMatrix();                // Restore The Old Projection Matrix

  glMatrixMode(GL_MODELVIEW);   // Select The Modelview Matrix

  glPopMatrix();                // Restore The Old Projection Matrix

  glDisable(GL_BLEND);

  glDepthMask(1);

  //Text
  glEnable(GL_TEXTURE_2D);
  glColor4f(.6 - .6 * percent / 100, 0, 0, 1);

  text.glPrint((int)(screenwidth * 0.4375), (int)(screenheight * 0.406),
               "LOADING...", 1, 1, screenwidth, screenheight);

  SDL_GL_SwapBuffers();
}

void Game::DrawMouse(void)
{
  glMatrixMode(GL_PROJECTION);  // Select The Projection Matrix

  glPushMatrix();               // Store The Projection Matrix

  glLoadIdentity();             // Reset The Projection Matrix

  glOrtho(0, screenwidth, 0, screenheight, -100, 100);  // Set Up An Ortho Screen

  glMatrixMode(GL_MODELVIEW);

  glDisable(GL_TEXTURE_2D);

  Point mouseloc;

  GetMouse(&mouseloc);

  mouseloc.v = screenheight - mouseloc.v;

  glColor4f(.1, 0, 0, 1);

  float size = 5;

  glBegin(GL_TRIANGLES);
  glVertex3f(mouseloc.h, mouseloc.v, 0);
  glVertex3f(mouseloc.h + 2 * size, mouseloc.v - 2 * size, 0);
  glVertex3f(mouseloc.h + .5 * size, mouseloc.v - 2 * size, 0);
  glEnd();

  glColor4f(1, 0, 0, 1);

  glBegin(GL_QUADS);
  glVertex3f(olddrawmouse.h, olddrawmouse.v, 0);
  glVertex3f(mouseloc.h, mouseloc.v, 0);
  glVertex3f(mouseloc.h + 2 * size, mouseloc.v - 2 * size, 0);
  glVertex3f(olddrawmouse.h + 2 * size, olddrawmouse.v - 2 * size, 0);

  glVertex3f(olddrawmouse.h, olddrawmouse.v, 0);
  glVertex3f(mouseloc.h, mouseloc.v, 0);
  glVertex3f(mouseloc.h + .5 * size, mouseloc.v - 2 * size, 0);
  glVertex3f(olddrawmouse.h + .5 * size, olddrawmouse.v - 2 * size, 0);

  glVertex3f(olddrawmouse.h + 2 * size, olddrawmouse.v - 2 * size, 0);
  glVertex3f(mouseloc.h + 2 * size, mouseloc.v - 2 * size, 0);
  glVertex3f(mouseloc.h + .5 * size, mouseloc.v - 2 * size, 0);
  glVertex3f(olddrawmouse.h + .5 * size, olddrawmouse.v - 2 * size, 0);
  glEnd();

  glPopMatrix();

  olddrawmouse = mouseloc;
}

void Game::DrawFlash()
{
  if(flashamount > 0) {

    if(flashamount > 1)
      flashamount = 1;

    flashamount -= multiplier;

    if(flashamount < 0)
      flashamount = 0;

    glDisable(GL_DEPTH_TEST);   // Disables Depth Testing
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);

    glDepthMask(0);

    glMatrixMode(GL_PROJECTION);        // Select The Projection Matrix

    glPushMatrix();             // Store The Projection Matrix

    glLoadIdentity();           // Reset The Projection Matrix

    glOrtho(0, screenwidth, 0, screenheight, -100, 100);        // Set Up An Ortho Screen

    glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix

    glPushMatrix();             // Store The Modelview Matrix

    glLoadIdentity();           // Reset The Modelview Matrix

    glScalef(screenwidth, screenheight, 1);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_BLEND);

    glColor4f(flashr, flashg, flashb, flashamount);

    glBegin(GL_QUADS);
    glVertex3f(0, 0, 0.0f);
    glVertex3f(256, 0, 0.0f);
    glVertex3f(256, 256, 0.0f);
    glVertex3f(0, 256, 0.0f);
    glEnd();

    glMatrixMode(GL_PROJECTION);        // Select The Projection Matrix

    glPopMatrix();              // Restore The Old Projection Matrix

    glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix

    glPopMatrix();              // Restore The Old Projection Matrix

    glEnable(GL_DEPTH_TEST);    // Enables Depth Testing
    glEnable(GL_CULL_FACE);

    glDisable(GL_BLEND);

    glDepthMask(1);

  }
}

void Game::DrawMainMenu()
{
  //Setup fast sine fluctuation
  sinefluct = sin(sinefluctprog);
  sinefluctprog += multiplier * 1.5;

  glLoadIdentity();
  glClearColor(0, 0, 0, 1);
  glDisable(GL_CLIP_PLANE0);
  glDisable(GL_FOG);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Title
  //"Black Shades"
  Image *image =
      (Image *) WindowMgr::getInstance().getRoot()->getWindow("mainmenu")->
      getWindow("title")->getWindow("title_image1");
  image->update(sinefluct, sinefluctprog);      //FIXME: hack

  image =
      (Image *) WindowMgr::getInstance().getRoot()->getWindow("mainmenu")->
      getWindow("title")->getWindow("title_image2");
  image->update(sinefluct, sinefluctprog);      //FIXME: hack

  //"New Game"
  image =
      (Image *) WindowMgr::getInstance().getRoot()->getWindow("mainmenu")->
      getWindow("game")->getWindow("game_image1");
  image->update(sinefluct, sinefluctprog);

  Image *image2 =
      (Image *) WindowMgr::getInstance().getRoot()->getWindow("mainmenu")->
      getWindow("game")->getWindow("game_image2");
  image2->update(sinefluct, sinefluctprog);

  if(mouseoverbutton != 1) {
    image->setActive(0);
    image2->setActive(0);
  }

  if(mouseoverbutton == 1) {
    image->setActive(1);
    image2->setActive(1);
  }

  Label *label =
      (Label *) WindowMgr::getInstance().getRoot()->getWindow("mainmenu")->
      getWindow("game");

  if(!gameinprogress)
    label->setText("New Game");

  if(gameinprogress)
    label->setText("Resume Game");

  image =
      (Image *) WindowMgr::getInstance().getRoot()->getWindow("mainmenu")->
      getWindow("quit")->getWindow("quit_image1");
  image->update(sinefluct, sinefluctprog);

  image2 =
      (Image *) WindowMgr::getInstance().getRoot()->getWindow("mainmenu")->
      getWindow("quit")->getWindow("quit_image2");
  image2->update(sinefluct, sinefluctprog);

  if(mouseoverbutton != 2) {
    image->setActive(0);
    image2->setActive(0);
  }

  if(mouseoverbutton == 2) {
    image->setActive(1);
    image2->setActive(1);
  }

  label =
      (Label *) WindowMgr::getInstance().getRoot()->getWindow("mainmenu")->
      getWindow("quit");

  if(!gameinprogress)
    label->setText("Quit");

  if(gameinprogress)
    label->setText("End Game");

  //WindowMgr::getInstance().draw();
  WindowMgr::getInstance().getRoot()->getWindow("mainmenu")->draw();

  static char string[256] = "";
  //text.glPrint((int)(screenwidth*0.308) - gameinprogress * 15, (int)(screenheight*0.181), string, 1, 1.5, screenwidth, screenheight);

  //High score
  glColor4f(.5 + sinefluct / 5, 0, 0, 1);

  if(!config.beatgame)
    sprintf(string, "High Score:  %d", config.highscore);

  if(config.beatgame)
    sprintf(string,
            "High Score:  %d *COMPLETED* Please vote for Black Shades at iDevGames.com!",
            config.highscore);

  text.glPrint(0, 0, string, 1, .8, screenwidth, screenheight);

  //Mandatory udg text
  glColor4f(.3 - sinefluct / 20, .3 - sinefluct / 20, .3 - sinefluct / 20, 1);
  sprintf(string, "uDevGame 2002 Entry - Visit iDevGames.com for more games!");
  text.glPrint((int)(screenwidth * 0.961), (int)(screenheight * 1.563), string,
               1, .6, screenwidth, screenheight);

  DrawMouse();

  DrawFlash();
}

void Game::DrawGame(void)
{
#if 0
  //FIXME: is this still an isssue?
  //If flashing to fix menu bug, go back to menu after a frame
  if(mainmenu == 2)
    mainmenu = 1;
#endif

  glLoadIdentity();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);

  glDisable(GL_CLIP_PLANE0);

  //Visions
  sinefluct = sin(sinefluctprog);
  sinefluctprog += multiplier * 3;

  int visions = config.visions;
  viewdistance = environment.viewdistance;

  if(visions == 0) {

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

    //Change fov if zooming with scope
    if(!zoom)
      ReSizeGLScene(90, .1, viewdistance);
    else
      ReSizeGLScene(10, .6, viewdistance);

    config.nocolors = 0;
  }

  if(visions == 1) {

    //light
    GLfloat LightAmbient[] = { 0, 0, 0, 1.0f };
    GLfloat LightDiffuse[] = { .1 + sinefluct / 5, 0, 0, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);

    glEnable(GL_LIGHT0);

    fog.fogcolor.r = (sinefluct / 4 + .5);
    fog.fogcolor.g = 0;
    fog.fogcolor.b = 0;

    fog.SetFog(fog.fogcolor.r, fog.fogcolor.g, fog.fogcolor.b, 0,
               viewdistance * .8 * .5 * (sinefluct / 4 + .3),
               sinefluct / 3 + .7);

    glClearColor(fog.fogcolor.r, fog.fogcolor.g, fog.fogcolor.b, 1);

    ReSizeGLScene(120 - sinefluct * 20, .3, viewdistance);

    glRotatef(sinefluct * 10, 0, 0, .1);

    config.nocolors = 1;

    //Pitch higher if moving for effect

    if(person[0].currentanimation == idleanim)
      alSourcef(gSourceID[visionsound], AL_PITCH, 1);

    if(person[0].currentanimation != idleanim)
      alSourcef(gSourceID[visionsound], AL_PITCH, 2);
  }
  //Camera
  float bluramount = .1 * config.blurness;
  blur = 1 - blur;

  //Set rotation/position
  int thirdperson = config.thirdperson;

  if(thirdperson)
    glTranslatef(camera.targetoffset.x, camera.targetoffset.y,
                 camera.targetoffset.z);

  if(thirdperson != 2 && (person[0].skeleton.free != 1 || thirdperson)) {

    glRotatef(camera.visrotation2 + -bluramount / 2 +
              (float)blur * bluramount, 1, 0, 0);
    glRotatef(camera.visrotation + -bluramount / 2 +
              (float)blur * bluramount, 0, 1, 0);
  }

  if(thirdperson == 0 && person[0].skeleton.free == 1) {

    glRotatef(person[0].skeleton.
              joints[(person[0].skeleton.jointlabels[head])].rotate3, 0, 1, 0);
    glRotatef(180 -
              (person[0].skeleton.
               joints[(person[0].skeleton.jointlabels[head])].rotate2 +
               90), 0, 0, 1);
    glRotatef(person[0].skeleton.
              joints[(person[0].skeleton.jointlabels[head])].rotate1 + 90,
              0, 1, 0);
  }

  if(thirdperson == 2) {
    glRotatef(oldrot2 + -bluramount / 2 + (float)blur * bluramount, 1, 0, 0);
    glRotatef(oldrot + -bluramount / 2 + (float)blur * bluramount, 0, 1, 0);
  }
  //Shake camera if grenade explosion
  if(camera.camerashake > 0) {
    if(!(person[0].aiming < 1 || person[0].weapon.type == grenade || thirdperson)) {
      camera.camerashake = 0;
    }

    glTranslatef((float)(Random() % 100) / 100 * camera.camerashake,
                 (float)(Random() % 100) / 100 * camera.camerashake,
                 (float)(Random() % 100) / 100 * camera.camerashake);
  }

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

  if(visions == 0)
    glEnable(GL_LIGHTING);

  if(visions == 1)
    glDisable(GL_LIGHTING);

  //Draw blocks
  glEnable(GL_BLEND);

  XYZ move;

  int beginx, endx;
  int beginz, endz;

  int distsquared;

  //Only nearby blocks
  beginx =
      (int)(camera.position.x - viewdistance +
            block_spacing / 2) / block_spacing - 2;

  if(beginx < 0)
    beginx = 0;

  beginz =
      (int)(camera.position.z - viewdistance +
            block_spacing / 2) / block_spacing - 2;

  if(beginz < 0)
    beginz = 0;

  endx =
      (int)(camera.position.x + viewdistance +
            block_spacing / 2) / block_spacing + 2;

  if(endx > num_blocks - 1)
    endx = num_blocks - 1;

  endz =
      (int)(camera.position.z + viewdistance +
            block_spacing / 2) / block_spacing + 2;

  if(endz > num_blocks - 1)
    endz = num_blocks - 1;

  bool draw;
  int whichtri;
  XYZ collpoint;

  for(int i = beginx; i <= endx; i++) {
    for(int j = beginz; j <= endz; j++) {
      drawn[i][j] = 1;
    }
  }

  if(beginx < endx && beginz < endz)

    for(int i = beginx; i <= endx; i++) {
      for(int j = beginz; j <= endz; j++) {
        draw = 1;

        //Only draw if visible
        distsquared =
            (int)(((i) * block_spacing -
                   camera.position.x) * ((i) * block_spacing -
                                         camera.position.x) +
                  ((j) * block_spacing -
                   camera.position.z) * ((j) * block_spacing -
                                         camera.position.z));

        if(distsquared >
           (viewdistance * viewdistance + block_spacing * block_spacing))
          draw = 0;

        if(draw && citytype[i][j] != 3
           && !frustum.CubeInFrustum((i) * block_spacing, 0,
                                     (j) * block_spacing, block_spacing))
          draw = 0;

        if(draw && citytype[i][j] != 3
           && !frustum.SphereInFrustum(blocks[citytype[i][j]].
                                       boundingspherecenter.x +
                                       (i) * block_spacing,
                                       blocks[citytype[i][j]].
                                       boundingspherecenter.y,
                                       blocks[citytype[i][j]].
                                       boundingspherecenter.z +
                                       (j) * block_spacing,
                                       blocks[citytype[i][j]].
                                       boundingsphereradius))
          draw = 0;

        if(draw) {
          glPushMatrix();
          glTranslatef(i * block_spacing, 0, j * block_spacing);
          glRotatef(cityrotation[i][j] * 90, 0, 1, 0);
          blocks[citytype[i][j]].draw();
          glPopMatrix();
        }

        if(!draw) {
          move.y = 0;
          move.x = i * block_spacing;
          move.z = j * block_spacing;

          if(findDistancefast(move, camera.position) < 300000)
            drawn[i][j] = 0;
        }
      }
    }
  //Decals
  decals.draw();

  //Occluding blocks
  beginx = (int)(camera.position.x + block_spacing / 2) / block_spacing - 2;

  if(beginx < 0)
    beginx = 0;

  beginz = (int)(camera.position.z + block_spacing / 2) / block_spacing - 2;

  if(beginz < 0)
    beginz = 0;

  endx = (int)(camera.position.x + block_spacing / 2) / block_spacing + 2;

  if(endx > num_blocks - 1)
    endx = num_blocks - 1;

  endz = (int)(camera.position.z + block_spacing / 2) / block_spacing + 2;

  if(endz > num_blocks - 1)
    endz = num_blocks - 1;

  float M[16];
  float size = 20;
  XYZ drawpoint;

  //Draw people
  if(visions == 1)
    fog.SetFog(fog.fogcolor.r, fog.fogcolor.g, fog.fogcolor.b, 0,
               viewdistance * .8 * .5 * (-sinefluct / 4 + .3),
               -sinefluct / 3 + .7);

  glColor4f(1, 1, 1, 1);

  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_BLEND);

  for(int i = 0; i < numpeople; i++) {
    draw = 1;

    if(person[i].skeleton.free < 1) {

      if(person[i].whichblockx >= 0
         && person[i].whichblockx < num_blocks
         && person[i].whichblocky >= 0 && person[i].whichblocky < num_blocks) {

        if(!drawn[person[i].whichblockx][person[i].whichblocky])
          draw = 0;

      } else
        draw = 0;

      if(draw)

        if(!frustum.
           CubeInFrustum(person[i].playercoords.x,
                         person[i].playercoords.y, person[i].playercoords.z, 5))
          draw = 0;

      if(draw)

        if(findDistancefast(person[i].playercoords, camera.position) > 1000000)
          draw = 0;

      if(draw)

        for(int j = beginx; j <= endx; j++) {

          for(int k = beginz; k <= endz; k++) {

            if(draw) {
              move.y = 0;
              move.x = j * block_spacing;
              move.z = k * block_spacing;

              if(findDistancefast(move, camera.position) < 100000) {

                whichtri =
                    blockocclude.LineCheck2(camera.position,
                                            person[i].
                                            playercoords, &collpoint, move, 0);

                if(whichtri != -1)
                  draw = 0;

              }
            }
          }
        }

      if(draw) {
        move.y = 0;
        move.x = person[i].whichblockx * block_spacing;
        move.z = person[i].whichblocky * block_spacing;

        whichtri =
            blockocclude.LineCheck2(camera.position,
                                    person[i].playercoords,
                                    &collpoint, move, 0);

        if(whichtri != -1)
          draw = 0;
      }

      if(i == 0)
        draw = 1;
    }

    if(person[i].skeleton.free == 1) {

      if(draw)
        if(!person[i].skeleton.broken
           && !frustum.CubeInFrustum(person[i].averageloc.x,
                                     person[i].averageloc.y,
                                     person[i].averageloc.z, 5))
          draw = 0;

      if(draw)
        if(findDistancefast(person[i].averageloc, camera.position) > 1000000)
          draw = 0;

      if(draw)
        if(person[i].skeleton.joints[0].position.y < -2)
          draw = 0;

      for(int j = beginx; j <= endx; j++) {

        for(int k = beginz; k <= endz; k++) {

          if(draw) {
            move.y = 0;
            move.x = j * block_spacing;
            move.z = k * block_spacing;

            if(findDistancefast(move, camera.position) < 100000) {

              whichtri =
                  blockocclude.LineCheck2(camera.position,
                                          person[i].averageloc,
                                          &collpoint, move, 0);

              if(whichtri != -1)
                draw = 0;

            }
          }
        }
      }

      if(draw) {
        move.y = 0;
        move.x = person[i].whichblockx * block_spacing;
        move.z = person[i].whichblocky * block_spacing;

        whichtri =
            blockocclude.LineCheck2(camera.position,
                                    person[i].averageloc, &collpoint, move, 0);

        if(whichtri != -1)
          draw = 0;

      }

      if(i == 0)
        draw = 1;
    }

    if(draw && person[i].existing == 1) {

      if((findDistancefast(person[i].playercoords, camera.position) <
          100000 + zoom * 3000000 && person[i].skeleton.free < 1)
         || (findDistancefast(person[i].averageloc, camera.position) <
             100000 + zoom * 3000000 && person[i].skeleton.free >= 1)) {

        glPushMatrix();

        if(person[i].skeleton.free == 0) {

          glTranslatef(person[i].playercoords.x,
                       person[i].playercoords.y, person[i].playercoords.z);

          glRotatef(person[i].playerrotation, 0, 1, 0);

          if(i != 0 || visions == 0)
            person[i].DoAnimations(i);

          if(i == 0 && visions == 1)
            person[i].DoAnimationslite(i);

        }

        if(visions == 1) {
          config.nocolors = 1;

          if(person[i].type == eviltype)
            config.nocolors = 2;

          if(person[i].type == viptype)
            config.nocolors = 3;
        }
        
        //where the people are drawn...
        if(!(visions==1 && i == 0) && !(zoom == 1 && i == 0))
          person[i].DrawSkeleton(i);

        glPopMatrix();
      } else {

        glPushMatrix();

        if(person[i].skeleton.free < 1)
          person[i].DoAnimationslite(i);

        glColor4f(1, 1, 1, 1);

        glEnable(GL_BLEND);

        glDisable(GL_CULL_FACE);

        glEnable(GL_TEXTURE_2D);

        glDisable(GL_LIGHTING);

        glDepthMask(0);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if(person[i].skeleton.free < 1) {

          glBindTexture(GL_TEXTURE_2D, personspritetextureptr);

          glTranslatef(person[i].playercoords.x,
                       person[i].playercoords.y + size / 2 * .3,
                       person[i].playercoords.z);
        }

        if(person[i].skeleton.free == 1) {

          glBindTexture(GL_TEXTURE_2D, deadpersonspritetextureptr);

          glTranslatef(person[i].averageloc.x,
                       person[i].averageloc.y + size / 2 * .3,
                       person[i].averageloc.z);
        }

        glGetFloatv(GL_MODELVIEW_MATRIX, M);

        drawpoint.x = M[12];
        drawpoint.y = M[13];
        drawpoint.z = M[14];

        glLoadIdentity();

        glTranslatef(drawpoint.x, drawpoint.y, drawpoint.z);

        glBegin(GL_TRIANGLES);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(.3f * size, .3f * size, 0.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-.3f * size, .3f * size, 0.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(.3f * size, -.3f * size, 0.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-.3f * size, -.3f * size, 0.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(.3f * size, -.3f * size, 0.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-.3f * size, .3f * size, 0.0f);
        glEnd();

        glPopMatrix();

        glDepthMask(1);

        glDisable(GL_TEXTURE_2D);

        glEnable(GL_CULL_FACE);

        if(visions != 1)
          glEnable(GL_LIGHTING);

      }

    }

    if(person[i].skeleton.free < 1 && !draw)
      person[i].DoAnimationslite(i);

    if(!person[i].existing)

      if(!draw
         || findDistancefast(person[i].playercoords, camera.position) > 10000) {
        person[i].existing = 1;
      }

  }

  glDisable(GL_COLOR_MATERIAL);
  glDisable(GL_BLEND);

  //Attacker psychicness 
  for(int i = 0; i < numpeople; i++) {

    if(person[i].killtarget > -1 && person[i].killtargetvisible
       && person[i].skeleton.free == 0
       && person[person[i].killtarget].skeleton.free == 0) {

      sprites.MakeSprite(bulletinstant,
                         (shotdelayamount / difficulty -
                          person[i].weapon.shotdelay) / shotdelayamount /
                         difficulty / 2, 1,
                         person[i].weapon.shotdelay / shotdelayamount /
                         difficulty,
                         person[i].weapon.shotdelay / shotdelayamount /
                         difficulty,
                         DoRotation(person[i].skeleton.
                                    joints[person[i].skeleton.
                                           jointlabels[lefthand]].
                                    position, 0,
                                    person[i].playerrotation,
                                    0) + person[i].playercoords,
                         person[person[i].killtarget].skeleton.
                         joints[person[person[i].killtarget].skeleton.
                                jointlabels[abdomen]].position +
                         person[person[i].killtarget].playercoords,
                         person[i].weapon.shotdelay * 2);

    }

    if(person[i].killtarget > -1 && person[i].killtargetvisible
       && person[i].skeleton.free == 0
       && person[person[i].killtarget].skeleton.free != 0) {

      sprites.MakeSprite(bulletinstant,
                         (shotdelayamount / difficulty -
                          person[i].weapon.shotdelay) / shotdelayamount /
                         difficulty / 2, 1,
                         person[i].weapon.shotdelay / shotdelayamount /
                         difficulty,
                         person[i].weapon.shotdelay / shotdelayamount /
                         difficulty,
                         DoRotation(person[i].skeleton.
                                    joints[person[i].skeleton.
                                           jointlabels[lefthand]].
                                    position, 0,
                                    person[i].playerrotation,
                                    0) + person[i].playercoords,
                         person[person[i].killtarget].skeleton.
                         joints[person[person[i].killtarget].skeleton.
                                jointlabels[abdomen]].position,
                         person[i].weapon.shotdelay * 2);

    }

  }

  //Sprites
  glEnable(GL_CLIP_PLANE0);
  sprites.draw();

  glDisable(GL_CLIP_PLANE0);
  glDisable(GL_FOG);

  //Zoom
  glAlphaFunc(GL_GREATER, 0.01);

  if(zoom) {
    glDisable(GL_DEPTH_TEST);   // Disables Depth Testing
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);

    glDepthMask(0);

    glMatrixMode(GL_PROJECTION);        // Select The Projection Matrix

    glPushMatrix();             // Store The Projection Matrix

    glLoadIdentity();           // Reset The Projection Matrix

    glOrtho(0, screenwidth, 0, screenheight, -100, 100);        // Set Up An Ortho Screen

    glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix

    glPushMatrix();             // Store The Modelview Matrix

    glLoadIdentity();           // Reset The Modelview Matrix

    glScalef(screenwidth, screenheight, 1);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);

    glColor4f(.5, .5, .5, 1);

    glBindTexture(GL_TEXTURE_2D, scopetextureptr);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(0, 0, 0.0f);

    glTexCoord2f(1, 0);
    glVertex3f(1, 0, 0.0f);

    glTexCoord2f(1, 1);
    glVertex3f(1, 1, 0.0f);

    glTexCoord2f(0, 1);
    glVertex3f(0, 1, 0.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);        // Select The Projection Matrix

    glPopMatrix();              // Restore The Old Projection Matrix

    glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix

    glPopMatrix();              // Restore The Old Projection Matrix

    glEnable(GL_DEPTH_TEST);    // Enables Depth Testing
    glEnable(GL_CULL_FACE);

    glDisable(GL_BLEND);

    glDepthMask(1);

  }

  //Flash
  if(flashamount > 0) {

    if(flashamount > 1)
      flashamount = 1;

    flashamount -= multiplier;

    if(flashamount < 0)
      flashamount = 0;

    glDisable(GL_DEPTH_TEST);   // Disables Depth Testing
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);

    glDepthMask(0);

    glMatrixMode(GL_PROJECTION);        // Select The Projection Matrix

    glPushMatrix();             // Store The Projection Matrix

    glLoadIdentity();           // Reset The Projection Matrix

    glOrtho(0, screenwidth, 0, screenheight, -100, 100);        // Set Up An Ortho Screen

    glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix

    glPushMatrix();             // Store The Modelview Matrix

    glLoadIdentity();           // Reset The Modelview Matrix

    glScalef(screenwidth, screenheight, 1);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_BLEND);

    glColor4f(flashr, flashg, flashb, flashamount);

    glBegin(GL_QUADS);
    glVertex3f(0, 0, 0.0f);
    glVertex3f(256, 0, 0.0f);
    glVertex3f(256, 256, 0.0f);
    glVertex3f(0, 256, 0.0f);
    glEnd();

    glMatrixMode(GL_PROJECTION);        // Select The Projection Matrix

    glPopMatrix();              // Restore The Old Projection Matrix

    glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix

    glPopMatrix();              // Restore The Old Projection Matrix

    glEnable(GL_DEPTH_TEST);    // Enables Depth Testing
    glEnable(GL_CULL_FACE);

    glDisable(GL_BLEND);

    glDepthMask(1);

  }

  if(person[0].skeleton.free > 0 && thirdperson != 2) {
    glDisable(GL_DEPTH_TEST);   // Disables Depth Testing
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);

    glDepthMask(0);

    glMatrixMode(GL_PROJECTION);        // Select The Projection Matrix

    glPushMatrix();             // Store The Projection Matrix

    glLoadIdentity();           // Reset The Projection Matrix

    glOrtho(0, screenwidth, 0, screenheight, -100, 100);        // Set Up An Ortho Screen

    glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix

    glPushMatrix();             // Store The Modelview Matrix

    glLoadIdentity();           // Reset The Modelview Matrix

    glScalef(screenwidth, screenheight, 1);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_BLEND);

    glColor4f(0, 0, 0, 1 - person[0].longdead);

    glBegin(GL_QUADS);
    glVertex3f(0, 0, 0.0f);
    glVertex3f(256, 0, 0.0f);
    glVertex3f(256, 256, 0.0f);
    glVertex3f(0, 256, 0.0f);
    glEnd();

    glMatrixMode(GL_PROJECTION);        // Select The Projection Matrix

    glPopMatrix();              // Restore The Old Projection Matrix

    glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix

    glPopMatrix();              // Restore The Old Projection Matrix

    glEnable(GL_DEPTH_TEST);    // Enables Depth Testing
    glEnable(GL_CULL_FACE);

    glDisable(GL_BLEND);

    glDepthMask(1);

  }

  //Text
  glEnable(GL_TEXTURE_2D);

  char string[256] = "";
  Window *ingame = WindowMgr::getInstance().getRoot()->getWindow("ingame");

  //In Game text
  if(!config.debug) {
    // This is the player playing
    Person *player = person;
    int weapon_type = player->weapon.type;

    switch (weapon_type) {
      case nogun:
        sprintf(string, "UNARMED");
        break;
      case knife:
        sprintf(string, "KNIFE");
        player->weapon.ammo = 0;
        break;
      case assaultrifle:
        sprintf(string, "ASSAULT RIFLE");
        break;
      case shotgun:
        sprintf(string, "SHOTGUN");
        break;
      case sniperrifle:
        sprintf(string, "SNIPER RIFLE");
        break;
      case grenade:
        sprintf(string, "HAND GRENADE");
        break;
      case handgun1:
        sprintf(string, "MAGNUM");
        break;
      case handgun2:
        sprintf(string, "HANDGUN");
        break;
      default:
        sprintf(string, "N\\A");
        break;
    }

    Label *label = (Label *)ingame->getWindow("weapon");
    label->setText(string);

    if(weapon_type != nogun && weapon_type != knife
       && weapon_type != grenade) {

      sprintf(string, "Magazines");
      Label *label = (Label *)ingame->getWindow("magazines");
      label->setText(string);

      sprintf(string, "0%d", (int)player->reloads[weapon_type]);
      label = (Label *)ingame->getWindow("magazines_num");
      label->setText(string);

      sprintf(string, "Ammo");
      label = (Label *)ingame->getWindow("ammo");
      label->setText(string);

      if(player->weapon.ammo > 9)
        sprintf(string, "%d", (int)player->weapon.ammo);
      else
        sprintf(string, "0%d", (int)player->weapon.ammo);

      label = (Label *)ingame->getWindow("ammo_num");
      label->setText(string);
    } else if(weapon_type == grenade) {
      sprintf(string, "Grenades Left:  %d",
              (int)player->reloads[weapon_type] + 1);
      label = (Label *)ingame->getWindow("ammo");
      label->setText(string);
    } //otherwise nogun/knife

    sprintf(string, "%d", score);
    label = (Label *)ingame->getWindow("score_val");
    label->setText(string);

    int time_remain = (int)timeremaining % 60;
    // padding the time correctly
    if(time_remain > 9) {
      sprintf(string, "%d:%d", (int)(timeremaining / 60), time_remain);
    } else {
      sprintf(string, "%d:0%d", (int)(timeremaining / 60),
              (int)timeremaining % 60);
    }

    label = (Label *)ingame->getWindow("time_val");
    label->setText(string);

    label = (Label *)ingame->getWindow("mission");
    sprintf(string, "Mission %d", mission);
    label->setText(string);
  }

  if(config.debug) {
    sprintf(string, "The framespersecond is %d out of maximum %d.",
            (int)framespersecond + 1, maxfps);
    text.glPrint(10, 30, string, 0, .8, screenwidth, screenheight);

    switch (enemystate) {
      case 0:
        sprintf(string, "Enemies are in random assassination mode.");
        break;
      case 1:
        sprintf(string, "Enemies are in passive mode.");
        break;
      case 2:
        sprintf(string, "Enemies are in DIE!!!! mode.");
        break;
    }

    text.glPrint(10, 20, string, 0, .8, screenwidth, screenheight);

    sprintf(string,
            "You have pointlessly shot or beaten %d unarmed civilians.",
            civkills);
    text.glPrint(10, 60, string, 0, .8, screenwidth, screenheight);

    sprintf(string, "You have incapacitated %d assassins.", goodkills);
    text.glPrint(10, 70, string, 0, .8, screenwidth, screenheight);

    sprintf(string, "You have allowed %d successful murders.", badkills);
    text.glPrint(10, 80, string, 0, .8, screenwidth, screenheight);
  }

  WindowMgr::getInstance().getRoot()->getWindow("ingame")->draw();
}

void DrawHelp()
{
  glLoadIdentity();
  glClearColor(0, 0, 0, 1.0);
  glDisable(GL_CLIP_PLANE0);
  glDisable(GL_FOG);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  WindowMgr::getInstance().getRoot()->getWindow("helpscreen")->draw();
}

int Game::DrawGLScene(void)
{
  switch (state) {
    case INIT:
      //TODO: loading screen moved here?
      break;
    case MAINMENU:
      DrawMainMenu();
      break;
    case GAME:
      DrawGame();
      break;
    case HELP:
      DrawHelp();
      break;
    default:
      return 0;
      break;
  }

  return 1;
}
