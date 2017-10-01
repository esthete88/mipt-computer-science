#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char const *argv[]) {
  pid_t pid = fork();
  if (pid > 0) {
    printf("%d\n", pid);
    int status;
    pid_t wpid = wait(&status);
    printf("wait returns: wpid=%ld; status=%d\n", (long int)wpid, status);
  }
  else {
    sleep(4);
    exit(123);
  }
  return 0;
}
