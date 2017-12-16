#include <fstream>
#include <iostream>
#include <cerrno>
#include <limits.h>
#include <cstdlib>
#include <dirent.h>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

int main(int argc, char const *argv[]) {
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    string path;

    dir = opendir(realpath(argv[1], NULL));
    if (!dir) {
        perror("diropen");
        exit(1);
    };

    while ( (entry = readdir(dir)) != NULL) {
        path = string(realpath(argv[1], NULL)) + "/" + string(entry->d_name);
        if (entry->d_name[0] == '.')
            continue;
        stat(path.c_str(), &st);
        cout << entry->d_name << " " << S_ISDIR(st.st_mode) << " " << S_ISREG(st.st_mode) << endl;
    };

    closedir(dir);
    return 0;
}
