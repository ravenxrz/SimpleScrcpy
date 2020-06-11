/**
 * @file common.h
 * @author raven (zhang.xingrui@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-06-11
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef COMMON_H
#define COMMON_H

#include <SDL2/SDL_stdinc.h>

#define MIN(X, Y) (X) < (Y) ? X : Y
#define MAX(X,Y) (X) > (Y) ? X : Y

struct size{
    Uint16 width;
    Uint16 height;
};

struct point{
    Uint16 x;
    Uint16 y;
};

struct position
{
    struct size screen_size;
    struct point point;
};

#endif // COMMON_H