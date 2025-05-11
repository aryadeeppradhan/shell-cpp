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
    }else if(input.find("echo")==0){//!=string:npos bhi de skte par it then $type echo yaha pe aa jaega dusre cond me nhi jaega
      cout<<input.substr(5)<<endl;
    }else if(input.find("type")==0){
      string a=input.substr(5);
      if (a[0] == ' ') {
        a = a.substr(1);
      }
      if(a=="echo"||a=="exit"||a=="type"){
        cout<<a+" is a shell builtin"<<endl;
 
      }else{
        cout<<a+": not found"<<endl;

      }
      }else{
  cout<<input+": command not found"<<endl;
    }
    cout<<"$ ";
  }
  return 0;
}

