#include <iostream>
using namespace std;
int main() {
  cout << unitbuf;
  cerr << unitbuf;//similar to cout but specifically used for printing error message. Used when you want to log error
  cout << "$ ";

  string input;
  while(getline(cin, input)){
    if(input=="exit 0"){
      return 0;
    }else if(input.find("echo")!=string::npos){
      cout<<input.substr(5)<<endl;
    }else if(input.find("type")!=string::npos){
      string a=input.substr(5);
      if(a=="echo"||a=="exit"||a=="type"){
        cout<<a<<" is a shell builtin"<<endl;
 
      }else{
        cout<<a<<": not found"<<endl;

      }
      }else{
  cout<<input<<": command not found"<<endl;
    }
    cout<<"$ ";
  }
  return 0;
}

