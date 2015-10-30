#include "Parser.h"

Parser::Parser(bool debugSwitch) {
  debug = debugSwitch;
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
      if (evalMinusAsSub(expr, i)) {
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

  if (isNum(id)) {
    return parseNum(id, out);
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
    return parseDeref(beforeArray, idxExp, out);

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
  bool hasMinus = false;

  for (int i = 0; i < text.size(); i++) {
    char c = text.at(i);

    if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5'
                 || c == '6' || c == '7' || c == '8' || c == '9' ) {

      hasNum = true;
    } else if (c == '-') {
      if (hasMinus || hasNum) {
        return false;
      } else {
        hasMinus = true;
      }
    } else if (c == ' ') {
      if (hasNum) {
        return false;
      }
    } else {
      return false;
    }
  }

  if (!hasNum) {
    cout << "Error: '" << text << "' cannot be resolved to an integer." << endl;
    return false;
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
  bool lFound = false;
  bool rFound = false;
  for (int i = 0; i < text.size(); i++) {
    char c = text.at(i);
    if (c == '(') {
      lFound = true;
      break;
    } else if (c == ' ') {

    } else {
      return false;
    }
  }

  for (int i = text.size() - 1; i >= 0; i--) {
    char c = text.at(i);
    if (c == ')') {
      rFound = true;
      break;
    } else if (c == ' ') {

    } else {
      return false;
    }
  }
  return (lFound && rFound);
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
