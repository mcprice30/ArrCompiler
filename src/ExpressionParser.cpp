#include "Parser.h"

using namespace std;

Parser::Parser(bool debugSwitch) {
  debug = debugSwitch;
  pUtil = ParseUtil();
}

string Parser::compileExp(string expr) {
  string output = "";
  bool result = parseExp(expr, output);

  if (result) {
    return output;
  } else {
    return "Did not compile.";
  }
}

bool Parser::parseExp (string expr, string &out) {
  bool success = false;
  int plusIdx = -1, minusIdx = -1, bracketDepth = 0, parenDepth = 0;
  bool prevTokenOp = true;
  if (debug) { cout << "EXPR " << expr << endl; }

  if(expr == "") {
    return false;
  }

  for (int i = expr.size() - 1; i >= 0; i--) {
    char c = expr.at(i);
    if (c == '[') {
      bracketDepth--;
      if (bracketDepth < 0) {
        cout << "Error: too many '[' without a matching ']'" << endl;
        return false;
      }
    } else if (c == ']') {
      bracketDepth++;
    } else if (c == '+' && bracketDepth == 0 && parenDepth == 0) {
      plusIdx = i;
      break;
    } else if (c == '-' && bracketDepth == 0 && parenDepth == 0) {
      //cout << "AT MINUS " << endl;
      //cout << evalMinusAsSub(expr, i) << endl;
      if (pUtil.evalMinusAsSub(expr, i)) {
        minusIdx = i;
        break;
      }
    } else if (c == ' ') {

    } else if (c == '(') {
      parenDepth --;
      if (parenDepth < 0) {
        cout << "Error: too many '(' without a matching ')'" << endl;
        return false;
      }
    } else if (c == ')') {
      parenDepth++;
    }
  }

  if (bracketDepth > 0) {
    cout << "Error: too many ']' without a matching '['" << endl;
    return false;
  }

  if (parenDepth > 0) {
    cout << "Error: too many ')' without a matching '('" << endl;
    return false;
  }

  if (plusIdx >= 0) {
    string lExp= expr.substr(0, plusIdx);
    string rTerm = expr.substr(plusIdx + 1);
    success = parseAdd(lExp, rTerm, out);
  } else if (minusIdx >= 0) {
    string lExp = expr.substr(0, minusIdx);
    string rTerm = expr.substr(minusIdx + 1);
    success = parseSub(lExp, rTerm, out);
  } else {
    success = parseTerm(expr, out);
  }
  return success;
}

bool Parser::parseTerm (string term, string &out) {
  bool success = false;
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
    return parseMul(lTerm, rId, out);
  } else {
    return parseId(term, out);
  }

}

bool Parser::parseId(string id, string &out) {

  if (debug) { cout << "ID " << id << endl; }

  if (pUtil.isNum(id)) {
    return parseNum(id, out);
  } else if (pUtil.isVariableLookup(id)){
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
    return parseDeref(beforeArray, idxExp, out);

  } else if (pUtil.isParenExp(id)) {
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
    return parseExp(insideParen, out);

  } else {
    cout << "Error: cannot resolve " << id << "." << endl;
    return false;
  }
}

bool Parser::parseAdd(string lExp, string rTerm, string &out) {
  //if (debug) {cout << "PARSING: " << lExp << " PLUS " << rTerm << endl;}
  bool success = true;
  string lOut = "", rOut = "";
  out = "";
  out += "; " + lExp+ " + " + rTerm + "\r\n";
  success = success && parseTerm(rTerm, rOut);
  out += rOut;
  out += "push ebx\r\n";
  out += "mov ebx, eax\r\n";
  success = success && parseExp(lExp, lOut);
  out += lOut;
  out += "add eax, ebx\r\n";
  out += "pop ebx\r\n";
  return success;
}

bool Parser::parseSub(string lExp, string rTerm, string &out) {
  string rOut = "", lOut = "";
  bool success = true;
  out = "";
  out += "; " + lExp + " - " + rTerm + "\r\n";
  success = success && parseTerm(rTerm, rOut);
  out += rOut;
  out += "push ebx\r\n";
  out += "mov ebx, eax\r\n";
  success = success && parseExp(lExp, lOut);
  out += lOut;
  out += "sub eax, ebx\r\n";
  out += "pop ebx\r\n";
  return success;
}

bool Parser::parseMul(string lTerm, string rId, string &out) {
  string lOut = "", rOut = "";
  bool success = true;
  out = "";
  out += "; " + lTerm + " * " + rId + "\r\n";
  success = success && parseId(rId, rOut);
  out += rOut;
  out += "push ebx\r\n";
  out += "mov ebx, eax\r\n";
  success = success && parseTerm(lTerm, lOut);
  out += lOut;
  out += "imul eax, ebx\r\n";
  out += "pop ebx\r\n";
  return success;
}

bool Parser::parseDeref(string arrayName, string indexExp, string &out) {
  bool success = true;
  string pOut = "";
  out = "";
  out += "; " + arrayName + "[ " + indexExp + " ]\r\n";
  out += "; NOTE: DEREFERENCING IS NOT YET IMPLEMENTED\r\n";
  out += "; This is currently evaluated as: (" + indexExp + ")\r\n";
  success = parseExp(indexExp, pOut);
  out += pOut;
  return success;
}

bool Parser::parseNum(string number, string &out) {
  if(debug) {cout << "NUMBER " << number << endl;}
  out = "";
  out += "; assignment\r\n";
  out += "mov eax, " + number + "\r\n";
  return true;
}
