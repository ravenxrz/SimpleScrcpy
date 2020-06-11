#include "command.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
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
    return kill(pid, SIGTERM) != -1;
}
SDL_bool cmd_simple_wait(process_t pid, exit_code_t *exit_code)
{
    int status;
    int code;
    if(waitpid(pid,&status,0) == -1 || !WIFEXITED(status))
    {
        // 无法等待,或者子进程退出异常
        code = -1;
    }
    else
    {
        code = WEXITSTATUS(status);
    }
    if(exit_code)
    {
        *exit_code = code;
    }
    return !code;
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
    const char *const adb_cmd[] = {"reverse", remote, local};
    return adb_execute(adb_cmd, ARRAY_LEN(adb_cmd));
}
process_t adb_reverse_remove(const char *device_socket_name)
{
    char remote[108 + 14 + 1];  //localabstract:NAME
    snprintf(remote, sizeof(remote), "localabstract:%s", device_socket_name);
    const char *const adb_cmd[] = {"reverse", "--remove", remote};
    return adb_execute(adb_cmd, ARRAY_LEN(adb_cmd));
}
process_t adb_push(const char *local, const char *remote)
{
    const char *const adb_cmd[] = {"push", local, remote};
    return adb_execute(adb_cmd, ARRAY_LEN(adb_cmd));
}
process_t adb_remove_path(const char *path)
{
    const char *const adb_cmd[] = {"shell", "rm", path};
    return adb_execute(adb_cmd, ARRAY_LEN(adb_cmd));
}


SDL_bool process_check_success(process_t proc, const char *name)
{
    if(proc == PROCESS_NONE)
    {
        LOGE("Cound not execute \"%s\"", name);
        return SDL_FALSE;
    }
    exit_code_t exit_code;
    if(!cmd_simple_wait(proc, &exit_code))
    {
        if(exit_code != NO_EXIT_CODE)
        {
            LOGE("\"%s\" returned with value %" PRIexitcode, name, exit_code);
        }
        else
        {
            LOGE("\"%s\" exited unexpectedly", name);
        }
        return SDL_FALSE;
    }

    LOGD("\"%s\" run success", name);
    return SDL_TRUE;
}