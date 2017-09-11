#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[]) {
  printf("Hello, world!");
  for (int i = 0; i < 16; ++i) {
    fork();
  }
  return 0;
}
