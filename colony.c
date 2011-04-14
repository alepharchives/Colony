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

#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "colony.h"

struct thread_pool {
    pthread_t *threads;
    
    pthread_mutex_t mut;
    pthread_cond_t cnd;
    
    void (*volatile func)(void *);
    volatile void *data;
    
    volatile bool quit;

    unsigned short n;
};

void *pool_thread(void *arg) {
    thread_pool *pool = arg;
    
    void (*func)(void *);
    void *data;
    
    pthread_mutex_lock(&pool->mut);
    while (!pool->quit) {
        if (pool->func == NULL) {
            pthread_cond_wait(&pool->cnd, &pool->mut);
        }
        
        void (*func)(void *) = pool->func;
        void *data = (void *)pool->data;
        
        pool->func = NULL;
        
        if (func != NULL) {
            pthread_cond_broadcast(&pool->cnd);
            pthread_mutex_unlock(&pool->mut);
            func(data);
            pthread_mutex_lock(&pool->mut);
        }
    }
    pthread_mutex_unlock(&pool->mut);
    return NULL;
}

thread_pool *thread_pool_new(unsigned short n) {
    thread_pool *pool = malloc(sizeof(thread_pool));
    
    pool->n = n;
    
    pthread_mutex_init(&pool->mut, NULL);
    pthread_cond_init(&pool->cnd, NULL);
    
    pool->func = NULL;
    pool->data = NULL;
    pool->quit = false;
    
    pool->threads = malloc(sizeof(pthread_t) * n);
    int i;
    for (i = 0; i < n; ++i) {
        pthread_create(&pool->threads[i], NULL, pool_thread, pool);
    }
    
    return pool;
}

void thread_pool_run(thread_pool *pool, void (*func)(void *), void *data) {
    pthread_mutex_lock(&pool->mut);
    
    /* wait until the job can be posted */
    while (pool->func != NULL) {
        pthread_cond_wait(&pool->cnd, &pool->mut);
    }
    
    pool->func = func;
    pool->data = data;
    
    pthread_cond_broadcast(&pool->cnd);
    
    pthread_mutex_unlock(&pool->mut);
}

void thread_pool_free(thread_pool *pool) {
    pthread_mutex_lock(&pool->mut);
    
    /* this ensures that all current jobs have started */
    while (pool->func != NULL) {
        pthread_cond_wait(&pool->cnd, &pool->mut);
    }
    
    pool->quit = true;
    
    pthread_cond_broadcast(&pool->cnd);
    pthread_mutex_unlock(&pool->mut);
    
    /* wait for all current jobs to finish */
    int i;
    for (i = 0; i < pool->n; ++i) {
        pthread_join(pool->threads[i], NULL);
    }
    
    pthread_mutex_destroy(&pool->mut);
    pthread_cond_destroy(&pool->cnd);
    
    free(pool->threads);
    free(pool);
}
