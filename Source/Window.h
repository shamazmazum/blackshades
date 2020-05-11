/*
 * Window.h
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
#ifndef _GUI_WINDOW_H_
#define _GUI_WINDOW_H_

#include "Quaternions.h"
#include <GL/gl.h>

#include <string>
#include <vector>

#include "Threads.h"

class Window {
public:
  Window() {
  };
  virtual ~Window();

  const char *getName() {
    return name.c_str();
  }
  void attach(Window *window) {
    children.push_back(window);
    //printf("'%s' children: %u\n", name.c_str(), children.size());
  }
  Window *getWindow(const char *winName);
  void setName(const char *winName) {
    name = winName;
  }

  virtual void draw();

protected:
  std::vector <Window*>children;

private:
  string name;
};

class WindowMgr {
public:
  static WindowMgr & getInstance();
  ~WindowMgr() {
    delete root;
  }
  Window *getRoot() {
    return root;
  }
  void draw() {
    root->draw();
  }

protected:
  WindowMgr() {
    root = new Window();
  }

private:
  static std::unique_ptr < WindowMgr > instance;
  static Mutex m;
  Window *root;
};

#endif
