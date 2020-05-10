/*
 * Models.cpp
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
#include "Models.h"
#include "Serialize.h"
#include "Globals.h"

extern Config config;

//Functions
void Model::UpdateVertexArray()
{
  int i, offset;
  XYZ *__restrict__ normal;
  TexturedTriangle * __restrict__ tri;
  for(i = 0; i < TriangleNum; i++) {
    tri = &Triangles[i];
    normal = &normals[i];
    offset = i * 27;
    vArray[offset + 0] = vertex[tri->vertex[0]].x;
    vArray[offset + 1] = vertex[tri->vertex[0]].y;
    vArray[offset + 2] = vertex[tri->vertex[0]].z;
    vArray[offset + 3] = normal->x;
    vArray[offset + 4] = normal->y;
    vArray[offset + 5] = normal->z;
    vArray[offset + 6] = tri->r;
    vArray[offset + 7] = tri->g;
    vArray[offset + 8] = tri->b;

    vArray[offset + 9] = vertex[tri->vertex[1]].x;
    vArray[offset + 10] = vertex[tri->vertex[1]].y;
    vArray[offset + 11] = vertex[tri->vertex[1]].z;
    vArray[offset + 12] = normal->x;
    vArray[offset + 13] = normal->y;
    vArray[offset + 14] = normal->z;
    vArray[offset + 15] = tri->r;
    vArray[offset + 16] = tri->g;
    vArray[offset + 17] = tri->b;

    vArray[offset + 18] = vertex[tri->vertex[2]].x;
    vArray[offset + 19] = vertex[tri->vertex[2]].y;
    vArray[offset + 20] = vertex[tri->vertex[2]].z;
    vArray[offset + 21] = normal->x;
    vArray[offset + 22] = normal->y;
    vArray[offset + 23] = normal->z;
    vArray[offset + 24] = tri->r;
    vArray[offset + 25] = tri->g;
    vArray[offset + 26] = tri->b;
  }

  XYZ average, vert;
  int howmany;
  average = 0;
  howmany = 0;
  boundingboxmin = 20000;
  boundingboxmax = -20000;
  for(int i = 0; i < vertexNum; i++) {
    howmany++;
    vert = vertex[i];
    average = average + vert;
    if(vert.x < boundingboxmin.x)
      boundingboxmin.x = vert.x;
    if(vert.y < boundingboxmin.y)
      boundingboxmin.y = vert.y;
    if(vert.z < boundingboxmin.z)
      boundingboxmin.z = vert.z;
    if(vert.x > boundingboxmax.x)
      boundingboxmax.x = vert.x;
    if(vert.y > boundingboxmax.y)
      boundingboxmax.y = vert.y;
    if(vert.z > boundingboxmax.z)
      boundingboxmax.z = vert.z;
  }
  average = average / howmany;
  boundingspherecenter = average;
  boundingsphereradius = 0;
  for(int i = 0; i < vertexNum; i++) {
    if(findDistancefast(average, vertex[i]) > boundingsphereradius)
      boundingsphereradius = findDistancefast(average, vertex[i]);
  }
  boundingsphereradius = fast_sqrt(boundingsphereradius);
}

bool Model::load(Str255 Name)
{
  short tfile;
  long err;
  Files file;

  tfile = file.OpenFile(Name);
  SetFPos(tfile, fsFromStart, 0);

  // read model config
  err = ReadShort(tfile, 1, &vertexNum);
  err = ReadShort(tfile, 1, &TriangleNum);

  // read the model data
  err = ReadXYZ(tfile, vertexNum, vertex);
  err = ReadTexturedTriangle(tfile, TriangleNum, Triangles);

  FSClose(tfile);

  UpdateVertexArray();

  XYZ average;
  int howmany;
  average = 0;
  howmany = 0;
  for(int i = 0; i < vertexNum; i++) {
    howmany++;
    average = average + vertex[i];
  }
  average = average / howmany;
  boundingspherecenter = average;
  boundingsphereradius = 0;
  for(int i = 0; i < vertexNum; i++) {
    if(findDistancefast(average, vertex[i]) > boundingsphereradius)
      boundingsphereradius = findDistancefast(average, vertex[i]);
  }
  boundingsphereradius = fast_sqrt(boundingsphereradius);

  return 1;
}

void Model::Scale(float xscale, float yscale, float zscale)
{
  int i;
  for(i = 0; i < vertexNum; i++) {
    vertex[i].x *= xscale;
    vertex[i].y *= yscale;
    vertex[i].z *= zscale;
  }
  UpdateVertexArray();
}

void Model::MultColor(float howmuch)
{
  int i;
  for(i = 0; i < TriangleNum; i++) {
    Triangles[i].r *= howmuch;
    Triangles[i].g *= howmuch;
    Triangles[i].b *= howmuch;
  }
  UpdateVertexArray();
}

void Model::ScaleNormals(float xscale, float yscale, float zscale)
{
  int i;
  for(i = 0; i < vertexNum; i++) {
    normals[i].x *= xscale;
    normals[i].y *= yscale;
    normals[i].z *= zscale;
  }
  UpdateVertexArray();
}

void Model::Translate(float xtrans, float ytrans, float ztrans)
{
  int i;
  for(i = 0; i < vertexNum; i++) {
    vertex[i].x += xtrans;
    vertex[i].y += ytrans;
    vertex[i].z += ztrans;
  }
  UpdateVertexArray();
}

void Model::Rotate(float xang, float yang, float zang)
{
  int i;
  for(i = 0; i < vertexNum; i++) {
    vertex[i] = DoRotation(vertex[i], xang, yang, zang);
  }
  UpdateVertexArray();
}


void Model::CalculateNormals()
{
  int i;
  for(i = 0; i < TriangleNum; i++) {
    CrossProduct(vertex[Triangles[i].vertex[1]] -
                 vertex[Triangles[i].vertex[0]],
                 vertex[Triangles[i].vertex[2]] -
                 vertex[Triangles[i].vertex[0]], &normals[i]);
    Normalise(&normals[i]);
  }
  UpdateVertexArray();
}

void Model::draw()
{
  int nocolors = config.nocolors;

  if(!nocolors) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 9 * sizeof (GLfloat), &vArray[0]);
    glNormalPointer(GL_FLOAT, 9 * sizeof (GLfloat), &vArray[3]);
    glColorPointer(3, GL_FLOAT, 9 * sizeof (GLfloat), &vArray[6]);
    glDrawArrays(GL_TRIANGLES, 0, TriangleNum * 3);
  } else {
    glColor4f(0, 0, 0, 1);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 9 * sizeof (GLfloat), &vArray[0]);
    glNormalPointer(GL_FLOAT, 9 * sizeof (GLfloat), &vArray[3]);
    glDrawArrays(GL_TRIANGLES, 0, TriangleNum * 3);
  }
}


void Model::draw(float r, float g, float b)
{
  int nocolors = config.nocolors;

  switch (nocolors) {
    case 0:
      glColor4f(r, g, b, 1);
      break;
    case 1:
      glColor4f(0, 0, 0, 1);
      break;
    case 2:
      glColor4f(1, 0, 0, 1);
      break;
    case 3:
      glColor4f(0, 0, 1, 1);
      break;
  }

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glVertexPointer(3, GL_FLOAT, 9 * sizeof (GLfloat), &vArray[0]);
  glNormalPointer(GL_FLOAT, 9 * sizeof (GLfloat), &vArray[3]);
  glDrawArrays(GL_TRIANGLES, 0, TriangleNum * 3);
}

void Model::draw(float r, float g, float b, float o)
{
  int nocolors = config.nocolors;

  switch (nocolors) {
    case 0:
      glColor4f(r, g, b, o);
      break;
    case 1:
      glColor4f(0, 0, 0, 1);
      break;
    case 2:
      glColor4f(1, 0, 0, 1);
      break;
    case 3:
      glColor4f(0, 0, 1, 1);
      break;
  }

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glVertexPointer(3, GL_FLOAT, 9 * sizeof (GLfloat), &vArray[0]);
  glNormalPointer(GL_FLOAT, 9 * sizeof (GLfloat), &vArray[3]);
  glDrawArrays(GL_TRIANGLES, 0, TriangleNum * 3);
}

void Model::draw(float r, float g, float b, float x, float y, float z)
{
  //TODO: setup nocolors elsewhere?
  switch(config.nocolors) {
    case 0:
      glColor4f(r, g, b, 1);
      break;
    case 1:
      glColor4f(0, 0, 0, 1);
      break;
    case 2:
      glColor4f(1, 0, 0, 1);
      break;
    case 3:
      glColor4f(1, 1, 1, 1);
      break;
    default:
      printf("Model::draw(): color setting failed.\n");
      break;
  }
  glNormal3f(x, y, z);
  glEnableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glVertexPointer(3, GL_FLOAT, 9 * sizeof (GLfloat), &vArray[0]);
  glDrawArrays(GL_TRIANGLES, 0, TriangleNum * 3);
}

///find nearest intersected triangle
int Model::LineCheck(XYZ p1, XYZ p2, XYZ * p)
{
  int j;
  float distance;
  float olddistance = 9999999.0;
  int intersecting = 0;
  int firstintersecting = -1;
  XYZ point;
  if(sphere_line_intersection(p1.x, p1.y, p1.z,
                              p2.x, p2.y, p2.z,
                              boundingspherecenter.x, boundingspherecenter.y,
                              boundingspherecenter.z, boundingsphereradius)) {
    for(j = 0; j < TriangleNum; j++) {
      intersecting =
          (int)LineFacetd(p1, p2, vertex[Triangles[j].vertex[0]],
                          vertex[Triangles[j].vertex[1]],
                          vertex[Triangles[j].vertex[2]], normals[j], &point);
      if(intersecting == 0)
        continue;
      distance =
          (point.x - p1.x) * (point.x - p1.x) + (point.y - p1.y) * (point.y -
                                                                    p1.y) +
          (point.z - p1.z) * (point.z - p1.z);
      if((distance < olddistance || firstintersecting == -1) && intersecting) {
        olddistance = distance;
        firstintersecting = j;
        *p = point;
      }
    }
  }
  return firstintersecting;
}

int Model::LineCheck2(XYZ p1, XYZ p2, XYZ * p, XYZ move, float rotate)
{
  int j;
  float distance;
  float olddistance = 9999999.0;
  int intersecting = 0;
  int firstintersecting = -1;
  XYZ point;
  p1 = p1 - move;
  p2 = p2 - move;
  if(rotate) {
    p1 = DoRotation(p1, 0, -rotate, 0);
    p2 = DoRotation(p2, 0, -rotate, 0);
  }
  if(sphere_line_intersection(p1.x, p1.y, p1.z,
                              p2.x, p2.y, p2.z,
                              boundingspherecenter.x, boundingspherecenter.y,
                              boundingspherecenter.z, boundingsphereradius))
    for(j = 0; j < TriangleNum; j++) {
      intersecting =
          (int)LineFacetd(p1, p2, vertex[Triangles[j].vertex[0]],
                          vertex[Triangles[j].vertex[1]],
                          vertex[Triangles[j].vertex[2]], normals[j], &point);
      if(intersecting == 0)
        continue;
      distance =
          (point.x - p1.x) * (point.x - p1.x) + (point.y - p1.y) * (point.y -
                                                                    p1.y) +
          (point.z - p1.z) * (point.z - p1.z);
      if((distance < olddistance || firstintersecting == -1) && intersecting) {
        olddistance = distance;
        firstintersecting = j;
        *p = point;
      }
    }

  if(rotate)
    *p = DoRotation(*p, 0, rotate, 0);
  *p = *p + move;
  return firstintersecting;
}

int Model::LineCheck2(XYZ * p1, XYZ * p2, XYZ * p, XYZ * move, float *rotate)
{
  int j;
  float distance;
  float olddistance = 9999999.0;
  int intersecting = 0;
  int firstintersecting = -1;
  XYZ point;
  *p1 = *p1 - *move;
  *p2 = *p2 - *move;
  if(*rotate) {
    *p1 = DoRotation(*p1, 0, -*rotate, 0);
  }
  if(*rotate) {
    *p2 = DoRotation(*p2, 0, -*rotate, 0);
  }
  if(sphere_line_intersection(p1->x, p1->y, p1->z,
                              p2->x, p2->y, p2->z,
                              boundingspherecenter.x, boundingspherecenter.y,
                              boundingspherecenter.z, boundingsphereradius))
    for(j = 0; j < TriangleNum; j++) {
      intersecting =
          (int)LineFacetd(p1, p2, &vertex[Triangles[j].vertex[0]],
                          &vertex[Triangles[j].vertex[1]],
                          &vertex[Triangles[j].vertex[2]], &normals[j], &point);
      if(intersecting == 0)
        continue;
      distance =
          (point.x - p1->x) * (point.x - p1->x) + (point.y - p1->y) * (point.y -
                                                                       p1->y) +
          (point.z - p1->z) * (point.z - p1->z);
      if((distance < olddistance || firstintersecting == -1) && intersecting) {
        olddistance = distance;
        firstintersecting = j;
        *p = point;
      }
    }

  if(*rotate)
    *p = DoRotation(*p, 0, *rotate, 0);
  *p = *p + *move;
  return firstintersecting;
}

int Model::LineCheck3(XYZ p1, XYZ p2, XYZ * p, XYZ move, float rotate, float *d)
{
  int j;
  float distance;
  float olddistance = 9999999.0;
  int intersecting = 0;
  int firstintersecting = -1;
  XYZ point;
  p1 = p1 - move;
  p2 = p2 - move;
  p1 = DoRotation(p1, 0, -rotate, 0);
  p2 = DoRotation(p2, 0, -rotate, 0);
  if(sphere_line_intersection(p1.x, p1.y, p1.z,
                              p2.x, p2.y, p2.z,
                              boundingspherecenter.x, boundingspherecenter.y,
                              boundingspherecenter.z, boundingsphereradius))
    for(j = 0; j < TriangleNum; j++) {
      intersecting =
          (int)LineFacetd(p1, p2, vertex[Triangles[j].vertex[0]],
                          vertex[Triangles[j].vertex[1]],
                          vertex[Triangles[j].vertex[2]], normals[j], &point);
      if(intersecting == 0)
        continue;
      distance =
          (point.x - p1.x) * (point.x - p1.x) + (point.y - p1.y) * (point.y -
                                                                    p1.y) +
          (point.z - p1.z) * (point.z - p1.z);
      if((distance < olddistance || firstintersecting == -1) && intersecting) {
        olddistance = distance;
        firstintersecting = j;
        *p = point;
      }
    }
  *d = intersecting;
  *p = DoRotation(*p, 0, rotate, 0);
  *p = *p + move;
  return firstintersecting;
}
