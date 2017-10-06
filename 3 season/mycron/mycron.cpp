#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

struct Task {
        int hh, mm, ss;
        string line;
};

int checkSymbol(char c) {
        if (c >= 48 && c <= 57) { // number
                return 0;
        } else if (c == 42) { // asterisk
                return 1;
        } else { // NaN
                return -1;
        }
}

int main(int argc, char const *argv[]) {
        ifstream file("mycrontab.txt");
        vector<string> vargs(3);
        vector<const char*> args;
        vargs[0] = "ls";
        vargs[1] = "-l";
        vargs[2] = "/";
        for (auto const &q : vargs) {
                args.push_back(q.c_str());
        }
        args.push_back(NULL);
        // printf("%s\n", (char**)&args[0]);
        execvp(args[0], (char**)&args[0]);
        return 0;
}
