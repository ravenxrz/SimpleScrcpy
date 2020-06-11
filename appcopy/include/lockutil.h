/**
 * @file lockutil.h
 * @author raven (zhang.xingrui@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-06-11
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef LOCKUTIL_H
#define LOCKUTIL_H

typedef struct SDL_mutex SDL_mutex;
typedef struct SDL_cond SDL_cond;

void mutex_lock(SDL_mutex *mutex);
void mutex_unlock(SDL_mutex *mutex);

void cond_wait(SDL_cond *cond, SDL_mutex *mutex);
void cond_signal(SDL_cond *cond);

#endif // LOCKUTIL_H