#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
        int fd[2];
        pipe(fd);
        pid_t pid = fork();
        if (pid == -1) {
                perror("fork failed");
        } else if (pid > 0) {
                write(fd[1], "Hello from, child", 13);
                exit(0);
        } else {
                char buf[512];
                int bytes = read(fd[0], buf, sizeof buf);
                printf("Received %d bytes from parent: '%s'\n", bytes, buf);
        }
}
