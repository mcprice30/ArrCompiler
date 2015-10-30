#ifndef TESTER_H
#define TESTER_H

#include <string>
#include <iostream>


class Tester {
private:
  int numPassed;
  int numFailed;

public:
  Tester();
  void expectEqual(int expected, int actual, std::string description);
  void expectEqual(bool expected, bool actual, std::string description);
  void printResults();
};

#endif
