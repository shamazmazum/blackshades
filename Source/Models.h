/*
 * Models.h
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
#ifndef _MODELS_H_
#define _MODELS_H_

/**> Model Loading <**/
//
// Model Maximums
//
#include "Quaternions.h"
#include <GL/gl.h>
#include "Files.h"
#include "Constants.h"

#define max_textured_triangle		400     // maximum number of texture-filled triangles in a model
#define max_model_vertex			max_textured_triangle*3 // maximum number of vertexs

//
// Model Structures
//

class TexturedTriangle {
public:
  short vertex[3];
  float r, g, b;
};

class Model {
public:
  short vertexNum, TriangleNum;

  XYZ vertex[max_model_vertex];
  XYZ normals[max_textured_triangle];
  TexturedTriangle Triangles[max_textured_triangle];
  GLfloat vArray[max_textured_triangle * 27];

  XYZ boundingspherecenter;
  float boundingsphereradius;
  int LineCheck(XYZ p1, XYZ p2, XYZ * p);
  int LineCheck2(XYZ p1, XYZ p2, XYZ * p, XYZ move, float rotate);
  int LineCheck2(XYZ * p1, XYZ * p2, XYZ * p, XYZ * move, float *rotate);
  int LineCheck3(XYZ p1, XYZ p2, XYZ * p, XYZ move, float rotate, float *d);

  void UpdateVertexArray();
  bool load(Str255 Name);
  void Scale(float xscale, float yscale, float zscale);
  void ScaleNormals(float xscale, float yscale, float zscale);
  void Translate(float xtrans, float ytrans, float ztrans);
  void CalculateNormals();
  void draw();
  void draw(float r, float g, float b);
  void draw(float r, float g, float b, float o);
  void draw(float r, float g, float b, float x, float y, float z);
  void Rotate(float xang, float yang, float zang);
  void MultColor(float howmuch);

  XYZ boundingboxmin, boundingboxmax;
};

#endif
