#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/wait.h>
#include <filesystem>
using namespace std;
namespace fs = filesystem;

vector<string> tokenize(const string& input) {
    vector<string> tokens;
    string token;
    bool inSingleQuote = false;
    bool inDoubleQuote = false;
    for (size_t i = 0; i < input.length(); ++i) {
        char c = input[i];
        if (inSingleQuote) {
            if (c == '\'') {
                inSingleQuote = false;
            } else {
                token += c;
            }
        } else if (inDoubleQuote) {
            if (c == '"') {
                inDoubleQuote = false;
            } else if (c == '\\' && i + 1 < input.size() &&
                      (input[i + 1] == '"' || input[i + 1] == '\\' || input[i + 1] == '$' || input[i + 1] == '\n')) {
                token += input[++i];  // take escaped character
            } else {
                token += c;
            }
        } else {
            if (isspace(c)) {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token.clear();
                }
            } else if (c == '\'') {
                inSingleQuote = true;
            } else if (c == '"') {
                inDoubleQuote = true;
            } else {
                token += c;
            }
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }
    return tokens;
}

int main() {
    cout << unitbuf;
    cerr << unitbuf;
    cout << "$ ";

    string input;
    while (getline(cin, input)) {
        vector<string> tokens = tokenize(input);
        if (tokens.empty()) {
            cout << "$ ";
            continue;
        }

        string command = tokens[0];

        if (command == "exit" && tokens.size() == 2 && tokens[1] == "0") {
            return 0;

        } else if (command == "cd") {
            if (tokens.size() < 2) {
                // Optional: cd with no argument could go to HOME
                cout << "$ ";
                continue;
            }
            string path = tokens[1];

            if (path[0] == '~') {
                const char* home = getenv("HOME");
                if (home != nullptr) {
                    if (path == "~") {
                        path = string(home);
                    } else if (path[1] == '/') {
                        path = string(home) + path.substr(1);
                    }
                }
            }

            if (chdir(path.c_str()) != 0) {
                cerr << "cd: " << path << ": No such file or directory" << endl;
            }

        } else if (command == "pwd") {
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) != nullptr) {
                cout << cwd << endl;
            } else {
                perror("getcwd");
            }

        } else if (command == "echo") {
            for (size_t i = 1; i < tokens.size(); ++i) {
                cout << tokens[i];
                if (i != tokens.size() - 1) cout << " ";
            }
            cout << endl;

        } else if (command == "type") {
            if (tokens.size() < 2) {
                cout << "type: missing argument" << endl;
            } else {
                string a = tokens[1];
                if (a == "echo" || a == "exit" || a == "type" || a == "pwd" || a == "cd") {
                    cout << a << " is a shell builtin" << endl;
                } else {
                    const char* pathEnv = getenv("PATH");
                    if (pathEnv == nullptr) {
                        cout << a << ": not found" << endl;
                    } else {
                        string pathStr(pathEnv);
                        stringstream ss(pathStr);
                        string dir;
                        bool found = false;
                        while (getline(ss, dir, ':')) {
                            string fullPath = dir + "/" + a;
                            if (fs::exists(fullPath) && fs::is_regular_file(fullPath) && access(fullPath.c_str(), X_OK) == 0) {
                                cout << a << " is " << fullPath << endl;
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            cout << a << ": not found" << endl;
                        }
                    }
                }
            }

        } else {
            const char* pathEnv = getenv("PATH");
            bool found = false;
            string fullPath;

            if (pathEnv != nullptr) {
                stringstream ss(pathEnv);
                string dir;
                while (getline(ss, dir, ':')) {
                    string tempPath = dir + "/" + command;
                    if (fs::exists(tempPath) && fs::is_regular_file(tempPath) && access(tempPath.c_str(), X_OK) == 0) {
                        fullPath = tempPath;
                        found = true;
                        break;
                    }
                }
            }

            if (!found) {
                cout << command << ": command not found" << endl;
            } else {
                vector<char*> argv;
                for (const string& arg : tokens) {
                    argv.push_back(const_cast<char*>(arg.c_str()));
                }
                argv.push_back(nullptr);

                pid_t pid = fork();
                if (pid == 0) {
                    execv(fullPath.c_str(), argv.data());
                    perror("execv");
                    exit(1);
                } else {
                    int status;
                    waitpid(pid, &status, 0);
                }
            }
        }

        cout << "$ ";
    }

    return 0;
}
