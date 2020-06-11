//
// Created by raven on 6/11/20.
//
#include "device.h"
#include "log.h"

SDL_bool device_read_info(socket_t device_socket, char *device_name, struct size *size)
{
    unsigned char buf[DEVICE_NAME_FILED_LENGTH + 4];
    // TODO: DEBUG HERE
    int r = net_recv_all(device_socket, buf, sizeof(buf));
    if( r < DEVICE_NAME_FILED_LENGTH + 4)
    {
        LOGE("Could not retrieve device information");
        return SDL_FALSE;
    }
    buf[DEVICE_NAME_FILED_LENGTH -1] = '\0';
    strcpy(device_name,(char *)buf);
    size->width = (buf[DEVICE_NAME_FILED_LENGTH]  << 8| buf[DEVICE_NAME_FILED_LENGTH + 1]);
    size->height = (buf[DEVICE_NAME_FILED_LENGTH + 2] << 8) | buf[DEVICE_NAME_FILED_LENGTH + 3];
}
