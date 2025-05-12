#include<bits/stdc++.h>
#include<unistd.h>
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
    }else if(input.find("echo")==0){//!=string:npos bhi de skte par it then $type echo yaha pe aa jaega dusre cond me nhi jaega
      cout<<input.substr(5)<<endl;
    }else if(input.find("type")==0){
      string a=input.substr(5);
      if (!a.empty()&&a[0] == ' ') {
        a = a.substr(1);
      }
      if(a=="echo"||a=="exit"||a=="type"){
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
        }else{
  cout<<input+": command not found"<<endl;
    }
    cout<<"$ ";
  }
  return 0;
}

