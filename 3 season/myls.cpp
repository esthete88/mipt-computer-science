#include <iostream>
#include <unistd.h>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

int main(int argc, char const *argv[]) {
  bool long_flag = false, recursive_flag = false;
  while (argc > 1 && argv[1][0] == '-') {
    if (argv[1][1] == 'l') {
      long_flag = true;
      argc--;
      argv++;
      cout << 1 << endl;
    } else if (argv[1][1] == 'R') {
      recursive_flag = true;
      argc--;
      argv++;
      cout << 2 << endl;
    }
  }
  string start = argc > 1 ? argv[1] : ".";
  struct stat st;
  int err = stat(start.c_str(), &st);
  if (err != 0) {
    perror(start.c_str());
    return 1;
  }
  return 0;
}
