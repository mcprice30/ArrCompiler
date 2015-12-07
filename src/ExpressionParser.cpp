#include "ExpParser.h"

using namespace std;

ExpParser::ExpParser(bool debugSwitch, map<string, int>* arrNames_in) {
  debug = debugSwitch;
  pUtil = ParseUtil();
  arrNames = arrNames_in;
}

ExpParser::ExpParser() {
  debug = false;
  pUtil = ParseUtil();
}

string ExpParser::compileExp(string expr) {
  string output = "";
  bool result = parseExp(expr, output);

  if (result) {
    return output;
  } else {
    return "Did not compile.";
  }
}

bool ExpParser::parseExp (string expr, string &out) {
  bool success = false;
  int plusIdx = -1, minusIdx = -1, bracketDepth = 0, parenDepth = 0;
  bool prevTokenOp = true;
  if (debug) { cout << "EXPR " << expr << endl; }

  if(expr == "") {
    cout << "Error: empty expression." << endl;
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

bool ExpParser::parseTerm (string term, string &out) {
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

bool ExpParser::parseId(string id, string &out) {

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

bool ExpParser::parseAdd(string lExp, string rTerm, string &out) {
  //if (debug) {cout << "PARSING: " << lExp << " PLUS " << rTerm << endl;}
  bool success = true;
  string lOut = "", rOut = "";
  out = "";
  out += "; " + lExp+ " + " + rTerm + "\n";
  success = success && parseTerm(rTerm, rOut);
  out += rOut;
  out += "push ebx\n";
  out += "mov ebx, eax\n";
  success = success && parseExp(lExp, lOut);
  out += lOut;
  out += "add eax, ebx\n";
  out += "pop ebx\n";
  return success;
}

bool ExpParser::parseSub(string lExp, string rTerm, string &out) {
  string rOut = "", lOut = "";
  bool success = true;
  out = "";
  out += "; " + lExp + " - " + rTerm + "\n";
  success = success && parseTerm(rTerm, rOut);
  out += rOut;
  out += "push ebx\n";
  out += "mov ebx, eax\n";
  success = success && parseExp(lExp, lOut);
  out += lOut;
  out += "sub eax, ebx\n";
  out += "pop ebx\n";
  return success;
}

bool ExpParser::parseMul(string lTerm, string rId, string &out) {
  string lOut = "", rOut = "";
  bool success = true;
  out = "";
  out += "; " + lTerm + " * " + rId + "\n";
  success = success && parseId(rId, rOut);
  out += rOut;
  out += "push ebx\n";
  out += "mov ebx, eax\n";
  success = success && parseTerm(lTerm, lOut);
  out += lOut;
  out += "imul eax, ebx\n";
  out += "pop ebx\n";
  return success;
}

bool ExpParser::parseDeref(string arrName, string indexExp, string &out) {
  bool success = true;
  string pOut = "";
  out = "";
  //out += "; " + arrayName + "[ " + indexExp + " ]\n";
  //out += "; NOTE: DEREFERENCING IS NOT YET IMPLEMENTED\n";
  //out += "; This is currently evaluated as: (" + indexExp + ")\n";
  success = parseExp(indexExp, pOut);
  out += pOut;
  arrName = pUtil.trim(arrName);
  int array_id = 0;
  if (arrNames->find(arrName) != arrNames->end()) {
    array_id = (*arrNames)[arrName];
  } else {
    array_id = arrNames->size();
    (*arrNames)[arrName] = array_id;
  }
  stringstream ss;
  ss << array_id;
  out += "; " + arrName + "[" + indexExp + " ]\n";
  out += "push ecx\n";
  out += "push eax\npush " + ss.str() + "\ncall getValue\n";
  out += "add esp, 8\npop ecx\n";
  return success;
}

bool ExpParser::parseNum(string number, string &out) {
  if(debug) {cout << "NUMBER " << number << endl;}
  out = "";
  out += "; assignment\n";
  out += "mov eax, " + number + "\n";
  return true;
}
