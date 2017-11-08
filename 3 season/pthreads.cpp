#include <unistd.h>
#include <stdio.h>
#include <thread>
#include <iostream>
#include <mutex>

const int N = 1000000;

struct args {
        int x, y;
        std::mutex mutex;
};

void func(args &p) {
        int q = 0;
        for (int i = 0; i < N; ++i) {
                q++;
        }
        p.mutex.lock();
        p.x += q;
        p.mutex.unlock();
}

int main() {
        std::cout << "About to launch a new thread" << std::endl;
        args args0;
        args0.x = 0;
        args0.y = 0;
        int q = 0;
        std::thread thr(func, std::ref(args0));
        for (int i = 0; i < N; ++i) {
                q++;
        }
        args0.mutex.lock();
        args0.x += q;
        args0.mutex.unlock();
        thr.join();
        std::cout << args0.x << std::endl;
}
