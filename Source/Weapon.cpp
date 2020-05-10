/*
 * Weapon.cpp
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

void Weapon::FindRotationGun(XYZ start, XYZ target)
{
  XYZ temppoint1, temppoint2, tempforward;
  float distance;

  temppoint1 = start;
  temppoint2 = target;
  distance = findDistance(temppoint1, temppoint2);
  gunrotate2 = asin((temppoint1.y - temppoint2.y) / distance) * RAD2DEG;
  temppoint1.y = 0;
  temppoint2.y = 0;
  gunrotate1 =
      acos((temppoint1.z - temppoint2.z) / findDistance(temppoint1,
                                                        temppoint2)) * RAD2DEG;
  if(temppoint1.x > temppoint2.x)
    gunrotate1 = 360 - gunrotate1;
  tempforward = target - start;
  tempforward = DoRotation(tempforward, -90, 0, 0);
  tempforward = DoRotation(tempforward, 0, gunrotate1 - 90, 0);
  tempforward = DoRotation(tempforward, 0, 0, gunrotate2 - 90);
  tempforward.y = 0;
  Normalise(&tempforward);
  gunrotate3 = acos(0 - tempforward.z) * RAD2DEG;
  if(0 > tempforward.x)
    gunrotate3 = 360 - gunrotate3;
}

