#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]) {
        vector<string> vargs;
        vector<const char*> args;
        for (auto const &q : vargs) {
                args.push_back(q.c_str());
        }
        args.push_back(NULL);
        execvp(args[0], (char**)&args[0]);
        return 0;
}
