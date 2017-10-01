#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
        for (int i = 0; i < 4; i++) {
                fork();
                printf("*\n");
        }
        return 0;
}
