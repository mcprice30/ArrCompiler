#include "StmtParser.h"

using namespace std;

StmtParser::StmtParser(bool debugSwitch) {
  debug = debugSwitch;
  expParser = ExpParser(debugSwitch);
  parseUtil = ParseUtil();
  loopCount = 1;
}

bool StmtParser::parseStmtList(string text, string &out) {
  int parenDepth = 0, semiIdx = -1;

  for (int i = text.size() - 1; i >= 0; i--) {
    char c = text.at(i);
    if (c == ')') {
      parenDepth ++;
    } else if (c == '(') {
      parenDepth --;
    } else if (c == ';' && parenDepth == 0) {
      semiIdx = i;
      break;
    }
  }

  if (semiIdx >= 0) {
    string sout = "", slout = "";
    string stmt = text.substr(semiIdx + 1);
    string leftList = text.substr(0, semiIdx);
    bool success = parseStmt(stmt, sout);
    if (success) {
      success = success && parseStmtList(leftList, slout);
      out = slout;
      out += "\r\n;Next Statement\r\n";
      out += sout;
      return success;
    } else {
      return false;
    }
  } else {
    return parseStmt(text, out);
  }
  return false;
}

bool StmtParser::parseStmt(string text, string &out) {
  int beginIdx = -1;
  for (int i = 0; i < text.size(); i++) {
    if (text.at(i) != ' ' && text.at(i) != '\r' && text.at(i) != '\n') {
      beginIdx = i;
      break;
    }
  }

  if (beginIdx < 0) {
    cout << "Error: statements cannot be empty." << endl;
    return false;
  }

  if (text.find("print ") == beginIdx) {
    string directive = "print ";
    string aExp = text.substr(beginIdx + directive.size());
    return parsePrint(aExp, out);
  } else if (text.find("printchar ") == beginIdx) {
    string directive = "printchar ";
    string aExp = text.substr(beginIdx + directive.size());
    return parsePrintChar(aExp, out);
  } else if (text.find("for ") == beginIdx) {

  } else if (text.at(beginIdx) == '(') {
    int closeIdx = -1;
    for (int i = text.size()-1; i >= 0; i--) {
      if (text.at(i) == ')') {
        closeIdx = i;
        break;
      } else if (text.at(i) != ' ' && text.at(i) != '\r' && text.at(i) != '\n') {
        cout << "Error: statements must lie inside parentheses." << endl;
        return false;
      }
    }

    if (closeIdx == -1) {
      cout << "Error: too many '(' without a matching ')'." << endl;
      return false;
    }

    string stmtList = text.substr(beginIdx + 1, closeIdx - beginIdx - 1);
    return parseStmtList(stmtList, out);

  } else { // assignment.

  }

  return false;
}

bool StmtParser::parsePrint(string text, string &out) {
  string evalResult = "";
  bool result = expParser.parseExp(text, evalResult);
  out = evalResult;
  out += "; TODO: call print function on eax.\r\n";
  return result;
}

bool StmtParser::parsePrintChar(string text, string &out) {
  string evalResult = "";
  bool result = expParser.parseExp(text, evalResult);
  out = evalResult;
  out += "; TODO: call print char function on eax.\r\n";
  return result;
}

bool StmtParser::parseForLoop(string text, string &out) {
  // for x[exp1] := exp2 to exp3 do S
  // 01234567890123456789012345678901
  // 00000000001111111111222222222233
  string arrName, exp_1, exp_2, exp_3, stmt;
  int arrNameEnd = -1, bracketDepth = 0, idxExpEnd = -1, assignOpIdx, toIdx, doIdx;

  for (int i = 4; i < text.size(); i++) {
    if (text.at(i) == '[') {
      arrNameEnd = i;
      break;
    }
  }

  if (arrNameEnd < 0) {
    cout << "Error: no array in first part of for loop!" << endl;
    return false;
  }

  for (int i = arrNameEnd; i < text.size(); i++) {
    if (text.at(i) == '[') {
      bracketDepth++;
    } else if (text.at(i) == ']') {
      bracketDepth--;
      if (bracketDepth == 0) {
        idxExpEnd = i;
        break;
      } else if (bracketDepth < 0) {
        cout << "Error: too many ']' without a matching '['." << endl;
        return false;
      }
    } else if (text.at(i) == ':' && i < text.size()-1 && text.at(i+1) == '=') {
      cout << "Error: cannot have assignment statement as array index." << endl;
      return false;
    }
  }

  if (idxExpEnd < 0) {
    cout << "Error: no matching ']'." << endl;
    return false;
  }

  assignOpIdx = text.find(":=");
  toIdx = text.find(" to ");
  doIdx = text.find(" do ");

  if (assignOpIdx == string::npos) {
    cout << "Error: no assignment statement." << endl;
    return false;
  } else if (toIdx == string::npos) {
    cout << "Error: missing \"to\" keyword." << endl;
    return false;
  } else if (doIdx == string::npos) {
    cout << "Error: missing \"do\" keyword." << endl;
    return false;
  }

  if (toIdx != text.find(" to ", assignOpIdx)) {
    cout << "Error: missing assignment before \"to\" operator." << endl;
    return false;
  } else if (doIdx != text.find(" do ", toIdx)) {
    cout << "Error: \"do\" keyword appears before \"to\" keyword." << endl;
    return false;
  }

  int forStart = text.find("for");

  arrName = text.substr(forStart + 3, arrNameEnd - forStart - 4);
  exp_1 = text.substr(arrNameEnd + 1, idxExpEnd - arrNameEnd - 2);
  exp_2 = text.substr(assignOpIdx + 2, toIdx - assignOpIdx - 3);
  exp_3 = text.substr(toIdx + 4, doIdx - toIdx - 5);
  stmt = text.substr(doIdx + 4);

  if (debug) {
    cout << "FOR: " << arrName << " at " << exp_1 << endl;
    cout << "FROM: " << exp_2 << " to " << exp_3 << endl;
    cout << "DO: " << stmt << endl;
  }

  bool compiled = true;

  stringstream ss;
  ss << loopCount++;
  string loopName = ss.str();
  out += "; beginning of for loop";
  string evalResult = "";
  out += "push ecx\r\nmov ecx, 0\r\n";
  out += "top_" + loopName + ": nop\r\n";
  compiled = compiled && expParser.parseExp(exp_3, evalResult);
  out += evalResult;
  out += "push ebx\r\n";
  out += "mov eax, ebx\r\n";
  compiled = compiled && expParser.parseExp(exp_2, evalResult);
  out += evalResult;
  out += "add eax, ecx\r\ninc ecx\r\n";
  out += "cmp eax, ebx\r\npop ebx\r\n";
  out += "jge done_" + loopName + "\r\n";
  out += "; TODO: store eax in x [eval index]\r\n";
  out += "; TODO: eval statement inside loop\r\n";
  out += "jmp top_" + loopName + "\r\n";
  out += "done_" + loopName + ": pop ecx\r\n";
  return compiled;
}
