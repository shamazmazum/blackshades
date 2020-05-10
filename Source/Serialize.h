/*
 * Serialize.h
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
#ifndef SERIALIZE_H
#define SERIALIZE_H

/* these all read big-endian data */

int ReadBool(int fd, int count, bool * b);
int ReadShort(int fd, int count, short *s);
int ReadInt(int fd, int count, int *s, bool rf = false);
int ReadFloat(int fd, int count, float *f);
int ReadXYZ(int fd, int count, XYZ * xyz);
int ReadTexturedTriangle(int fd, int count, TexturedTriangle * tt);

int WriteBool(int fd, int count, bool * b);
int WriteShort(int fd, int count, short *s);
int WriteInt(int fd, int count, int *s, bool rf = false);
int WriteFloat(int fd, int count, float *f);
int WriteXYZ(int fd, int count, XYZ * xyz);
int WriteTexturedTriangle(int fd, int count, TexturedTriangle * tt);

#endif
