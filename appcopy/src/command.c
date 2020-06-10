#include "command.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "log.h"

#define ARRAY_LEN(a) (sizeof(a) / sizeof(a[0]))

static const char *adb_command;

static inline const char *get_adb_command()
{
    if(!adb_command)
    {
        adb_command = getenv("ADB");
        if(!adb_command)
            adb_command = "adb";
    }
    return adb_command;
}

process_t cmd_execute(const char *path, const char *const argv[])
{
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork");
        return -1;
    }
    if(pid == 0)
    {
        // 子进程
        execvp(path, (char * const*)argv);
        // 只有执行失败了,才会执行下面的代码
        perror("exec");
        exit(1);    // 源代码是用的_exit
    }
    return pid;
}
SDL_bool cmd_terminate(process_t pid)
{

}
SDL_bool cmd_simple_wait(process_t pid, exit_code_t *exit_code)
{

}

process_t adb_execute(const char *const adb_cmd[], int len)
{
    const char *cmd[len + 2];
    cmd[0] = get_adb_command();
    memcpy(&cmd[1], adb_cmd, len * sizeof(char *));
    cmd[len + 1] = NULL;
    return cmd_execute(cmd[0], cmd);
}
process_t adb_forward(uint16_t local_port, const char *device_socket_name)
{

}
process_t adb_reverse(const char *device_socket_name, uint16_t local_port)
{
    // 4 -- tcP;
    // 5 -- local_port是16位的,2^16 = 65536(5位)
    // 1 -- '\0'
    char local[4 + 5 + 1];
    // TODO: 暂时未知 
    char remote[108 + 14 + 1];
    sprintf(local, "tcp:%" PRIu16, local_port);
    snprintf(remote, sizeof(remote), "localabstract:%s", device_socket_name);
    const char *const adb_cmd[] = {"forward", local, remote};
    return adb_execute(adb_cmd, ARRAY_LEN(adb_cmd));
}
process_t adb_reverse_remove(const char *device_socket_name)
{

}
process_t adb_push(const char *local, const char *remote)
{
    const char *const adb_cmd[] = {"push", local, remote};
    return adb_execute(adb_cmd, ARRAY_LEN(adb_cmd));
}
process_t adb_remove_path(const char *path)
{

}



