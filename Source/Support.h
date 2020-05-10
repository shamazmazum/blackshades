/*
 * Support.h
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
#ifndef SUPPORT_H
#define SUPPORT_H

#include <stdio.h>
#include <unistd.h>

#include <AL/al.h>
#include <AL/alut.h>

#define STUB_FUNCTION fprintf(stderr,"STUB: %s at " __FILE__ ", line %d, thread %d\n",__FUNCTION__,__LINE__,getpid())

#define fsFromStart SEEK_SET

typedef unsigned char *Str255;
typedef int OSErr;
typedef short int SInt16;

typedef bool Boolean;
#define TRUE true
#define FALSE false

typedef struct UnsignedWide {
  unsigned int lo;
  unsigned int hi;
} UnsignedWide;

typedef struct Point {
  int h;
  int v;
} Point;

#define SetFPos(fildes, whence, offset) lseek(fildes, offset, whence)
#define FSClose(fildes) close(fildes)

int Random();
void Microseconds(UnsignedWide * microTickCount);
void GetMouse(Point * p);
void GetMouseRel(Point * p);
void GetKeys(unsigned long *keys);
//int Button(void);
int ButtonClick(int button);
#ifdef NOOGG
void alutLoadWAVFile_CFH(char *filename, ALenum * format, void **wave,
                         unsigned int *size, ALsizei * freq);
void alutUnloadWAV_CFH(ALenum format, void *wave, unsigned int size,
                       ALsizei freq);
#define alutLoadWAVFile alutLoadWAVFile_CFH
#define alutUnloadWAV alutUnloadWAV_CFH
#else
void LoadOGG_CFH(char *filename, ALenum * format, void **wave,
                 unsigned int *size, ALsizei * freq);
void FreeOGG(ALenum format, void *wave, unsigned int size, ALsizei freq);
#endif


FILE *cfh_fopen(const char *filename, const char *mode);

#endif
