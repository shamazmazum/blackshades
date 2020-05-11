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
static SDL_Window *main_window = NULL;
static SDL_Renderer *renderer = NULL;

static unsigned char ourkeys[16];

static int key_sdl2mac (int key)
{
    int res;

    switch (key) {
    case SDLK_1:
        res = MAC_1_KEY;
        break;
    case SDLK_2:
        res = MAC_2_KEY;
        break;
    case SDLK_3:
        res = MAC_3_KEY;
        break;
    case SDLK_4:
        res = MAC_4_KEY;
        break;
    case SDLK_5:
        res = MAC_5_KEY;
        break;
    case SDLK_6:
        res = MAC_6_KEY;
        break;
    case SDLK_7:
        res = MAC_7_KEY;
        break;
    case SDLK_8:
        res = MAC_8_KEY;
        break;
    case SDLK_9:
        res = MAC_9_KEY;
        break;
    case SDLK_0:
        res = MAC_0_KEY;
        break;
    case SDLK_KP_1:
        res = MAC_NUMPAD_1_KEY;
        break;
    case SDLK_KP_2:
        res = MAC_NUMPAD_2_KEY;
        break;
    case SDLK_KP_3:
        res = MAC_NUMPAD_3_KEY;
        break;
    case SDLK_KP_4:
        res = MAC_NUMPAD_4_KEY;
        break;
    case SDLK_KP_5:
        res = MAC_NUMPAD_5_KEY;
        break;
    case SDLK_KP_6:
        res = MAC_NUMPAD_6_KEY;
        break;
    case SDLK_KP_7:
        res = MAC_NUMPAD_7_KEY;
        break;
    case SDLK_KP_8:
        res = MAC_NUMPAD_8_KEY;
        break;
    case SDLK_KP_9:
        res = MAC_NUMPAD_9_KEY;
        break;
    case SDLK_KP_0:
        res = MAC_NUMPAD_0_KEY;
        break;
    case SDLK_a:
        res = MAC_A_KEY;
        break;
    case SDLK_b:
        res = MAC_B_KEY;
        break;
    case SDLK_c:
        res = MAC_C_KEY;
        break;
    case SDLK_d:
        res = MAC_D_KEY;
        break;
    case SDLK_e:
        res = MAC_E_KEY;
        break;
    case SDLK_f:
        res = MAC_F_KEY;
        break;
    case SDLK_g:
        res = MAC_G_KEY;
        break;
    case SDLK_h:
        res = MAC_H_KEY;
        break;
    case SDLK_i:
        res = MAC_I_KEY;
        break;
    case SDLK_j:
        res = MAC_J_KEY;
        break;
    case SDLK_k:
        res = MAC_K_KEY;
        break;
    case SDLK_l:
        res = MAC_L_KEY;
        break;
    case SDLK_m:
        res = MAC_M_KEY;
        break;
    case SDLK_n:
        res = MAC_N_KEY;
        break;
    case SDLK_o:
        res = MAC_O_KEY;
        break;
    case SDLK_p:
        res = MAC_P_KEY;
        break;
    case SDLK_q:
        res = MAC_Q_KEY;
        break;
    case SDLK_r:
        res = MAC_R_KEY;
        break;
    case SDLK_s:
        res = MAC_S_KEY;
        break;
    case SDLK_t:
        res = MAC_T_KEY;
        break;
    case SDLK_u:
        res = MAC_U_KEY;
        break;
    case SDLK_v:
        res = MAC_V_KEY;
        break;
    case SDLK_w:
        res = MAC_W_KEY;
        break;
    case SDLK_x:
        res = MAC_X_KEY;
        break;
    case SDLK_y:
        res = MAC_Y_KEY;
        break;
    case SDLK_z:
        res = MAC_Z_KEY;
        break;
    case SDLK_F1:
        res = MAC_F1_KEY;
        break;
    case SDLK_F2:
        res = MAC_F2_KEY;
        break;
    case SDLK_F3:
        res = MAC_F3_KEY;
        break;
    case SDLK_F4:
        res = MAC_F4_KEY;
        break;
    case SDLK_F5:
        res = MAC_F5_KEY;
        break;
    case SDLK_F6:
        res = MAC_F6_KEY;
        break;
    case SDLK_F7:
        res = MAC_F7_KEY;
        break;
    case SDLK_F8:
        res = MAC_F8_KEY;
        break;
    case SDLK_F9:
        res = MAC_F9_KEY;
        break;
    case SDLK_F10:
        res = MAC_F10_KEY;
        break;
    case SDLK_F11:
        res = MAC_F11_KEY;
        break;
    case SDLK_F12:
        res = MAC_F12_KEY;
        break;
    case SDLK_RETURN:
        res = MAC_RETURN_KEY;
        break;
    case SDLK_KP_ENTER:
        res = MAC_ENTER_KEY;
        break;
    case SDLK_TAB:
        res = MAC_TAB_KEY;
        break;
    case SDLK_SPACE:
        res = MAC_SPACE_KEY;
        break;
    case SDLK_BACKSPACE:
        res = MAC_DELETE_KEY;
        break;
    case SDLK_ESCAPE:
        res = MAC_ESCAPE_KEY;
        break;
    case SDLK_LCTRL:
        res = MAC_CONTROL_KEY;
        break;
    case SDLK_RCTRL:
        res = MAC_CONTROL_KEY;
        break;
    case SDLK_LSHIFT:
        res = MAC_SHIFT_KEY;
        break;
    case SDLK_RSHIFT:
        res = MAC_SHIFT_KEY;
        break;
    case SDLK_CAPSLOCK:
        res = MAC_CAPS_LOCK_KEY;
        break;
    case SDLK_LALT:
        res = MAC_OPTION_KEY;
        break;
    case SDLK_RALT:
        res = MAC_OPTION_KEY;
        break;
    case SDLK_PAGEUP:
        res = MAC_PAGE_UP_KEY;
        break;
    case SDLK_PAGEDOWN:
        res = MAC_PAGE_DOWN_KEY;
        break;
    case SDLK_INSERT:
        res = MAC_INSERT_KEY;
        break;
    case SDLK_DELETE:
        res = MAC_DEL_KEY;
        break;
    case SDLK_HOME:
        res = MAC_HOME_KEY;
        break;
    case SDLK_END:
        res = MAC_END_KEY;
        break;
    case SDLK_LEFTBRACKET:
        res = MAC_LEFT_BRACKET_KEY;
        break;
    case SDLK_RIGHTBRACKET:
        res = MAC_RIGHT_BRACKET_KEY;
        break;
    case SDLK_UP:
        res = MAC_ARROW_UP_KEY;
        break;
    case SDLK_DOWN:
        res = MAC_ARROW_DOWN_KEY;
        break;
    case SDLK_LEFT:
        res = MAC_ARROW_LEFT_KEY;
        break;
    case SDLK_RIGHT:
        res = MAC_ARROW_RIGHT_KEY;
        break;
    default:
        res = -1;
        break;
    }

    return res;
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

  mackey = key_sdl2mac (event->key.keysym.sym);
  if(mackey != -1) {
    index = mackey / 8;
    mask = 1 << (mackey % 8);

    if(press) {
      ourkeys[index] |= mask;
    } else {
      ourkeys[index] &= ~mask;
    }
  }

//   if(event->key.keysym.unicode && !(event->key.keysym.unicode & 0xFF80)) {
// //#if _GAME
//     Game::getInstance().HandleKeyDown(event->key.keysym.unicode);
// //#endif
// #if _VIEWER
//     Viewer::getInstance().HandleKeyDown(event->key.keysym.unicode);
// #endif
//   }
}

void ProcessSDLEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
            // if(event.key.keysym.sym == SDLK_RETURN &&
            //    event.key.keysym.mod & KMOD_ALT) {
            //   SDL_WM_ToggleFullScreen(SDL_GetVideoSurface());
            //   break;
            // }
            if(event.key.keysym.sym == SDLK_g && event.key.keysym.mod & KMOD_CTRL) {
                if(SDL_GetRelativeMouseMode () == SDL_FALSE) {
                    SDL_SetRelativeMouseMode(SDL_TRUE);
                } else {
                    SDL_SetRelativeMouseMode(SDL_FALSE);
                }
            }
            /* Fall through */
        case SDL_KEYUP:
            DoSDLKey(&event);
            break;
        case SDL_QUIT:
            exit(0);
        }
    }
}

static void Cleanup ()
{
    if (main_window != NULL)
        SDL_DestroyWindow (main_window);
    if (renderer != NULL)
        SDL_DestroyRenderer (renderer);

    SDL_Quit();
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

  atexit(Cleanup);

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  Uint32 flags = SDL_WINDOW_OPENGL;
  flags |= (config.fullscreen)? SDL_WINDOW_FULLSCREEN: 0;

  main_window = SDL_CreateWindow ("Black Shades Elite",
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  screenwidth,
                                  screenheight,
                                  flags);
  if (main_window == NULL) {
      fprintf(stderr, "(OpenGL) SDL_CreateWindow failed: %s\n", SDL_GetError());
      exit(EXIT_FAILURE);
  }
  /* XXX */
  config.bpp = 32;

  renderer = SDL_CreateRenderer (main_window, -1, 0);
  if (renderer == NULL) {
      fprintf(stderr, "(OpenGL) SDL_CreateRenderer failed: %s\n", SDL_GetError());
      exit(EXIT_FAILURE);
  }

  if(config.fullscreen) {
      SDL_SetRelativeMouseMode (SDL_TRUE);
      SDL_GetWindowSize (main_window, &config.screenwidth, &config.screenheight);
  }

  SDL_GL_SetSwapInterval (1);
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

void SwapBuffers ()
{
    SDL_GL_SwapWindow (main_window);
}
