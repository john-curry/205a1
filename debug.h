#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <string>
namespace d {
  
  static int debug = 0;

  void ebug(std::string s) {
    if (debug) std::cout << s << std::endl;
  }
}
#endif
