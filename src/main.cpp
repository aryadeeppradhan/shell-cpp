#include <iostream>
using namespace std;
int main() {
  // Flush after every std::cout / std:cerr
  cout << unitbuf;
  cerr << unitbuf;//similar to cout but specifically used for printing error message. Used when you want to log error

  // Uncomment this block to pass the first stage
  cout << "$ ";

  string input;
  while(getline(cin, input)){
    if(input=="exit 0"){
      return 0;
    }
  cout<< input<<": command not found"<<endl;
  cout<<"$ ";
  }
}
