/*
    Copyright (C) 2011 Colony authors,
    
    This file is part of Colony.
    
    Colony is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Colony is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef COLONY_H
#define COLONY_H

struct thread_pool;
typedef struct thread_pool thread_pool;

thread_pool *thread_pool_new(unsigned short);
void thread_pool_run(thread_pool *, void (*)(void *), void *);
void thread_pool_free(thread_pool *);

#endif
