#include "Tester.h"

using namespace std;

Tester::Tester() {
  numPassed = 0;
  numFailed = 0;
}

void Tester::expectEqual(int expected, int actual, string description) {
  cout << "[TEST] " << description << endl;
  if (expected == actual) {
    cout << "[PASS]" << endl;
    numPassed ++;
  } else {
    cout << "[FAIL]" << endl;
    cout << "Expected: " << expected << " but recieved: " << actual << endl;
    numFailed++;
  }
}

void Tester::expectEqual(bool expected, bool actual, string description) {
  cout << "[TEST] " << description << endl;
  if (expected == actual) {
    cout << "[PASS]" << endl;
    numPassed ++;
  } else {
    cout << "[FAIL]" << endl;
    cout << "Expected: " << expected << " but recieved: " << actual << endl;
    numFailed++;
  }
}

void Tester::printResults() {
  cout << endl;
  cout << "    [RESULTS]  " << endl;
  cout << "[PASS] " << numPassed << " tests." << endl;
  cout << "[FAIL] " << numFailed << " tests." << endl;
  cout << endl;
}
