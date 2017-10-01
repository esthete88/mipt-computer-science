#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
        int fd[2];
        pipe(fd);
        pid_t pid = fork();
        if (pid > 0) {
                long size = 0;
                char c;
                while(write(fd[1], &c, 1)) {
                        printf("%ld\n", size);
                        size++;
                }
        }
        return 0;
}
