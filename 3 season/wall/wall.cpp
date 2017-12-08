#include <iostream>
#include <thread>
#include <cstdio>
#include <mutex>
#include <unistd.h>
#include <ctime>

struct Args {
  int N, M, PMIN, PMAX, SMIN, SMAX, qtime; // N - width, M - height
  int ind, row, column; // ind - indication of blocking; row, column - coordinates of the brick to be built
  char** wall;
};

void buildWall(Args&, int); // int num - number of the worker: 0 for father (even rows)
void printWall(Args&); // and 1 for son (odd rows)

int main(int argc, char const *argv[]) {
  Args args;
  std::cout << "Enter width and height of the wall to build" << std::endl;
  std::cin >> args.N >> args.M;
  std::cout << "Enter minimum and maximum times of placing a brick for father in ms" << std::endl;
  std::cin >> args.PMIN >> args.PMAX;
  std::cout << "Enter minimum and maximum times of placing a brick for son in ms" << std::endl;
  std::cin >> args.SMIN >> args.SMAX;

  args.qtime = (args.SMIN + args.PMIN) / 2; // frequency of printing the wall
  args.ind = 0;
  srand(time(NULL));

  args.wall = new char* [args.M];
  for (int i = 0; i < args.M; ++i) {
    args.wall[i] = new char [2*args.N];
    for (int j = 0; j < 2*args.N; ++j) {
      args.wall[i][j] = ' ';
    }
  }
  /* Wall example
    [][][][]
    ][][][][
    [][][][]
  */
  std::thread father(buildWall, std::ref(args), 0), son(buildWall, std::ref(args), 1);
  std::thread print(printWall, std::ref(args));
  if (errno) {
          perror("Thread cannot be created");
          exit(EXIT_FAILURE);
  }

  father.join();
  son.join();
  print.join();
  return 0;
}

void printWall(Args &args) {
  do {
    usleep(1000 * args.qtime);
    for (int i = args.M - 1; i >= 0; --i) {
      for (int j = 0; j < 2*args.N; ++j) {
        std::cout << args.wall[i][j];
      }
      std::cout << std::endl;
    }
  } while(args.wall[args.M-1][2*args.N-1] == ' ');
}

void buildWall(Args &args, int num) {
  int buildTime;
  for (int i = num; i < args.M; i += 2) {
    for (int j = 0; j < args.N + num; ++j) {
      if (num == 0) {
        buildTime = rand() % (args.PMAX - args.PMIN) + args.PMIN;
        if (i == 0) {
          goto _mark;
        } else if (args.wall[i-1][2*j + 1] != ' ') {
          goto _mark;
        } else {
          args.ind = 1;
          args.row = i-1;
          args.column = j+1;
          while(args.ind) continue;
          goto _mark;
        }
      _mark:
        usleep(1000 * buildTime);
        args.wall[i][2*j] = '[';
        args.wall[i][2*j+1] = ']';
        if (args.ind && args.row == i && args.column == j) args.ind = 0;
      } else {
        buildTime = rand() % (args.SMAX - args.SMIN) + args.SMIN;
        _begins:
          if (args.wall[i-1][2*j] != ' ') {
            usleep(1000 * buildTime);
            if (j == 0) args.wall[i][0] = ']';
            else if (j == args.N) args.wall[i][2*args.N-1] = '[';
            else {
              args.wall[i][2*j-1] = '[';
              args.wall[i][2*j] = ']';
            }
            if (args.ind && args.row == i && args.column == j) args.ind = 0;
          } else {
            args.ind = 1;
            args.row = i-1;
            args.column = j;
            while(args.ind) continue;
            goto _begins;
          }
      }
    }
  }
}
