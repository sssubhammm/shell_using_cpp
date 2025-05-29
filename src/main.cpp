#include <iostream>
#include <string>
using namespace std;
int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;


  std::string input;
  while (true) {
    string input;
    cout << "$ ";
    getline(std::cin, input);
    if (input == "exit 0") {
      break;
    } else if (input.substr(0, 4) == "echo") {
        cout << input.substr(5) << endl;
    } else if (input.substr(0, 7) == "invalid") {
        cout << input + ": command not found" << endl;
    } else if (input.substr(0, 9) == "type echo" || input.substr(0, 9) == "type exit" || input.substr(0, 9) == "type type") {
        cout << input.substr(5) + " is a shell builtin" << endl;
    } else {
        cout << input.substr(5) + ": not found" << endl;
    }
  }
  return 0;
}
