/*
 * SDL_funcs.cpp
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

#include "Game.h"
#include "MacInput.h"

#include "SDL_funcs.h"
extern Config config;

static int sdlkeymap[SDLK_LAST];

static unsigned char ourkeys[16];

void init_sdlkeymap()
{
  int i;

  for(i = 0; i < SDLK_LAST; i++) {
    sdlkeymap[i] = -1;
  }

  sdlkeymap[SDLK_1] = MAC_1_KEY;
  sdlkeymap[SDLK_2] = MAC_2_KEY;
  sdlkeymap[SDLK_3] = MAC_3_KEY;
  sdlkeymap[SDLK_4] = MAC_4_KEY;
  sdlkeymap[SDLK_5] = MAC_5_KEY;
  sdlkeymap[SDLK_6] = MAC_6_KEY;
  sdlkeymap[SDLK_7] = MAC_7_KEY;
  sdlkeymap[SDLK_8] = MAC_8_KEY;
  sdlkeymap[SDLK_9] = MAC_9_KEY;
  sdlkeymap[SDLK_0] = MAC_0_KEY;
  sdlkeymap[SDLK_KP1] = MAC_NUMPAD_1_KEY;
  sdlkeymap[SDLK_KP2] = MAC_NUMPAD_2_KEY;
  sdlkeymap[SDLK_KP3] = MAC_NUMPAD_3_KEY;
  sdlkeymap[SDLK_KP4] = MAC_NUMPAD_4_KEY;
  sdlkeymap[SDLK_KP5] = MAC_NUMPAD_5_KEY;
  sdlkeymap[SDLK_KP6] = MAC_NUMPAD_6_KEY;
  sdlkeymap[SDLK_KP7] = MAC_NUMPAD_7_KEY;
  sdlkeymap[SDLK_KP8] = MAC_NUMPAD_8_KEY;
  sdlkeymap[SDLK_KP9] = MAC_NUMPAD_9_KEY;
  sdlkeymap[SDLK_KP0] = MAC_NUMPAD_0_KEY;
  sdlkeymap[SDLK_a] = MAC_A_KEY;
  sdlkeymap[SDLK_b] = MAC_B_KEY;
  sdlkeymap[SDLK_c] = MAC_C_KEY;
  sdlkeymap[SDLK_d] = MAC_D_KEY;
  sdlkeymap[SDLK_e] = MAC_E_KEY;
  sdlkeymap[SDLK_f] = MAC_F_KEY;
  sdlkeymap[SDLK_g] = MAC_G_KEY;
  sdlkeymap[SDLK_h] = MAC_H_KEY;
  sdlkeymap[SDLK_i] = MAC_I_KEY;
  sdlkeymap[SDLK_j] = MAC_J_KEY;
  sdlkeymap[SDLK_k] = MAC_K_KEY;
  sdlkeymap[SDLK_l] = MAC_L_KEY;
  sdlkeymap[SDLK_m] = MAC_M_KEY;
  sdlkeymap[SDLK_n] = MAC_N_KEY;
  sdlkeymap[SDLK_o] = MAC_O_KEY;
  sdlkeymap[SDLK_p] = MAC_P_KEY;
  sdlkeymap[SDLK_q] = MAC_Q_KEY;
  sdlkeymap[SDLK_r] = MAC_R_KEY;
  sdlkeymap[SDLK_s] = MAC_S_KEY;
  sdlkeymap[SDLK_t] = MAC_T_KEY;
  sdlkeymap[SDLK_u] = MAC_U_KEY;
  sdlkeymap[SDLK_v] = MAC_V_KEY;
  sdlkeymap[SDLK_w] = MAC_W_KEY;
  sdlkeymap[SDLK_x] = MAC_X_KEY;
  sdlkeymap[SDLK_y] = MAC_Y_KEY;
  sdlkeymap[SDLK_z] = MAC_Z_KEY;
  sdlkeymap[SDLK_F1] = MAC_F1_KEY;
  sdlkeymap[SDLK_F2] = MAC_F2_KEY;
  sdlkeymap[SDLK_F3] = MAC_F3_KEY;
  sdlkeymap[SDLK_F4] = MAC_F4_KEY;
  sdlkeymap[SDLK_F5] = MAC_F5_KEY;
  sdlkeymap[SDLK_F6] = MAC_F6_KEY;
  sdlkeymap[SDLK_F7] = MAC_F7_KEY;
  sdlkeymap[SDLK_F8] = MAC_F8_KEY;
  sdlkeymap[SDLK_F9] = MAC_F9_KEY;
  sdlkeymap[SDLK_F10] = MAC_F10_KEY;
  sdlkeymap[SDLK_F11] = MAC_F11_KEY;
  sdlkeymap[SDLK_F12] = MAC_F12_KEY;
  sdlkeymap[SDLK_RETURN] = MAC_RETURN_KEY;
  sdlkeymap[SDLK_KP_ENTER] = MAC_ENTER_KEY;
  sdlkeymap[SDLK_TAB] = MAC_TAB_KEY;
  sdlkeymap[SDLK_SPACE] = MAC_SPACE_KEY;
  sdlkeymap[SDLK_BACKSPACE] = MAC_DELETE_KEY;
  sdlkeymap[SDLK_ESCAPE] = MAC_ESCAPE_KEY;
  sdlkeymap[SDLK_LCTRL] = MAC_CONTROL_KEY;
  sdlkeymap[SDLK_RCTRL] = MAC_CONTROL_KEY;
  sdlkeymap[SDLK_LSHIFT] = MAC_SHIFT_KEY;
  sdlkeymap[SDLK_RSHIFT] = MAC_SHIFT_KEY;
  sdlkeymap[SDLK_CAPSLOCK] = MAC_CAPS_LOCK_KEY;
  sdlkeymap[SDLK_LALT] = MAC_OPTION_KEY;
  sdlkeymap[SDLK_RALT] = MAC_OPTION_KEY;
  sdlkeymap[SDLK_PAGEUP] = MAC_PAGE_UP_KEY;
  sdlkeymap[SDLK_PAGEDOWN] = MAC_PAGE_DOWN_KEY;
  sdlkeymap[SDLK_INSERT] = MAC_INSERT_KEY;
  sdlkeymap[SDLK_DELETE] = MAC_DEL_KEY;
  sdlkeymap[SDLK_HOME] = MAC_HOME_KEY;
  sdlkeymap[SDLK_END] = MAC_END_KEY;
  sdlkeymap[SDLK_LEFTBRACKET] = MAC_LEFT_BRACKET_KEY;
  sdlkeymap[SDLK_RIGHTBRACKET] = MAC_RIGHT_BRACKET_KEY;
  sdlkeymap[SDLK_UP] = MAC_ARROW_UP_KEY;
  sdlkeymap[SDLK_DOWN] = MAC_ARROW_DOWN_KEY;
  sdlkeymap[SDLK_LEFT] = MAC_ARROW_LEFT_KEY;
  sdlkeymap[SDLK_RIGHT] = MAC_ARROW_RIGHT_KEY;

  return;
}

void GetKeys(unsigned long *keys)
{
  /* this is just weird */
  memcpy(keys, ourkeys, sizeof(ourkeys));
}

void DoSDLKey(SDL_Event * event)
{
  int mackey;
  int index;
  int mask;
  int press = (event->type == SDL_KEYDOWN) ? 1 : 0;

  mackey = sdlkeymap[event->key.keysym.sym];

  if(mackey != -1) {
    index = mackey / 8;
    mask = 1 << (mackey % 8);

    if(press) {
      ourkeys[index] |= mask;
    } else {
      ourkeys[index] &= ~mask;
    }
  }

  if(event->key.keysym.unicode && !(event->key.keysym.unicode & 0xFF80)) {
//#if _GAME
    Game::getInstance().HandleKeyDown(event->key.keysym.unicode);
//#endif
#if _VIEWER
    Viewer::getInstance().HandleKeyDown(event->key.keysym.unicode);
#endif
  }
}

void ProcessSDLEvents()
{
  SDL_Event event;

  if(SDL_PollEvent(&event)) {
    do {
      switch (event.type) {
        case SDL_KEYDOWN:
          if(event.key.keysym.sym == SDLK_RETURN &&
             event.key.keysym.mod & KMOD_ALT) {
            SDL_WM_ToggleFullScreen(SDL_GetVideoSurface());
            break;
          }
          if(event.key.keysym.sym == SDLK_g && event.key.keysym.mod & KMOD_CTRL) {
            if(SDL_WM_GrabInput(SDL_GRAB_QUERY) == SDL_GRAB_OFF) {
              SDL_WM_GrabInput(SDL_GRAB_ON);
              SDL_ShowCursor(SDL_DISABLE);
            } else {
              SDL_WM_GrabInput(SDL_GRAB_OFF);
              SDL_ShowCursor(SDL_ENABLE);
            }
            break;
          }
        case SDL_KEYUP:
          DoSDLKey(&event);
          break;
        case SDL_QUIT:
          exit(0);
      }
    } while(SDL_PollEvent(&event));
  }
}

int InitGL(void)
{
  int screenwidth = config.screenwidth;
  int screenheight = config.screenheight;

  //Setup screen
  if(SDL_Init(SDL_INIT_VIDEO) == -1) {
    fprintf(stderr, "SDL Init Video failed: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  atexit(SDL_Quit);

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  if(screenwidth < 640 || screenheight < 480) {
    int fsFlag = 0;
    if(config.fullscreen == 1) {
      fsFlag = SDL_FULLSCREEN;
    }
    if(SDL_SetVideoMode(640, 480, 0, SDL_OPENGL | fsFlag) == NULL) {
      fprintf(stderr, "(OpenGL) SDL SetVideoMode failed: %s\n", SDL_GetError());
      exit(EXIT_FAILURE);
    }
  } else {
    int fsFlag = 0;
    if(config.fullscreen == 1) {
      fsFlag = SDL_FULLSCREEN;
    }
    if(SDL_SetVideoMode
       (screenwidth, screenheight, 0, SDL_OPENGL | fsFlag) == NULL) {
      fprintf(stderr, "(OpenGL) SDL SetVideoMode failed: %s\n", SDL_GetError());
      exit(EXIT_FAILURE);
    }
  }

  const SDL_VideoInfo* info = SDL_GetVideoInfo();

  if(!info) {
    fprintf(stdout, "Video query failed: %s\n",
             SDL_GetError());
  }

  config.bpp = info->vfmt->BitsPerPixel;

  SDL_WM_SetCaption("Black Shades Elite", "Black Shades Elite");

  SDL_EnableUNICODE(1);         /* toggle it to ON */

  if(config.fullscreen) {
    SDL_WM_GrabInput(SDL_GRAB_ON);
    SDL_ShowCursor(0);
  }

  glAlphaFunc(GL_GREATER, 0.01);
  glDepthFunc(GL_LESS);

  glPolygonOffset(-8, 0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  return TRUE;
}

/// far == viewdistance
GLvoid ReSizeGLScene(float fov, float near, float far)
{
  int screenwidth = config.screenwidth;
  int screenheight = config.screenheight;

  if(screenheight == 0) {
    screenheight = 1;
  }

  glViewport(0, 0, screenwidth, screenheight);

  glMatrixMode(GL_PROJECTION);

  glLoadIdentity();

  gluPerspective(fov, (GLfloat) screenwidth / (GLfloat) screenheight, near,
                 far);

  glMatrixMode(GL_MODELVIEW);

  glLoadIdentity();
}

