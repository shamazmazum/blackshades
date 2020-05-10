/*
 * Globals.cpp
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
#include "Globals.h"
#include "Quaternions.h"
#include "Constants.h"
#include "Models.h"
#include "Person.h"
#include "Decals.h"
#include "Font.h"

float sinefluct;
float sinefluctprog;

double multiplier = 0;

unsigned int gSourceID[100];    // hundred source IDs
unsigned int gSampleSet[100];   // hundred sample set ID numbers

float soundscalefactor;
int slomo;

Animation animation[30];
Model skeletonmodels[10];
Model gunmodels[11];
Costume costume[10];
Sprites sprites;
Decals decals;
Camera camera;
Skeleton testskeleton;
Config config;
Environment environment;
Font font;

Fog fog;

bool blood;

float d;
float a1, a2, a3;
float total, denom, mu;
XYZ n, pa1, pa2, pa3;

float u0, u1, u2;
float v0, v1, v2;
float a, b;
float max;
int i, j; //wha?
bool bInter;
float pointv[3];
float p1v[3];
float p2v[3];
float p3v[3];
float normalv[3];

//skeletal animation
XYZ vel;
XYZ midp;
XYZ newpoint1, newpoint2;
float oldlength;
float relaxlength;

//physics
float friction;
int numrepeats;
float groundlevel;
float offset;
XYZ impact;
XYZ overpoint;
XYZ underpoint;
int whichtri;
XYZ normalrotated;
bool groundish;
