#include "ParseUtil.h"

using namespace std;

bool ParseUtil::isWhiteSpace(string text) {
  for (int i = 0; i < text.size(); i++) {
    if (text.at(i) != ' ') {
      return false;
    }
  }
  return true;
}

bool ParseUtil::isNum(string text) {
  bool hasNum = false;
  bool hasMinus = false;

  if (isCharLiteral(text)) {
    return true;
  }

  for (int i = 0; i < text.size(); i++) {
    char c = text.at(i);
    char n = ' ';
    if (i < text.size() - 1) {
      n = text.at(i + 1);
    }

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
      if (hasNum && i < text.size() - 1 && (n == '0' || n == '1' || n == '3' ||
            n == '4' || n == '5' || n == '6' || n == '7' || n == '8' || n == '9')) {
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

bool ParseUtil::evalMinusAsSub(string text, int idx) {
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

bool ParseUtil::isParenExp(string text) {
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

bool ParseUtil::isVariableLookup(string text) {
  for (int i = text.size() - 1; i >= 0; i--) {
    char c = text.at(i);
    if (c == ']') {
      break;
    } else if (c == ' ' || c == '\r' || c == '\n') {

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

bool ParseUtil::isValidVarName(string text) {
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

  string varName = test.substr(start, 1 + end - start);

  if (varName == "for") {
    return false;
  } else if (varName == "print") {
    return false;
  } else if (varName == "printchar") {
    return false;
  } else if (varName == "to") {
    return false;
  } else if (varName == "do") {
    return false;
  }

  return true;
}

bool ParseUtil::isCharLiteral(string text) {
  int openQuoteIdx = -1;
  for (int i = 0; i < text.size(); i++) {
    if (text.at(i) == '\'') {
      openQuoteIdx = i;
      break;
    } else if (text.at(i) == ' ') {

    } else {
      return false;
    }
  }

  int charIdx = openQuoteIdx + 1, closeQuoteIdx = 0;
  if (charIdx < text.size()) {
    if (text.at(charIdx) == '\\') {
      closeQuoteIdx = charIdx + 2;
    } else {
      closeQuoteIdx = charIdx + 1;
    }
  } else {
    return false;
  }

  if (closeQuoteIdx < text.size()) {
    if (text.at(closeQuoteIdx) != '\'') {
      return false;
    }
  } else {
    return false;
  }

  for (int i = closeQuoteIdx + 1; i < text.size(); i++) {
    if (text.at(i) != ' ') {
      return false;
    }
  }

  return true;

}

string ParseUtil::trim(string text) {
  int start = 0, end = -1;
  bool started = false;
  for (int i = 0; i < text.size(); i++) {
    if (!started && text.at(i) != ' ') {
      start = i;
      end = i;
      started = true;
    } else if (started && text.at(i) != ' '){
      end = i;
    }
  }
  //cout << "TRIMMING \"" << text << "\" to \""  << text.substr(start, end-start+1) << "\"" << endl;
  return text.substr(start, end-start+1);
}
