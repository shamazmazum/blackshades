/*
 * SDL_funcs.h
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

#ifndef _SDL_FUNCS_H_
#define _SDL_FUNCS_H_

#include <GL/gl.h>
#include <GL/glu.h>

#include <SDL/SDL.h>
#include <AL/al.h>
#include <AL/alut.h>

void init_sdlkeymap();

void GetKeys(unsigned long *keys);

void DoSDLKey(SDL_Event *event);

void ProcessSDLEvents();

int InitGL();

GLvoid ReSizeGLScene(float fov, float near, float far);

#endif //_SDL_FUNCS_H_
