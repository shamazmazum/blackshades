/*
 * TGALoader.cpp
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
#include "TGALoader.h"
#include "Support.h"

TGAImageRec *LoadTGA(const char *filename)
{
  GLubyte TGAheader[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };       // Uncompressed TGA Header
  GLubyte TGAcompare[12];       // Used To Compare TGA Header
  GLubyte header[6];            // First 6 Useful Bytes From The Header
  GLuint bytesPerPixel;         // Holds Number Of Bytes Per Pixel Used In The TGA File
  GLuint imageSize;             // Used To Store The Image Size When Setting Aside Ram
  GLuint temp;                  // Temporary Variable
  TGAImageRec *texture;
  FILE *file;

  // Open The TGA File
  file = cfh_fopen(filename, "rb");

  if((file == NULL) ||          // Does File Even Exist?
     (fread(TGAcompare, 1, sizeof (TGAcompare), file) != sizeof (TGAcompare)) ||        // Are There 12 Bytes To Read?
     (memcmp(TGAheader, TGAcompare, sizeof (TGAheader)) != 0) ||        // Does The Header Match What We Want?
     (fread(header, 1, sizeof (header), file) != sizeof (header)))      // If So Read Next 6 Header Bytes
  {
    // If anything failed then close the file and return false
    fclose(file);
    return NULL;
  }
  // Create a new RGBAImageRec
  texture = (TGAImageRec *) malloc(sizeof (TGAImageRec));

  // Determine the TGA width (highbyte*256+lowbyte) and height (highbyte*256+lowbyte)
  texture->sizeX = header[1] * 256 + header[0];
  texture->sizeY = header[3] * 256 + header[2];

  // Make sure the height, width, and bit depth are valid
  if((texture->sizeX <= 0) || (texture->sizeY <= 0)
     || ((header[4] != 24) && (header[4] != 32))) {
    // If anything failed then close the file, free up memory for the image, and return NULL
    fclose(file);
    free(texture);
    return NULL;
  }
  // Grab The TGA's Bits Per Pixel (24 or 32)
  texture->bpp = header[4];
  bytesPerPixel = texture->bpp / 8;     // Divide By 8 To Get The Bytes Per Pixel

  // Calculate The Memory Required For The TGA Data
  imageSize = texture->sizeX * texture->sizeY * bytesPerPixel;

  // Reserve Memory To Hold The TGA Data
  texture->data = (GLubyte *) malloc(imageSize);

  // Make sure the right amount of memory was allocated
  if((texture->data == NULL)
     || (fread(texture->data, 1, imageSize, file) != imageSize)) {
    // Free up the image data if there was any
    if(texture->data != NULL)
      free(texture->data);

    // If anything failed then close the file, free up memory for the image, and return NULL
    fclose(file);
    free(texture);
    return NULL;
  }
  // Loop Through The Image Data
  for(GLuint i = 0; i < (imageSize); i += bytesPerPixel) {
    // Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
    temp = texture->data[i];    // Temporarily Store The Value At Image Data 'i'
    texture->data[i] = texture->data[i + 2];    // Set The 1st Byte To The Value Of The 3rd Byte
    texture->data[i + 2] = temp;        // Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
  }

  // Close The File
  fclose(file);

  return texture;
}

void WriteTGA(const char *filename, TGAImageRec *image, unsigned int size)
{
  //TGA header is 18 bytes
  // True color image with no color map, uncompressed = 2 (3rd byte)
  GLubyte TGAheader[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  GLubyte header[6];     // First 6 Useful Bytes From The Header
  int bytes_written = 0;
  FILE* file;
  unsigned char width[4];
  unsigned char height[4];

  file = fopen(filename, "w");
  if(!file) {
    perror("WriteTGA");
  }

  //defaults
  memcpy(width, &image->sizeX, 4);
  memcpy(height, &image->sizeY, 4);
  //endian issues
  header[0] = (GLubyte)width[0];
  header[1] = (GLubyte)width[1];
  header[2] = (GLubyte)height[0];
  header[3] = (GLubyte)height[1];
  header[4] = (GLubyte)24; //forced bpp
  header[5] = 0; //no alpha

  //write header
  fwrite(TGAheader, sizeof(TGAheader), 1, file);
  fwrite(header, sizeof(header), 1, file);

  //write data
  while(size != 0) {
    bytes_written = fwrite(image->data, 1, size, file);
    size -= bytes_written;
  }
  fclose(file);
}
