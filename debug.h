#ifndef DEBUG
#define DEBUG

#include <iostream>
#include <string>
namespace d {
  static int debug = 1;

  void ebug(std::string s) {
    if (debug) std::cout << s << std::endl;
  }
}
#endif
