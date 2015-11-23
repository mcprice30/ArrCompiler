#include "runtimelib.h"

extern "C"
int getValue(int arrId, int idx) {
  if (vals.find(arrId) == vals.end()) {
    return 0;
  } else if ( (vals[arrId]).find(idx) == (vals[arrId]).end()) {
    return 0;
  } else {
    return vals[arrId][idx];
  }
}

extern "C"
void setValue(int val, int idx, int arrId) {
  vals[arrId][idx] = val;
}

extern "C"
void print(int val) {
  std::cout << val;
}

extern "C"
void printChar(int val) {
  std::cout << (char) val;
}
