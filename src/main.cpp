#include <iostream>
#include <unistd.h>
#include <string>
#include <cstdlib>
#include <filesystem>
#include <sstream>
using namespace std;
namespace fs = std::filesystem;
int main() {
  cout << unitbuf;
  cerr << unitbuf;//similar to cout but specifically used for printing error message. Used when you want to log error
  cout << "$ ";

  string input;
  while(getline(cin, input)){
    if(input=="exit 0"){
      return 0;
    }else if(input.find("echo")==0){//!=string:npos bhi de skte par it then $type echo yaha pe aa jaega dusre cond me nhi jaega
      cout<<input.substr(5)<<endl;
    }else if(input.find("type")==0){
      string a=input.substr(5);
      if (a[0] == ' ') {
        a = a.substr(1);
      }
      if(a=="echo"||a=="exit"||a=="type"){
        cout<<a+" is a shell builtin"<<endl;
 
      }else {
        // Get PATH from environment
        const char* path_env = getenv("PATH");
        if (path_env) {
          stringstream ss(path_env);
          string dir;
          bool found = false;
          while (getline(ss, dir, ':')) {
            fs::path full_path = fs::path(dir) / a;
            if (fs::exists(full_path) && fs::is_regular_file(full_path) && access(full_path.c_str(), X_OK) == 0) {
              cout << a << " is " << full_path << endl;
              found = true;
              break;
            }
          } if (!found) {
            cout << a << ": not found" << endl;
          }
        }else {
          cout << "PATH environment variable not set." << endl;
        }
      }
      }else{
  cout<<input+": command not found"<<endl;
    }
    cout<<"$ ";
  }
  return 0;
}

