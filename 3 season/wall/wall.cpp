#include <iostream>
#include <thread>
#include <cstdio>
#include <mutex>

struct Args {
  int N, M, PMIN, PMAX, SMIN, SMAX;
  std::mutex mutex;
};

int main(int argc, char const *argv[]) {
  Args args;
  std::cout << "Enter width and height of the wall to build" << std::endl;
  std::cin >> args.N >> args.M;
  std::cout << "Enter minimum and maximum times of placing a brick for father in ms" << std::endl;
  std::cin >> args.PMIN >> args.PMAX;
  std::cout << "Enter minimum and maximum times of placing a brick for son in ms" << std::endl;
  std::cin >> args.SMIN >> args.SMAX;

  std::cout << "|____|____|____|" << std::endl;
  std::cout << "|__|____|____|_|" << std::endl;
  std::thread father, son;

  return 0;
}
