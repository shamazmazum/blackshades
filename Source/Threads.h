/*
 * Threads.h
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
#ifndef _THREADS_H_
#define _THREADS_H_

#include <pthread.h>
#include <memory>

class Mutex {
public:
  Mutex() {
    pthread_mutex_init(&m, 0);
  } void lock() {
    pthread_mutex_lock(&m);
  }

  void unlock() {
    pthread_mutex_unlock(&m);
  }

private:
  pthread_mutex_t m;
};

class MutexLocker {
public:
  MutexLocker(Mutex & pm):m(pm) {
    m.lock();
  } ~MutexLocker() {
    m.unlock();
  }
private:
  Mutex & m;
};

#endif
