#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>     // for getenv
#include <unistd.h>    // for access()
using namespace std;

bool is_builtin(const string& cmd) {
    return cmd == "echo" || cmd == "exit" || cmd == "type";
}

string find_in_path(const string& cmd) {
    const char* path_env = getenv("PATH");
    if (!path_env) return "";

    stringstream ss(path_env);
    string dir;

    while (getline(ss, dir, ':')) {
        string full_path = dir + "/" + cmd;
        if (access(full_path.c_str(), X_OK) == 0) {
            return full_path;
        }
    }
    return "";
}

int main() {
    cout << std::unitbuf;
    cerr << std::unitbuf;

    string input;
    while (true) {
        cout << "$ ";
        if (!getline(cin, input)) break;

        if (input == "exit 0") break;

        if (input.rfind("echo ", 0) == 0) {
            cout << input.substr(5) << endl;
        } else if (input.rfind("type ", 0) == 0) {
            string cmd = input.substr(5);
            if (is_builtin(cmd)) {
                cout << cmd << " is a shell builtin" << endl;
            } else {
                string path = find_in_path(cmd);
                if (!path.empty()) {
                    cout << cmd << " is " << path << endl;
                } else {
                    cout << cmd << ": not found" << endl;
                }
            }
        } else {
            cout << input << ": command not found" << endl;
        }
    }

    return 0;
}
