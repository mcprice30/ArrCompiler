#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

class Parser {

private:
  bool debug;

public:

  Parser(bool debugSwitch);

  string parseExp (string expr);
  string parseTerm (string term);
  string parseId(string id);

  string parseAdd(string lTerm, string rExp);
  string parseSub(string lTerm, string rExp);
  string parseMul(string lId, string rTerm);
  string parseDeref(string arrayName, string indexExp);
  string parseNum(string number);

  bool isWhiteSpace(string text);
  bool isNum(string text);
};
