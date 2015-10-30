#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

class Parser {

private:
  bool debug;

public:

  // Constructor for Parser.
  // Inputs: debugSwitch - true to enable debugging statements, false otherwise.
  Parser(bool debugSwitch);

  // =======================================
  // ==     Primary Grammar Parsing       ==
  // =======================================

  // Function:  parseExp
  // Inputs:    expr - an expression to parse. Expresions are terms separated by
  //            + or - operators.
  // Outputs:   Assembly for the expression, using recursive descent.
  string parseExp (string expr);

  // Function:  parseTerm
  // Inputs:    term - a term to parse. Terms are ids separated by * operators.
  // Outputs:   Assembly for the term, using recursive descent.
  string parseTerm (string term);

  // Function:  parseId
  // Inputs:    id - an id to parse. Ids are numbers, array accesses, or
  //            expressions in parentheses.
  // Outputs:   Assembly for the id, using recursive descent.
  string parseId(string id);

  //============================
  //==    Operation Parsing   ==
  //============================

  // Function:  parseAdd
  // Inputs:    lExp - The expression on the left side of the '+' operator.
  //            rExp  - The term on the right side of the '+' operator.
  // Outputs:   Assembly for the sum, using recursive descent.
  string parseAdd(string lExp, string rTerm);

  // Function:  parseSub
  // Inputs:    lExp - The expression on the left side of the '-' operator.
  //            rTerm - The term on the right side of the '-' operator.
  // Outputs:   Assembly for the subtraction, using recursive descent.
  string parseSub(string lExp, string rTerm);

  // Function:  parseMul
  // Inputs:    lTerm   - The term on the left side of the '*' operator.
  //            rId - The id on the right side of the '*' operator.
  // Outputs:   Assembly for the multiplication, using recursive descent.
  string parseMul(string lTerm, string rId);

  // Function:  parseDeref
  // Inputs:    arrayName - The name of the array to access.
  //            indexExp  - Expression that evaluates to the array index.
  // Outputs:   Assembly for the deference.
  // TODO: Finish this function. parseDeref currently evaluates this as
  //       (indexExp) rather than deferencing the array.
  string parseDeref(string arrayName, string indexExp);

  // Function: parseNum
  // Inputs:   number - A string to parse as a number.
  // Outputs:  Assembly for a number literal.
  string parseNum(string number);

  //===========================
  //==   Utility Functions   ==
  //===========================

  // Function: isWhiteSpace
  // Inputs:   text - A string to test.
  // Outputs:  true if text contains only whitespace, false otherwise.
  bool isWhiteSpace(string text);

  // Function: isNum
  // Inputs:   text - A string to test.
  // Outputs:  true if text is a numerical value, false otherwise.
  bool isNum(string text);

  // Function: evalMinusAsSub
  // Inputs:   text - A string that the '-' character appears in.
  //           index - The index that the '-' character appears in.
  // Outputs:  true if this '-' operator denotes subtraction, false if it
  //           denotes multiplication by -1.
  bool evalMinusAsSub(string text, int index);

  // Function: isParenExp
  // Inputs:   text - A string to evaluate.
  // Outputs:  true if text is an expression surrounded by parentheses, false
  //           otherwise.
  bool isParenExp(string text);

  // Function: isVariableLookup
  // Inputs:   text - A string to evaluate.
  // Outputs:  true if text is a variable access, false otherwise.
  bool isVariableLookup(string text);

  // Function: isValidVarName
  // Inputs:   text - A string to evaluate.
  // Outputs:  true if text is a valid variable name, false otherwise.
  bool isValidVarName(string text);
};

#endif
