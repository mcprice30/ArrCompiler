#include "Parser.h"

int main() {
  cout << "PARSER TEST" << std::endl;

  Parser p = Parser(true);

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
    cout << p.parseExp(test) << endl;
    cout << endl;
  }

  return 0;
}

Parser::Parser(bool debugSwitch) {
  debug = debugSwitch;
}

string Parser::parseExp (string expr) {
  int plusIdx = -1, minusIdx = -1, bracketDepth = 0, parenDepth = 0;
  bool prevTokenOp = true;
  if (debug) { cout << "EXPR " << expr << endl; }

  for (int i = expr.size() - 1; i >= 0; i--) {
    char c = expr.at(i);
    if (c == '[') {
      bracketDepth--;
      if (bracketDepth < 0) {
        cerr << "Error: too many '[' without a matching ']'" << endl;
        exit(1);
      }
    } else if (c == ']') {
      bracketDepth++;
    } else if (c == '+' && bracketDepth == 0 && parenDepth == 0) {
      plusIdx = i;
      break;
    } else if (c == '-' && bracketDepth == 0 && parenDepth == 0) {
      //cout << "AT MINUS " << endl;
      //cout << evalMinusAsSub(expr, i) << endl;
      if (evalMinusAsSub(expr, i)) {
        minusIdx = i;
        break;
      }
    } else if (c == ' ') {

    } else if (c == '(') {
      parenDepth --;
      if (parenDepth < 0) {
        cerr << "Error: too many '(' without a matching ')'" << endl;
        exit(1);
      }
    } else if (c == ')') {
      parenDepth++;
    }
  }

  if (bracketDepth > 0) {
    cerr << "Error: too many ']' without a matching '['" << endl;
    exit(1);
  }

  if (parenDepth > 0) {
    cerr << "Error: too many ')' without a matching '('" << endl;
    exit(1);
  }

  if (plusIdx >= 0) {
    string lExp= expr.substr(0, plusIdx);
    string rTerm = expr.substr(plusIdx + 1);
    return parseAdd(lExp, rTerm);
  } else if (minusIdx >= 0) {
    string lExp = expr.substr(0, minusIdx);
    string rTerm = expr.substr(minusIdx + 1);
    return parseSub(lExp, rTerm);
  } else {
    return parseTerm(expr);
  }
}

string Parser::parseTerm (string term) {
  int mulIdx = -1, bracketDepth = 0, parenDepth = 0;

  if (debug) { cout << "TERM " << term << endl; }

  for (int i = term.size() - 1; i >= 0; i--) {
    char c = term.at(i);

    if (c == '[') {
      bracketDepth--;
    } else if (c == ']') {
      bracketDepth++;
    } else if (c == '*' && bracketDepth == 0 && parenDepth == 0){
      mulIdx = i;
      break;
    } else if (c == '(') {
      parenDepth--;
    } else if (c == ')') {
      parenDepth++;
    }
  }

  if (mulIdx >= 0) {
    string lTerm = term.substr(0, mulIdx);
    string rId = term.substr(mulIdx + 1);
    return parseMul(lTerm, rId);
  } else {
    return parseId(term);
  }

}

string Parser::parseId(string id) {

  if (debug) { cout << "ID " << id << endl; }

  if (isNum(id)) {
    return parseNum(id);
  } else if (isVariableLookup(id)){
    int arrStartIdx = -1, arrEndIdx = -1;
    for (int i = 0; i < id.size(); i++) {
      if (id.at(i) == '[') {
        arrStartIdx = i;
        break;
      }
    }
    for (int i = id.size() - 1; i >= 0; i--) {
      if (id.at(i) == ']') {
        arrEndIdx = i;
        break;
      }
    }

    string beforeArray = id.substr(0, arrStartIdx);
    string idxExp = id.substr(arrStartIdx + 1, arrEndIdx - arrStartIdx - 1);
    return parseDeref(beforeArray, idxExp);

  } else if (isParenExp(id)) {
    int parenStartIdx = -1, parenEndIdx = -1;
    for (int i = 0; i < id.size(); i++) {
      if (id.at(i) == '(') {
        parenStartIdx = i;
        break;
      }
    }
    for (int i = id.size() -1; i >= 0; i--) {
      if (id.at(i) == ')') {
        parenEndIdx = i;
        break;
      }
    }
    string insideParen = id.substr(parenStartIdx + 1, parenEndIdx - parenStartIdx - 1);
    return parseExp(insideParen);

  } else {
    cerr << "Error: cannot resolve " << id << "." << endl;
    exit(1);
  }
}

string Parser::parseAdd(string lExp, string rTerm) {
  //if (debug) {cout << "PARSING: " << lExp << " PLUS " << rTerm << endl;}
  string out = "";
  out += "; " + lExp+ " + " + rTerm + "\r\n";
  out += parseTerm(rTerm);
  out += "push ebx\r\n";
  out += "mov ebx, eax\r\n";
  out += parseExp(lExp);
  out += "add eax, ebx\r\n";
  out += "pop ebx\r\n";
  return out;
}

string Parser::parseSub(string lExp, string rTerm) {
  string out = "";
  out += "; " + lExp + " - " + rTerm + "\r\n";
  out += parseTerm(rTerm);
  out += "push ebx\r\n";
  out += "mov ebx, eax\r\n";
  out += parseExp(lExp);
  out += "sub eax, ebx\r\n";
  out += "pop ebx\r\n";
  return out;
}

string Parser::parseMul(string lTerm, string rId) {
  string out = "";
  out += "; " + lTerm + " * " + rId + "\r\n";
  out += parseId(rId);
  out += "push ebx\r\n";
  out += "mov ebx, eax\r\n";
  out += parseTerm(lTerm);
  out += "imul eax, ebx\r\n";
  out += "pop ebx\r\n";
  return out;
}

string Parser::parseDeref(string arrayName, string indexExp) {
  string out = "";
  out += "; " + arrayName + "[ " + indexExp + " ]\r\n";
  out += "; NOTE: DEREFERENCING IS NOT YET IMPLEMENTED\r\n";
  out += "; This is currently evaluated as: (" + indexExp + ")\r\n";
  out += parseExp(indexExp);
  return out;
}

string Parser::parseNum(string number) {
  if(debug) {cout << "NUMBER" << endl;}
  string out = "";
  out += "; assignment\r\n";
  out += "mov eax, " + number + "\r\n";
  return out;
}

bool Parser::isWhiteSpace(string text) {
  for (int i = 0; i < text.size(); i++) {
    if (text.at(i) != ' ') {
      return false;
    }
  }
  return true;
}

bool Parser::isNum(string text) {
  bool hasNum = false;

  for (int i = 0; i < text.size(); i++) {
    char c = text.at(i);

    if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5'
                 || c == '6' || c == '7' || c == '8' || c == '9' ) {

      hasNum = true;
    } else if (c == '-' || c == ' ') {

    } else {
      return false;
    }
  }

  if (!hasNum) {
    cerr << "Error: '" << text << "' cannot be resolved to an integer." << endl;
    exit(1);
  }

  return hasNum;
}

bool Parser::evalMinusAsSub(string text, int idx) {
  for (int i = idx - 1; i >= 0; i--) {
    char c = text.at(i);
    //cout << c << endl;
    if (c == '*' || c == '+' || c == '-' || c == '(' || c == '[') {
      //cout << "OPERATOR" << endl;
      return false;
    } else if (c == ' ') {
      //cout << "SPACE" << endl;
    } else {
      return true;
    }
  }
  //cout << "DONE" << endl;
  return false;
}

bool Parser::isParenExp(string text) {
  for (int i = 0; i < text.size(); i++) {
    char c = text.at(i);
    if (c == '(') {
      break;
    } else if (c == ' ') {

    } else {
      return false;
    }
  }

  for (int i = text.size() - 1; i >= 0; i--) {
    char c = text.at(i);
    if (c == ')') {
      break;
    } else if (c == ' ') {

    } else {
      return false;
    }
  }
  return true;
}

bool Parser::isVariableLookup(string text) {
  for (int i = text.size() - 1; i >= 0; i--) {
    char c = text.at(i);
    if (c == ']') {
      break;
    } else if (c == ' ') {

    } else {
      return false;
    }
  }
  int openBracketIdx = -1;
  for (int i = 0; i < text.size(); i++) {
    if (text.at(i) == '[') {
      openBracketIdx = i;
      break;
    }
  }
  if (openBracketIdx < 0) {
    return false;
  } else {
    return isValidVarName(text.substr(0, openBracketIdx));
  }
}

bool Parser::isValidVarName(string text) {
  string test = " " + text;
  int end = -1, start = -1;
  for (int i = test.size() - 1; i >= 0; i--) {
    if (test.at(i) != ' ') {
      end = i;
      break;
    }
  }
  if (end < 0) {
    return false;
  }

  for (int i = end; i > 0; i--) {
    char c = test.at(i);
    char p = test.at(i-1);
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || p == '_') {
      start = i;
    } else if (c >= '0' && c <= '9') {
      if ((p >= 'a' && p <= 'z') || (p >= 'A' && p <= 'Z') || (p >= '0' && p <= '9') || p == '_') {
        start = i;
      } else {
        return false;
      }
    } else if (c == ' ') {
      if (p == ' ') {

      } else {
        return false;
      }
    } else {
      return false;
    }
  }

  if (test.substr(start, 1 + end - start) == "for") {
    return false;
  }

  return true;
}
