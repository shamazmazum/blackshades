/*
 * Camera.cpp 
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

#include <stdlib.h>

#include "Camera.h"

void Camera::Apply()
{
  glTranslatef(-position.x, -position.y, -position.z);
}

void Camera::MouseRotate(float mousesensitivity)
{
  Point mouseloc;
  GetMouseRel(&mouseloc);

  float mouserotation = (mouseloc.h / 1.3888) * mousesensitivity;
  float mouserotation2 = (mouseloc.v / 1.3888) * mousesensitivity;

  if(abs(mouseloc.h) < 400)
    rotation += mouserotation;
  if(abs(mouseloc.v) < 200)
    rotation2 += mouserotation2;
  if(mouseloc.h > 400)
    rotation += mouserotation - (500 / 1.3888 * mousesensitivity);
  if(mouseloc.h < -400)
    rotation += mouserotation + (500 / 1.3888 * mousesensitivity);

  if(mouseloc.v > 200)
    rotation2 += mouserotation2 - (300 / 1.3888 * mousesensitivity);

  if(mouseloc.v < -200)
    rotation2 += mouserotation2 + (300 / 1.3888 * mousesensitivity);

  if(rotation2 > 89) {
    rotation2 = 89;
  }

  if(rotation2 < -89) {
    rotation2 = -89;
  }
}

void Camera::Smooth()
{
  rotation =
        (oldoldrotation +
         ((rotation - oldoldrotation) * .7 +
          (oldrotation - oldoldrotation) * .3));

  rotation2 =
        (oldoldrotation2 +
         ((rotation2 - oldoldrotation2) * .7 +
          (oldrotation2 - oldoldrotation2) * .3));

  if(visrotation < rotation - 7)
    visrotation = rotation - 7;

  if(visrotation > rotation + 7)
    visrotation = rotation + 7;

  if(visrotation2 < rotation2 - 15)
    visrotation2 = rotation2 - 15;

  if(visrotation2 > rotation2 + 15)
    visrotation2 = rotation2 + 15;

  oldoldrotation = oldrotation;
  oldoldrotation2 = oldrotation2;
  oldrotation = rotation;
  oldrotation2 = rotation2;
}

void Camera::Update()
{
  oldposition = position;
  targetoffset = 0;
  targetoffset.z = -5;
}
