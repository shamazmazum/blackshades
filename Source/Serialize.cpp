/*
 * Serialize.cpp
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
#include <unistd.h>

#include "Models.h"
#include "Quaternions.h"
#include "Serialize.h"

/* these all read big-endian data */

int ReadBool(int fd, int count, bool * b)
{
  while(count--) {
    unsigned char buf[1];

    if(read(fd, buf, 1) != 1) {
      STUB_FUNCTION;
    }

    *b = (buf[0] != 0) ? true : false;

#if DEBUG2
    printf("ReadBool(): %d: bool: %u\n", count, *b);
#endif
    b++;
  }

  return 1;
}

int ReadShort(int fd, int count, short *s)
{
  while(count--) {
    unsigned char buf[2];

    if(read(fd, buf, 2) != 2) {
      STUB_FUNCTION;
    }

    *s = (short)((buf[0] << 8) | buf[1]);

#if DEBUG2
    printf("ReadShort(): %d: short: %d\n", count, *s);
#endif
    s++;
  }

  return 1;
}

int ReadInt(int fd, int count, int *s, bool rf)
{
  while(count--) {
    unsigned char buf[4];

    if(read(fd, buf, 4) != 4) {
      STUB_FUNCTION;
    }

    *s = (int)((buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3]);

#if DEBUG2
    if(!rf)
      printf("ReadInt(): %d: int: %d\n", count, *s);
#endif
    s++;
  }

  return 1;
}

union intfloat {
  int i;
  float f;
} intfloat;

int ReadFloat(int fd, int count, float *f)
{
  union intfloat infl;

  while(count--) {
    ReadInt(fd, 1, &(infl.i), true);

    *f = infl.f;

#if DEBUG2
    printf("ReadFloat(): %d: float: %f\n", count, *f);
#endif
    f++;
  }

  return 1;
}

int ReadXYZ(int fd, int count, XYZ * xyz)
{
  while(count--) {
    ReadFloat(fd, 1, &(xyz->x));
    ReadFloat(fd, 1, &(xyz->y));
    ReadFloat(fd, 1, &(xyz->z));

#if DEBUG2
    printf("ReadXYZ(): %d: \n", count);
#endif
    xyz++;
  }

  return 1;
}

int ReadTexturedTriangle(int fd, int count, TexturedTriangle * tt)
{
  while(count--) {
#if DEBUG2
    printf("ReadTexturedTriangle(): %d: \n", count);
#endif
    short pad;
    ReadShort(fd, 3, tt->vertex);
    ReadShort(fd, 1, &pad);     /* crud */
    ReadFloat(fd, 1, &(tt->r));
    ReadFloat(fd, 1, &(tt->g));
    ReadFloat(fd, 1, &(tt->b));

#if DEBUG2
    printf("ReadTexturedTriangle(): %d: done.\n", count);
#endif
    tt++;
  }

  return count;
}

int WriteBool(int fd, int count, bool * b)
{
  while(count--) {
    unsigned char buf[1];
    b[0] = (unsigned char)*b;

    if(write(fd, buf, 1) != 1) {
      STUB_FUNCTION;
    }

#if DEBUG2
    printf("WriteBool(): %d: bool: %u\n", count, *b);
#endif
    b++;
  }

  return 1;
}

int WriteShort(int fd, int count, short *s)
{
  while(count--) {
    unsigned char buf[2];
    buf[0] = s[0];
    buf[1] = s[1];

    if(read(fd, buf, 2) != 2) {
      STUB_FUNCTION;
    }

#if DEBUG2
    printf("WriteShort(): %d: short: %d\n", count, *s);
#endif
    s++;
  }

  return 1;
}

int WriteInt(int fd, int count, int *s, bool rf)
{
  while(count--) {
    unsigned char buf[4];
    buf[0] = s[0];
    buf[1] = s[1];
    buf[2] = s[2];
    buf[3] = s[3];

    if(read(fd, buf, 4) != 4) {
      STUB_FUNCTION;
    }

#if DEBUG2
    if(!rf)
      printf("WriteInt(): %d: int: %d\n", count, *s);
#endif
    s++;
  }

  return 1;
}

int WriteFloat(int fd, int count, float *f)
{
  union intfloat infl;

  while(count--) {
    infl.f = *f;
    WriteInt(fd, 1, &(infl.i), true);

#if DEBUG2
    printf("WriteFloat(): %d: float: %f\n", count, *f);
#endif
    f++;
  }

  return 1;
}

int WriteXYZ(int fd, int count, XYZ * xyz)
{
  while(count--) {
    WriteFloat(fd, 1, &(xyz->x));
    WriteFloat(fd, 1, &(xyz->y));
    WriteFloat(fd, 1, &(xyz->z));

#if DEBUG2
    printf("WriteXYZ(): %d: \n", count);
#endif
    xyz++;
  }

  return 1;
}

int WriteTexturedTriangle(int fd, int count, TexturedTriangle * tt)
{
  while(count--) {
#if DEBUG2
    printf("WriteTexturedTriangle(): %d: \n", count);
#endif
    short pad = 0;
    WriteShort(fd, 3, tt->vertex);
    WriteShort(fd, 1, &pad);     /* crud */
    WriteFloat(fd, 1, &(tt->r));
    WriteFloat(fd, 1, &(tt->g));
    WriteFloat(fd, 1, &(tt->b));

#if DEBUG2
    printf("WriteTexturedTriangle(): %d: done.\n", count);
#endif
    tt++;
  }

  return count;
}

