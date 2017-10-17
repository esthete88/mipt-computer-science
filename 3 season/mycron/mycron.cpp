#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <ctime>
#include <sys/stat.h>

using namespace std;

struct Task {
        int h, m, s;
        string line;
};


int checkSymbol(char);
vector<string> parseStr(string S, char delimiter);
string* splitTask(string S);
int strToInt(string S);
Task parseToTask(string S);
void launch(string S);


int main() {
        ifstream file("mycrontab.txt");
        if (file.fail()) {
                perror("File doesn't exist");
                return 0;
        }
        vector<Task> tasks;
        string temp;
        struct stat buf;
        stat("mycrontab.txt", &buf);
        time_t raw_time, mtime = buf.st_mtime;
        tm* times;
        getline(file, temp);
        while (temp != "") {
                auto ptt = parseToTask(temp);
                if (ptt.h >= -1 && ptt.h < 24)
                        tasks.emplace_back(ptt);
                getline(file, temp);
        }
        while (true) {
                time(&raw_time);
                times = localtime(&raw_time);
                stat("mycrontab.txt", &buf);
                if (mtime != buf.st_mtime)
                        main();
                for (auto task : tasks) {
                        if ((task.h == times->tm_hour || task.h == -1) &&
                            (task.m == times->tm_min || task.m == -1) &&
                            (task.s == times->tm_sec || task.s == -1)) {
                                launch(task.line);
                        }
                }
                sleep(1);
        }
        return 0;
}

int checkSymbol(char c) {
        if (c >= (int)('0') && c <= (int)('9')) { // number
                return 0;
        } else if (c == (int)('*')) { // asterisk
                return 1;
        } else { // NaN
                return -1;
        }
}

vector<string> parseStr(string S, char delimiter) {
        size_t pos;
        vector<string> Split;
        string temp;
        while((pos = S.find(delimiter)) != string::npos) {
                temp = S.substr(0, pos);
                Split.emplace_back(temp);
                S.erase(0, pos + 1);
        }
        Split.emplace_back(S);
        return Split;
}

string* splitTask(string S) {
        size_t pos;
        string* split = new string [2];
        pos = S.find(' ');
        if (pos != string::npos) {
                split[0] = S.substr(0, pos);
                S.erase(0, pos + 1);
                split[1] = S;
        }
        else {
                cout << "Incorrect input" << endl;
                return NULL;
        }
        return split;
}

int strToInt(string S) {
        int x = 0, length = S.size();
        for (int i = 0; i < length; ++i) {
                x += (S[i]-48) * pow(10, length - i - 1);
        }
        return x;
}

Task parseToTask(string S) {
        auto split = splitTask(S);
        Task task;
        if (split == NULL) {
                return task;
        }
        task.line = split[1];
        auto time = parseStr(split[0], ':');
        if (time.size() != 3) {
                cout << "Incorrect time format" << endl;
                return task;
        }
        for (int i = 0; i < 3; ++i) {
                int p = checkSymbol(time[i][0]);
                switch (i) {
                case 0:
                        if (p == 0)
                                task.h = strToInt(time[i]);
                        else if (p == 1)
                                task.h = -1;
                        break;
                case 1:
                        if (p == 0)
                                task.m = strToInt(time[i]);
                        else if (p == 1)
                                task.m = -1;
                        break;
                default:
                        if (p == 0)
                                task.s = strToInt(time[i]);
                        else if (p == 1)
                                task.s = -1;
                        break;
                }
        }
        return task;
}

void launch(string S) {
        auto vargs = parseStr(S, ' ');
        vector<const char*> args;
        for (auto const &q : vargs) {
                args.push_back(q.c_str());
        }
        args.push_back(NULL);
        pid_t pid = fork();
        if (pid == 0) {
                int num = execvp(args[0], (char**)&args[0]);
                if (num == -1) {
                        perror("Process was not executed");
                        exit(123);
                }
        }
}
