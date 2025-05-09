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
      cout<<"$ ";
    }else{
  cout<< input<<": command not found"<<endl;
  cout<<"$ ";
    }
  }
}
