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
  //out += "; ERROR: for loop currently unsupported!\r\n";
  stringstream ss;
  ss << loopCount++;
  string loopName = ss.str();
  out += "; beginning of for loop";
  string evalResult = "";
  out += "push ecx\r\nmov ecx, 0\r\n";
  out += "top_" + loopName + ": nop\r\n";
  out += "; TODO: DA EXPRESSIONS\r\n";
  out += "push ebx\r\n";
  out += "mov eax, ebx\r\n";
  out += ": TODO: DA EXPRESSIONS 2\r\n";
  out += "add eax, ecx\r\ninc ecx\r\n";
  out += "cmp eax, ebx\r\npop ebx\r\n";
  out += "jge done_" + loopName + "\r\n";
  out += "; TODO: store eax in x [eval index]\r\n";
  out += "; TODO: eval statement inside loop\r\n";
  out += "jmp top_" + loopName + "\r\n";
  out += "done_" + loopName + "; pop ecx\r\n";
  return true;
}
