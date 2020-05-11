/*
 * AGL_DSp.h
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

#pragma once

#ifndef _AGL_DSP_H_
#define _AGL_DSP_H_


/**> HEADER FILES <**/
#include <stdlib.h>             // ANSI C cross platform headers
#include <stdio.h>
#include <DrawSprocket.h>       // DrawSprocket
#include <agl.h>                // Apple's OpenGL
#include <tk.h>                 // Used for loading images


/**> CONSTANT DECLARATIONS <**/
#define	kMoveToFront		kFirstWindowOfClass

// Screen Dimensions
#define SCREEN_WIDTH		640
#define SCREEN_HEIGHT		480


/**> GLOBAL VARIABLES <**/
extern DSpContextAttributes gDSpContextAttributes;      // Global DrawSprocket context attributes
extern DSpContextReference gDSpContext; // The global DrawSprocket context
extern AGLContext gOpenGLContext;       // The global OpenGL (AGL) context
                                                                                                                // Note: These are actually defined in AGL_DSp.cpp


/**> FUNCTION PROTOTYPES <**/
void ToolboxInit(void);
Boolean HasAppearance(void);
CGrafPtr SetupScreen(int width, int height);
void CreateWindow(CGrafPtr & theFrontBuffer, int width, int height);
void ShutdownScreen(CGrafPtr theFrontBuffer);
AGLContext SetupAGL(AGLDrawable window);
void CleanupAGL(AGLContext context);


#endif
