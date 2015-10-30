#include "Parser.h"
#include "Tester.h"

int main() {
  cout << "PARSER TEST" << std::endl;

  Parser p = Parser(true);
  Tester t = Tester();

  t.expectEqual(p.isValidVarName("for"), false, "Variable test 1");
  t.expectEqual(p.isValidVarName("a abc"), false, "Variable test 2");
  t.expectEqual(p.isValidVarName("1abc"), false, "Variable test 3");
  t.expectEqual(p.isValidVarName("abc1"), true, "Variable test 4");
  t.expectEqual(p.isValidVarName("a+b"), false, "Variable test 5");
  t.expectEqual(p.isValidVarName("a "), true, "Variable test 6");

  t.expectEqual(p.isVariableLookup("a[1+1]"), true, "Lookup test 1");
  t.expectEqual(p.isVariableLookup("abc1[]"), true, "Lookup test 2");
  t.expectEqual(p.isVariableLookup("for[1]"), false, "Lookup test 3");
  t.expectEqual(p.isVariableLookup("1abc[2]"), false, "Lookup test 4");
  t.expectEqual(p.isVariableLookup("a[2])"), false, "Lookup test 5");
  t.expectEqual(p.isVariableLookup("[a]"), false, "Lookup test 6");
  t.expectEqual(p.isVariableLookup("a [0]"), true, "Lookup test 7");
  t.printResults();

  vector<string> tests;
  tests.push_back("1+1");
  tests.push_back("1 + 1");
  tests.push_back("1 - 2");
  tests.push_back("2 * 3 - 1 * 1");
  tests.push_back("x[1+1]*3 - 2");
  tests.push_back("-1*-2");
  tests.push_back("x[1*x[1+2] + 5] - 3 * x[2-1]");
  tests.push_back("(1+1)*(2+1)");
  tests.push_back("1*(1+(2*3-4)*-2)-5*(1+-2)+7");
  tests.push_back("2 - 1 - 1");
  tests.push_back("x[[1+1]*3 - 2");

  for (int i = 0; i < tests.size(); i++) {
    string test = tests.at(i);
    cout << test << endl;
    cout << "ASSEMBLY: " << endl;
    cout << endl;
    cout << p.compileExp(test) << endl;
    cout << endl;
  }

  return 0;
}
