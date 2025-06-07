#include<bits/stdc++.h>
#include <unistd.h>
#include <sys/wait.h>
#include<filesystem>
using namespace std;
namespace fs = filesystem;
int main() {
  cout << unitbuf;
  cerr << unitbuf;//similar to cout but specifically used for printing error message. Used when you want to log error
  cout << "$ ";

  string input;
  while(getline(cin, input)){
    if(input=="exit 0"){
      return 0;
    }else if (input == "pwd"){//pwd command prints the current working directory
      char cwd[PATH_MAX];
      if (getcwd(cwd, sizeof(cwd)) != nullptr){
          cout << cwd << endl;
    }else{
      perror("getcwd");
    }
  }else if(input.find("echo")==0){//!=string:npos bhi de skte par it then $type echo yaha pe aa jaega dusre cond me nhi jaega
      cout<<input.substr(5)<<endl;
    }else if(input.find("type")==0){
      string a=input.substr(5);
      if (!a.empty()&&a[0] == ' '){
        a = a.substr(1);
      }
      if(a=="echo"||a=="exit"||a=="type"|| a=="pwd"){
        cout<<a+" is a shell builtin"<<endl;
      }else{
        const char* pathEnv = getenv("PATH");//returns the value of the environment variable PATH 
                if (pathEnv == nullptr) {
                    cout << a << ": not found" << endl;
                } else {
                    string pathStr(pathEnv);// Convert to string for easier manipulation, convert char* to string
                    stringstream ss(pathStr);//it split the string using ':' as delimiter
                    string dir;
                    bool found = false;

                    // Step 3: Split by ':' and check each dir
                    while (getline(ss, dir, ':')) {//jab tak ':' nahi milta tab tak line ko read karta rahega
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
        }else{
          //handling external commands
          stringstream iss(input);
          vector<string> tokens;
          string token;
          while(iss>>token){//ek ek word jo stringstream toda wo token me daal dega
            tokens.push_back(token);
          }
          if(tokens.empty()){
            cout<<"$ ";
            continue;
          }
           string command = tokens[0];
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
            }else{
              //building argv
              vector<char*> argv;
              for(const string& arg : tokens) {
                argv.push_back(const_cast<char*>(arg.c_str()));//c_str() returns a const char* pointer to the string's data. Par execv ko const char* nahi chahiye. essiliye const_cast use kiya taki wo const char* ko char* me convert kar sake
              }
              argv.push_back(nullptr);//execv ko last me null pointer chahiye hota hai
              pid_t pid = fork();  // create child
              if(pid == 0){
                execv(fullPath.c_str(), argv.data());
                perror("execv");
                exit(1);
              } else {
                int status;
                waitpid(pid, &status, 0);
                }
            }
    } 
    cout<<"$ ";
  }
  return 0;
}

