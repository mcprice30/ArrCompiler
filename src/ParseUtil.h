#ifndef PARSE_UTILS_H
#define PARSE_UTILS_H

#include <string>
#include <iostream>

class ParseUtil {

public:

  // Function: isWhiteSpace
  // Inputs:   text - A string to test.
  // Outputs:  true if text contains only whitespace, false otherwise.
  bool isWhiteSpace(std::string text);

  // Function: isNum
  // Inputs:   text - A string to test.
  // Outputs:  true if text is a numerical value, false otherwise.
  bool isNum(std::string text);

  // Function: evalMinusAsSub
  // Inputs:   text - A string that the '-' character appears in.
  //           index - The index that the '-' character appears in.
  // Outputs:  true if this '-' operator denotes subtraction, false if it
  //           denotes multiplication by -1.
  bool evalMinusAsSub(std::string text, int index);

  // Function: isParenExp
  // Inputs:   text - A string to evaluate.
  // Outputs:  true if text is an expression surrounded by parentheses, false
  //           otherwise.
  bool isParenExp(std::string text);

  // Function: isVariableLookup
  // Inputs:   text - A string to evaluate.
  // Outputs:  true if text is a variable access, false otherwise.
  bool isVariableLookup(std::string text);

  // Function: isValidVarName
  // Inputs:   text - A string to evaluate.
  // Outputs:  true if text is a valid variable name, false otherwise.
  bool isValidVarName(std::string text);

  // Function: isCharLiteral
  // Inputs:   text - A string to evaluate.
  // Outputs:  true if text is a char literal, false otherwise.
  bool isCharLiteral(std::string text);

  // Function: trim
  // Inputs:   A string, potentially with leading or following spaces.
  // Outputs:  The string, with leading and following spaces removed.
  std::string trim(std::string text);

};

#endif
