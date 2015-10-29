#include "Parser.h"


int main() {
  cout << "PARSER TEST" << std::endl;

  Parser p = Parser(false);

  vector<string> tests;
  tests.push_back("1+1");
  tests.push_back("1 + 1");
  tests.push_back("1 - 2");
  tests.push_back("2 * 3 - 1 * 1");
  tests.push_back("x[1+1]*3 - 2");
  tests.push_back("-1*-2");
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
  int plusIdx = -1, minusIdx = -1, bracketDepth = 0;
  bool prevTokenOp = true;
  if (debug) { cout << "EXPR " << expr << endl; }

  for (int i = 0; i < expr.size(); i++) {
    char c = expr.at(i);
    if (c == '[') {
      bracketDepth++;
      prevTokenOp = true;
    } else if (c == ']') {
      bracketDepth--;
      if (bracketDepth < 0) {
        cerr << "Error: too many ']' without a matching '['" << endl;
        exit(1);
      }
      prevTokenOp = false;
    } else if (c == '+' && bracketDepth == 0) {
      plusIdx = i;
      break;
    } else if (c == '-' && bracketDepth == 0) {
      if (!prevTokenOp) {
        minusIdx = i;
        break;
      }
    } else if (c == ' ') {

    } else if (c == '*') {
      prevTokenOp = true;
    } else {
      prevTokenOp = false;
    }
  }

  if (bracketDepth > 0) {
    cerr << "Error: too many '[' without a matching ']'" << endl; 
    exit(1);
  }

  if (plusIdx >= 0) {
    string lTerm = expr.substr(0, plusIdx);
    string rExp = expr.substr(plusIdx + 1);
    return parseAdd(lTerm, rExp);
  } else if (minusIdx >= 0) {
    string lTerm = expr.substr(0, minusIdx);
    string rExp = expr.substr(minusIdx + 1);
    return parseSub(lTerm, rExp);
  } else {
    return parseTerm(expr);
  }
}

string Parser::parseTerm (string term) {
  int mulIdx = -1, bracketDepth = 0;

  if (debug) { cout << "TERM " << term << endl; }

  for (int i = 0; i < term.size(); i++) {
    char c = term.at(i);

    if (c == '[') {
      bracketDepth++;
    } else if (c == ']') {
      bracketDepth--;
    } else if (c == '*' && bracketDepth == 0){
      mulIdx = i;
      break;
    }
  }

  if (mulIdx >= 0) {
    string lId = term.substr(0, mulIdx);
    string rTerm = term.substr(mulIdx + 1);
    return parseMul(lId, rTerm);
  } else {
    return parseId(term);
  }

}

string Parser::parseId(string id) {

  if (debug) { cout << "ID " << id << endl; }

  if (isNum(id)) {
    return parseNum(id);
  } else {
    int idxLoc = -1;
    for (int i = 0; i < id.size(); i++) {
      char c = id.at(i);
      if (c == '[') {
        idxLoc = i;
        break;
      }
    }

    if (idxLoc >= 0) {
      int idxEnd = -1;
      for (int i = id.size() - 1; i > idxLoc; i--) {
        if (id.at(i) == ']') {
          idxEnd = i;
          break;
        }
      }

      if (idxEnd >= 0) {
        string beforeArray = id.substr(0, idxLoc);
        string idxExp = id.substr(idxLoc + 1, (idxEnd - idxLoc - 1));
        string afterIdx = id.substr(idxEnd + 1);
        if (!isWhiteSpace(afterIdx)) {
          cerr << "Error: missing operator before '" << afterIdx << "'." << endl;
          exit(1);
        } else {
          return parseDeref(beforeArray, idxExp);
        }
      } else {
        cerr << "Error: too many '[' without a matching ']'." << endl;
        exit(1);
      }
    } else {
      cerr << "Error: '" << id << "' cannot be resolved to a variable." << endl;
      exit(1);
    }

    return " PARSE ERROR! \r\n";
  }
}

string Parser::parseAdd(string lTerm, string rExp) {
  if (debug) {cout << "PARSING: " << lTerm << " PLUS " << rExp << endl;}
  string out = "";
  out += "; " + lTerm + " + " + rExp + "\r\n";
  out += parseExp(rExp);
  out += "push ebx\r\n";
  out += "mov ebx, eax\r\n";
  out += parseTerm(lTerm);
  out += "add eax, ebx\r\n";
  out += "pop ebx\r\n";
  return out;
}

string Parser::parseSub(string lTerm, string rExp) {
  string out = "";
  out += "; " + lTerm + " - " + rExp + "\r\n";
  out += parseExp(rExp);
  out += "push ebx\r\n";
  out += "mov ebx, eax\r\n";
  out += parseTerm(lTerm);
  out += "sub eax, ebx\r\n";
  out += "pop ebx\r\n";
  return out;
}

string Parser::parseMul(string lId, string rTerm) {
  string out = "";
  out += "; " + lId + " * " + rTerm + "\r\n";
  out += parseTerm(rTerm);
  out += "push ebx\r\n";
  out += "mov ebx, eax\r\n";
  out += parseId(lId);
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
