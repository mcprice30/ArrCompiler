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

  // ===========================
  // ==    Driver Function    ==
  // ===========================

  // Function: compileExp
  // Inputs:   expr - an expression to compile.
  // Outputs:  The compiled expression, or a statement saying that the expression
  //           cannot be compiled.
  string compileExp (string expr);

  // =======================================
  // ==     Primary Grammar Parsing       ==
  // =======================================

  // Function:  parseExp
  // Inputs:    expr - an expression to parse. Expressions are terms separated by
  //            + or - operators.
  // Outputs:   Assembly for the expression, using recursive descent.
  bool parseExp (string expr, string &out);

  // Function:  parseTerm
  // Inputs:    term - a term to parse. Terms are ids separated by * operators.
  // Outputs:   Assembly for the term, using recursive descent.
  bool parseTerm (string term, string &out);

  // Function:  parseId
  // Inputs:    id - an id to parse. Ids are numbers, array accesses, or
  //            expressions in parentheses.
  // Outputs:   Assembly for the id, using recursive descent.
  bool parseId(string id, string &out);

  //============================
  //==    Operation Parsing   ==
  //============================

  // Function:  parseAdd
  // Inputs:    lExp - The expression on the left side of the '+' operator.
  //            rExp  - The term on the right side of the '+' operator.
  // Outputs:   Assembly for the sum, using recursive descent.
  bool parseAdd(string lExp, string rTerm, string &out);

  // Function:  parseSub
  // Inputs:    lExp - The expression on the left side of the '-' operator.
  //            rTerm - The term on the right side of the '-' operator.
  // Outputs:   Assembly for the subtraction, using recursive descent.
  bool parseSub(string lExp, string rTerm, string &out);

  // Function:  parseMul
  // Inputs:    lTerm   - The term on the left side of the '*' operator.
  //            rId - The id on the right side of the '*' operator.
  // Outputs:   Assembly for the multiplication, using recursive descent.
  bool parseMul(string lTerm, string rId, string &out);

  // Function:  parseDeref
  // Inputs:    arrayName - The name of the array to access.
  //            indexExp  - Expression that evaluates to the array index.
  // Outputs:   Assembly for the deference.
  // TODO: Finish this function. parseDeref currently evaluates this as
  //       (indexExp) rather than deferencing the array.
  bool parseDeref(string arrayName, string indexExp, string &out);

  // Function: parseNum
  // Inputs:   number - A string to parse as a number.
  // Outputs:  Assembly for a number literal.
  bool parseNum(string number, string &out);

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
