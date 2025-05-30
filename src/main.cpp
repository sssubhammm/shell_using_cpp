#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>     // for getenv
#include <unistd.h>    // for access(), fork(), exec(), getcwd()
#include <sys/wait.h>  // for wait()
#include <limits.h>    // for PATH_MAX
using namespace std;

bool is_builtin(const string& cmd) {
    return cmd == "echo" || cmd == "exit" || cmd == "type" || cmd == "pwd";
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

vector<string> split_args(const string& input) {
    vector<string> args;
    stringstream ss(input);
    string arg;
    while (ss >> arg) {
        args.push_back(arg);
    }
    return args;
}

void handle_pwd() {
    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) != nullptr) {
        cout << buffer << endl;
    } else {
        cerr << "pwd: error getting current directory" << endl;
    }
}

int main() {
    cout << std::unitbuf;
    cerr << std::unitbuf;

    string input;
    while (true) {
        cout << "$ ";
        if (!getline(cin, input)) break;

        if (input.empty()) continue;

        if (input == "exit 0") break;

        vector<string> args = split_args(input);
        if (args.empty()) continue;

        string cmd = args[0];

        if (cmd == "echo" && args.size() > 1) {
            // Handle echo with arguments
            cout << input.substr(5) << endl;
        } else if (cmd == "type" && args.size() > 1) {
            string target_cmd = args[1];
            if (is_builtin(target_cmd)) {
                cout << target_cmd << " is a shell builtin" << endl;
            } else {
                string path = find_in_path(target_cmd);
                if (!path.empty()) {
                    cout << target_cmd << " is " << path << endl;
                } else {
                    cout << target_cmd << ": not found" << endl;
                }
            }
        } else if (cmd == "pwd") {
            handle_pwd();
        } else {
            string cmd_path = find_in_path(cmd);
            if (!cmd_path.empty()) {
                char** argv = new char*[args.size() + 1];
                for (size_t i = 0; i < args.size(); i++) {
                    argv[i] = const_cast<char*>(args[i].c_str());
                }
                argv[args.size()] = nullptr;
                
                pid_t pid = fork();
                if (pid == 0) {
                    execv(cmd_path.c_str(), argv);
                    cerr << cmd << ": execution failed" << endl;
                    exit(1);
                } else if (pid > 0) {
                    wait(nullptr);
                } else {
                    cerr << "fork failed" << endl;
                }
                
                delete[] argv;
            } else {
                cout << cmd << ": command not found" << endl;
            }
        }
    }
    return 0;
}