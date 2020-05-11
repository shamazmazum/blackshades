/*
 * Window.cpp
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
#include "Window.h"

Window::~Window()
{
  for(unsigned int i = 0; i < children.size(); i++) {
    Window *w = (Window *)children[i];
    delete w;
    w = 0;
  }
  children.clear();
}

void Window::draw()
{
  for(unsigned int i = 0; i < children.size(); i++) {
    children[i]->draw();
  }
}

Window *Window::getWindow(const char *winName)
{
  for(unsigned int i = 0; i < children.size(); i++) {
    if(string(children[i]->getName()) == winName) {
      return children[i];
    }
  }
  return 0;
}

WindowMgr & WindowMgr::getInstance()
{
  MutexLocker obtain_lock(m);
  if(instance.get() == 0)
    instance.reset(new WindowMgr);
  return *instance;
}

std::unique_ptr < WindowMgr > WindowMgr::instance;
Mutex WindowMgr::m;
