#include <iostream>
#include <unistd.h>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <sys/stat.h>
#include <sys/types.h>
#include <utime.h>
#include <cstring>
#include <thread>
#include <dirent.h>
#include <string>
#include <vector>
#include <mutex>
#include <functional>
#include <chrono>

const int int0 = (int)('0'); // ascii of 0
const int int9 = (int)('9'); // ascii of 9
const int BUFFER_SIZE = 8192;

struct Task {
    std::string src, dst;
};

struct Args {
    std::vector<Task> tasks;
    unsigned int taskPos, taskSize;
    std::mutex mutex;
};

void parseArgs(int argc, char const **argv, std::string *path1, std::string *path2, int *tnum);
void copyFile(std::string filename, std::string dirFrom, std::string dirTo);
void copyDir(std::string src, std::string dst);
void makeTasklist(std::string src, std::string dst, std::vector<Task> *tasks);
void thrCopy(Args &args);

int main(int argc, char const *argv[]) {
    // std::ofstream fout("logfile.txt");
    int threadsNumber;
    std::string dirFrom, dirTo;
    parseArgs(argc, argv, &dirFrom, &dirTo, &threadsNumber);
    if (access(dirFrom.c_str(), 0)) {
        perror("Source directory does not exist");
        exit(EXIT_FAILURE);
    }
    if (access(dirTo.c_str(), 0)) {
        std::cerr << "Destination directory does not exist. Creating... " << std::endl;
        errno = 0;
        mkdir(dirTo.c_str(), 0775);
    }
    dirFrom = std::string(realpath(dirFrom.c_str(), NULL));
    dirTo = std::string(realpath(dirTo.c_str(), NULL));
    Args args;
    args.taskPos = 0;
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    makeTasklist(dirFrom, dirTo, &args.tasks);
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    std::chrono::microseconds timeSpan = std::chrono::duration_cast<std::chrono::microseconds>(t2-t1);
    std::cout << "Task list created. It took me " << timeSpan.count() << " microseconds" << std::endl;
    args.taskSize = args.tasks.size();
    std::thread *threads = new std::thread[threadsNumber];
    for (int i = 0; i < threadsNumber; ++i) {
        threads[i] = std::thread(thrCopy, std::ref(args));
        if (errno) {
            perror("Thread cannot be created");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < threadsNumber; ++i) {
        threads[i].join();
        if (errno) {
            perror("Thread cannot be joined");
            exit(EXIT_FAILURE);
        }
    }
    std::chrono::high_resolution_clock::time_point t3 = std::chrono::high_resolution_clock::now();
    timeSpan = std::chrono::duration_cast<std::chrono::microseconds>(t3-t1);
    std::cout << "All files were copied. It took me " << timeSpan.count() << " microseconds" << std::endl;
    return 0;
}

void parseArgs(int argc, char const **argv, std::string *src, std::string *dst, int *tnum) {
    if (argc != 4) {
        std::cerr << "Incorrect number of arguments" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (argv[1][0] != '-' || argv[1][1] != 't' || argv[1][2] <= int0 || argv[1][2] > int9) {
        std::cerr << "Number of threads is unspecified or entered incorrectly" << std::endl;
        exit(EXIT_FAILURE);
    }
    *tnum = (int)argv[1][2]-int0;
    if (*tnum <= 0) {
        std::cerr << "Number of threads is incorrect" << std::endl;
        exit(EXIT_FAILURE);
    }
    *src = argv[2];
    *dst = argv[3];
}

void copyFile(std::string src, std::string dst) {
    std::string filepath = src;
    std::string newFilepath = dst;

    // checking the filename for existence in the directory-receiver
    if (!access(newFilepath.c_str(), 0)) {
        if (!access((newFilepath + ".old").c_str(), 0)) {
            remove((newFilepath + ".old").c_str());
        }
        if (rename(newFilepath.c_str(), (newFilepath + ".old").c_str()) != 0) {
            perror(("Cannot rename existing " + src).c_str());
        }
    }

    std::ifstream file(filepath, std::ifstream::in);
    if (file.fail()) {
        perror(("File " + src + " cannot be opened").c_str());
        return;
    }

    std::ofstream newFile(newFilepath, std::ofstream::out);
    if (newFile.fail()) {
        perror(("File " + src + " cannot be created in destination folder").c_str());
        return;
    }

    char *buf = new char [BUFFER_SIZE];
    int size = 1;

    while(size != 0) {
        size = file.readsome(buf, BUFFER_SIZE);
        if (file.fail()) {
            perror(("File " + src + " cannot be read").c_str());
            return;
        }
        newFile.write(buf, size);
        if (newFile.fail()) {
            perror(("File " + src + " cannot be copied").c_str());
            return;
        }
    }

    file.close();
    newFile.close();

    // setting destination file metadata
    struct stat st;
    struct utimbuf tm;
    if (stat(filepath.c_str(), &st) != 0) {
        perror(("File " + src + " has been copied, but metadata copying has failed").c_str());
        return;
    }
    if (chmod(newFilepath.c_str(), st.st_mode) != 0) {
        perror(("File " + src + " has been copied, but rights settings copying has failed").c_str());
        return;
    }
    if (chown(newFilepath.c_str(), st.st_uid, st.st_gid) != 0) {
        perror(("File " + src + " has been copied, but owners settings copying has failed").c_str());
        return;
    }
    tm.actime = st.st_atime;
    tm.modtime = st.st_mtime;
    if (utime(newFilepath.c_str(), &tm) != 0) {
        perror(("File " + src + " has been copied, but time settings copying has failed").c_str());
        return;
    }

    delete [] buf;
}

void copyDir(std::string src, std::string dst) {
    struct stat st;
    if (stat(src.c_str(), &st) != 0) {
        perror(("Stat of " + src + " cannot be read").c_str());
        return;
    }

    // create the directory if it does not exist in the destination folder
    if (access(dst.c_str(), 0) != 0) {
        errno = 0;
        if (mkdir(dst.c_str(), st.st_mode) != 0) {
            perror("Cannot create directory");
            return;
        }

        // setting destination directory metadata
        struct utimbuf tm;
        if (chmod(dst.c_str(), st.st_mode) != 0) {
            perror(("Cannot copy rights settings of " + src).c_str());
            return;
        }
        if (chown(dst.c_str(), st.st_uid, st.st_gid) != 0) {
            perror(("Cannot copy owners settings of " + src).c_str());
            return;
        }
        tm.actime = st.st_atime;
        tm.modtime = st.st_mtime;
        if (utime(dst.c_str(), &tm) != 0) {
            perror(("Cannot copy time settings of " + src).c_str());
            return;
        }
        return;
    }

    if (stat(dst.c_str(), &st) != 0) {
        perror(("Directory " + dst + " exists and cannot be read").c_str());
        return;
    }

    if (!S_ISDIR(st.st_mode)) {
        std::cerr << dst << " exists and is not a folder" << std::endl;
        return;
    }
}

void makeTasklist(std::string src, std::string dst, std::vector<Task> *tasks) {
    std::string newsrc, newdst;
    copyDir(src, dst);
    DIR *dir = opendir(src.c_str());
    struct stat st;
    if (dir) {
        struct dirent *entry;
        while ((entry = readdir(dir))) {
            if (entry->d_name[0] == '.')
                continue;
            newsrc = src + "/" + std::string(entry->d_name);
            if (stat(newsrc.c_str(), &st) != 0) {
                perror(("Stat of " + newsrc + " cannot be read").c_str());
                continue;
            }

            // if file
            if (S_ISREG(st.st_mode)) {
                Task task;
                task.src = newsrc;
                task.dst = dst + "/" + std::string(entry->d_name);
                tasks->push_back(task);
            } else if (S_ISDIR(st.st_mode)) {             // if directory
                newdst = dst + "/" + std::string(entry->d_name);
                makeTasklist(newsrc, newdst, tasks);
            }
            if (errno) {
                perror(("Directory " + src + " cannot be read").c_str());
                errno = 0;
            }
        }
    }
    else {
        perror(("Directory " + src + " cannot be opened").c_str());
    }
    closedir(dir);
}

void thrCopy(Args &args) {
    while (true) {
        args.mutex.lock();
        if (errno) {
            perror("Mutex cannot be locked");
            return;
        }

        // if there are any tasks
        if (args.taskSize) {
            Task task = args.tasks[args.taskPos];
            ++args.taskPos;
            --args.taskSize;
            args.mutex.unlock();
            if (errno) {
                perror("Mutex cannot be unlocked");
                return;
            }
            copyFile(task.src, task.dst);
        } else {
            args.mutex.unlock();
            if (errno) {
                perror("Mutex cannot be unlocked");
                return;
            }
            return;
        }
    }
}
