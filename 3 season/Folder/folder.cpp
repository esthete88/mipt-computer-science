#include <iostream>
#include <unistd.h>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <sys/stat.h>
#include <sys/types.h>
#include <utime.h>
#include <string>

const int N = 100000;

using namespace std;

int main(int argc, char const *argv[]) {
  ofstream fout;
  mkdir("./pipiska", 0775);
  chdir("./pipiska");
  string S;
  for (int i = 0; i < 100000; ++i) {
    S += "*";
  }
  for (int i = 0; i < N; ++i) {
    string filename = string("samplefile") + to_string(i) + string(".txt");
    fout.open(filename);
    fout << S;
    fout.close();
  }
  return 0;
}
