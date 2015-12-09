#include "StmtParser.h"

#include "../lib/prototypes.h"

using namespace std;

StmtParser::StmtParser(bool debugSwitch) {
  debug = debugSwitch;
  arrNames = new map<string, int>();
  expParser = ExpParser(debugSwitch, arrNames);
  loopCount = 1;
}


string StmtParser::compile(string stmtList) {
  string output = "", main = "";
  bool result = parseStmtList(stmtList, main);

  if (result) {
    output += prototypes_include + "\n";
    output += "int main() {\n";
    output += "__asm{\n";
    output += main;
    output += "} return 0; }";
    output += definitions_include + "\n";
    return output;
  } else {
    return "Did not compile.";
  }
}

bool StmtParser::parseStmtList(string text, string &out) {
  int parenDepth = 0, semiIdx = -1;

  if (debug) {
    cout << "STMT LIST " << text << endl;
  }


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
      out += "; Next Statement\n";
      out += sout;
      return success;
    } else {
      cout << "Unable to parse statement." << endl;
      return false;
    }
  } else {
    return parseStmt(text, out);
  }
  cout << "How did I get here?" << endl;
  return false;
}

bool StmtParser::parseStmt(string text, string &out) {


  if (debug) {
    cout << "STMT " << text << endl;
  }

  int beginIdx = -1;
  for (int i = 0; i < text.size(); i++) {
    if (text.at(i) != ' ' && text.at(i) != '\r' && text.at(i) != '\n') {
      beginIdx = i;
      break;
    }
  }

  if (beginIdx < 0) { // parse empty statement
    //cout << "Error: statements cannot be empty." << endl;
    return true;
  }

  if (text.find("print ") == beginIdx) {      // print
    string directive = "print ";
    string aExp = text.substr(beginIdx + directive.size());
    return parsePrint(aExp, out);
  } else if (text.find("printchar ") == beginIdx) { //print char
    string directive = "printchar ";
    string aExp = text.substr(beginIdx + directive.size());
    return parsePrintChar(aExp, out);
  } else if (text.find("for ") == beginIdx) { // for loop
    return parseForLoop(text, out);
  } else if (text.at(beginIdx) == '(') {    // (<stmtList>)
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
    return parseAssignment(text, out);
  }
  cout << "How did I get here?" << endl;
  return false;
}

bool StmtParser::parsePrint(string text, string &out) {
  string evalResult = "";
  bool result = expParser.parseExp(text, evalResult);
  out = evalResult;
  out += "; Printing value\n";
  out += "push ecx\npush eax\ncall print\nadd esp, 4\npop ecx\n";
  //out += "; TODO: call print function on eax.\n";
  return result;
}

bool StmtParser::parsePrintChar(string text, string &out) {
  string evalResult = "";
  bool result = expParser.parseExp(text, evalResult);
  out = evalResult;
  out += "; Printing value\n";
  out += "push ecx\npush eax\ncall printChar\nadd esp, 4\npop ecx\n";
  //out += "; TODO: call print char function on eax.\n";
  return result;
}

bool StmtParser::parseForLoop(string text, string &out) {
  // for arrName[exp1] := exp2 to exp3 do stmt

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

  arrName = text.substr(forStart + 4, arrNameEnd - forStart - 4);
  exp_1 = text.substr(arrNameEnd + 1, idxExpEnd - arrNameEnd - 1);
  exp_2 = text.substr(assignOpIdx + 3, toIdx - assignOpIdx - 3);
  exp_3 = text.substr(toIdx + 4, doIdx - toIdx - 4);
  stmt = text.substr(doIdx + 4);
  arrName = parseUtil.trim(arrName);
  int array_id = 0;
  if (arrNames->find(arrName) != arrNames->end()) {
    array_id = (*arrNames)[arrName];
  } else {
    array_id = arrNames->size();
    (*arrNames)[arrName] = array_id;
  }

  if (debug) {
    //cout << "arrayName " << (forStart + 4) << " length " << (arrNameEnd - forStart - 4) << endl;
    //cout << "exp1 " << (arrNameEnd+1) << " length " << (idxExpEnd - arrNameEnd - 1) << endl;
    //cout << "exp2 " << (assignOpIdx + 3) << " length " << (toIdx - assignOpIdx - 3) << endl;
    //cout << "exp3 " << (toIdx + 4) << " length " << (doIdx - toIdx - 4) << endl;
    //cout << text << " " << text.substr(3, 1) << endl;
    cout << "FOR: " << arrName << " (id: " << array_id << ") at " << exp_1 << endl;
    cout << "FROM: " << exp_2 << " to " << exp_3 << endl;
    cout << "DO: " << stmt << endl;
  }

  bool compiled = true;

  stringstream ss, arrStream;
  ss << loopCount++;
  arrStream << array_id;
  string loopName = ss.str();
  out += "; beginning of for loop\n";
  string evalResult = "";
  out += "push ecx\nmov ecx, 0\n";
  out += "top_" + loopName + ": nop\n";
  compiled = compiled && expParser.parseExp(exp_3, evalResult);
  out += evalResult;
  out += "push ebx\n";
  out += "mov ebx, eax\n";
  compiled = compiled && expParser.parseExp(exp_2, evalResult);
  out += evalResult;
  out += "add eax, ecx\ninc ecx\n";
  out += "cmp eax, ebx\npop ebx\n";
  out += "jg done_" + loopName + "\n";
  out += "push ebx\n";
  out += "mov ebx, eax\n";
  compiled = compiled && expParser.parseExp(exp_1, evalResult);
  out += evalResult;
  out += "push ecx\n";
  out += "push " + arrStream.str() + "\npush eax \npush ebx\n";
  out += "call setValue\nadd esp, 12\n";
  out += "pop ecx\npop ebx\n";
  evalResult = "";
  compiled = compiled && parseStmt(stmt, evalResult);
  out += evalResult;
  out += "jmp top_" + loopName + "\n";
  out += "done_" + loopName + ": pop ecx\n";
  return compiled;
}


bool StmtParser::parseAssignment(string text, string &out) {
  int parenDepth = 0, bracketDepth = 0, assignOpIdx = -1, b1Idx = -1, b2Idx = -1;

  for (int i = 0; i < text.size()-1; i++) {
    char c = text.at(i);
    if (c == '[') {
      if (parenDepth == 0 && b1Idx < 0 && bracketDepth == 0) {
        b1Idx = i;
      }
      bracketDepth++;
    } else if (c == ']') {
      bracketDepth--;
      if (bracketDepth < 0) {
        cout << "Error: too many ']' without a matching '['" << endl;
        return false;
      }
      if (parenDepth == 0 && b2Idx < 0 && bracketDepth == 0) {
        b2Idx = i;
      }
    } else if (c == '(') {
      parenDepth++;
    } else if (c == ')') {
      parenDepth--;
      if (parenDepth < 0) {
        cout << "Error: too many ')' without a matching '('" << endl;
        return false;
      }
    } else if (c == ':' && text.at(i+1) == '=' && parenDepth == 0
              && bracketDepth == 0) {
      assignOpIdx = i;
      break;
    }
  }

  if (assignOpIdx < 0) {
    cout << "Error: no \":=\" operator found in assignment statement." << endl;
    return false;
  }

  if (b1Idx < 0 || b2Idx < 0 || b1Idx > assignOpIdx || b2Idx > assignOpIdx) {
    cout << "Error: no valid variable to right of \":=\"" << endl;
    return false;
  }

  for (int i = b2Idx + 1; i < assignOpIdx; i++) {
    char c = text.at(i);
    if (c != ' ' && c != '\r' && c != '\n') {
      cout << "Error: invalid character between variable and assignment."
           << endl;
      return false;
    }
  }

    //string lhs = text.substr(0, assignOpIdx);
  string arrName = text.substr(0, b1Idx);
  if (!parseUtil.isValidVarName(arrName)) {
    cout << "Error: cannot name array: " << arrName << endl;
    return false;
  }
  arrName = parseUtil.trim(arrName);

  int array_id = 0;
  if (arrNames->find(arrName) != arrNames->end()) {
    array_id = (*arrNames)[arrName];
  } else {
    array_id = arrNames->size();
    (*arrNames)[arrName] = array_id;
  }

  stringstream ss;
  ss << array_id;
  string idxExp = text.substr(b1Idx + 1, b2Idx - b1Idx - 1);
  string rhs = text.substr(assignOpIdx+2);
  string evalResult = "";
  bool compiled = true;
  out += "push ecx\n";
  out += "; assignment statement for: " + text + "\n";
  out += "; pushing array name\n";
  out += "push " + ss.str() + "\n";
  out += "; pushing index expression: " + idxExp + "\n";
  compiled = compiled && expParser.parseExp(idxExp, evalResult);
  out += evalResult;
  out += "push eax\n";
  out += "; pushing rhs expression: " + rhs + "\n";
  compiled = compiled && expParser.parseExp(rhs, evalResult);
  out += evalResult;
  out += "push eax\n";
  out += "call setValue\n";
  out += "add esp, 12\n";
  out += "pop ecx \n";
  return compiled;

}
