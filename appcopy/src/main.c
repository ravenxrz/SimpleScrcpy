#include <stdio.h>
#include "command.h"

int main(int argc, char const *argv[])
{
    process_t pid = adb_push("/home/raven/Projects/phonemirror/server/scrcpy-server.jar", "/data/local/tmp");
    printf("pid %d\n", pid);
    return 0;
}
