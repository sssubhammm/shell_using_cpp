#include <iostream>
#include <string>
using namespace std;
int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;


  std::string input;
  while (true) {
    cout << "$ ";
    if (!getline(cin, input)) break;
    if(input == "exit 0") return 0;
    cout << input << ": command not found" << endl;
  }
}
