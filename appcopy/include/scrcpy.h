/**
 * @file scrcpy.h
 * @author raven (zhang.xingrui@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-06-11
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef SCRCPY_H
#define SCRCPY_H

#include <SDL2/SDL_stdinc.h>

SDL_bool scrcpy(Uint16 local_port, Uint16 max_size, Uint32 bit_rate);

#endif // SCRCPY_H