#include "command.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"

#define ARRAY_LEN(a) (sizeof(a) / sizeof(a[0]))

static const char *adb_command;

static inline const char *get_adb_command() {
    if (!adb_command) {
        adb_command = getenv("ADB");
        if (!adb_command)
            adb_command = "adb";
    }
    return adb_command;
}

process_t adb_execute(const char *serial, const char *const adb_cmd[], int len) {
    const char *cmd[len + 4];
    int i;
    cmd[0] = get_adb_command();
    if (serial) {
        cmd[1] = "-s";
        cmd[2] = serial;
        i = 3;
    } else {
        i = 1;
    }

    memcpy(&cmd[i], adb_cmd, len * sizeof(const char *));
    cmd[len + i] = NULL;
    return cmd_execute(cmd[0], cmd);
}

process_t adb_forward(const char *serial, uint16_t local_port, const char *device_socket_name) {
    char local[4 + 5 + 1]; // tcp:PORT
    char remote[108 + 14 + 1]; // localabstract:NAME
    sprintf(local, "tcp:%" PRIu16, local_port);
    snprintf(remote, sizeof(remote), "localabstract:%s", device_socket_name);
    const char *const adb_cmd[] = {"forward", local, remote};
    return adb_execute(serial, adb_cmd, ARRAY_LEN(adb_cmd));
}

process_t adb_reverse(const char *serial, const char *device_socket_name, uint16_t local_port) {
    char local[4 + 5 + 1]; // tcp:PORT
    char remote[108 + 14 + 1]; // localabstract:NAME
    sprintf(local, "tcp:%" PRIu16, local_port);
    snprintf(remote, sizeof(remote), "localabstract:%s", device_socket_name);
    const char *const adb_cmd[] = {"reverse", remote, local};
    return adb_execute(serial, adb_cmd, ARRAY_LEN(adb_cmd));
}

process_t adb_reverse_remove(const char *serial, const char *device_socket_name) {
    char remote[108 + 14 + 1]; // localabstract:NAME
    snprintf(remote, sizeof(remote), "localabstract:%s", device_socket_name);
    const char *const adb_cmd[] = {"reverse", "--remove", remote};
    return adb_execute(serial, adb_cmd, ARRAY_LEN(adb_cmd));
}

process_t adb_push(const char *serial, const char *local, const char *remote) {
    const char *const adb_cmd[] = {"push", local, remote};
    return adb_execute(serial, adb_cmd, ARRAY_LEN(adb_cmd));
}

process_t adb_remove_path(const char *serial, const char *path) {
    const char *const adb_cmd[] = {"shell", "rm", path};
    return adb_execute(serial, adb_cmd, ARRAY_LEN(adb_cmd));
}

SDL_bool process_check_success(process_t proc, const char *name) {
    if (proc == PROCESS_NONE) {
        LOGE("Could not execute \"%s\"", name);
        return SDL_FALSE;
    }
    exit_code_t exit_code;
    if (!cmd_simple_wait(proc, &exit_code)) {
        if (exit_code != NO_EXIT_CODE) {
            LOGE("\"%s\" returned with value %" PRIexitcode, name, exit_code);
        } else {
            LOGE("\"%s\" exited unexpectedly", name);
        }
        return SDL_FALSE;
    }
    return SDL_TRUE;
}
