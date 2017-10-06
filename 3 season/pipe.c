#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
        close(0);
        close(1);
        int fd[2];
        pipe(fd);
        pid_t pid = fork();
        if (pid > 0) {
                close(fd[0]);
                printf("Hi!\n");
                exit(0);
        } else {
                close(fd[1]);
                char buf[512];
                fgets(buf, sizeof buf, stdin);
                printf("%s\n", buf);
        }
        return 0;
}
