#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

struct size{
    int width;
    int height;
};

struct size test_stack_memory(void);

int main(int argc, char const *argv[])
{
    struct size size = test_stack_memory();
    printf("main size addr %p\n", &size);
    printf("size width %d, height %d\n", size.width, size.height);
    return 0;
}

struct size test_stack_memory(void)
{
    struct size size =
    {
        .width = 10,
        .height = 20,
    };
    printf("stack addr %p\n", &size);
    return size;
}

int test_execl(void)
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