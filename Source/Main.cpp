/*
 * Main.cpp
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

#include "Game.h"


int main(int argc, char *argv[])
{
  InitGL();

  ///viewer?
  if(argc > 1 && strcmp(argv[1], "--viewer") == 0) {
#if 0
    Viewer *viewer;
    viewer = &Viewer::getInstance();

    viewer->setup(); //and init GUI

    viewer->EventLoop();

    viewer->Exit();
#endif
  } else {
    Game* game;
    game = &Game::getInstance();
    game->state = INIT;

    game->InitGame();

    game->InitGUI();

    game->EventLoop();

    game->Dispose();
  }

}
