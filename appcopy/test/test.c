#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char const *argv[])
{
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork");
        return -1;
    }
    if(pid == 0)
    {
        printf("son process start\n");
        const char *argvv[] = {"ls", "-l", NULL};
        if(execlp("ls",(const char *)argvv) == -1)
        {
            perror("run exec");
        }
        printf("son process end\n");
    }

    printf("father process\n");
    return 0;
}
