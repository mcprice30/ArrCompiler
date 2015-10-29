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
  // Inputs:    lTerm - The term on the left-hand side of the '+' operator.
  //            rExp  - The expression on the right side of the '+' operator.
  // Outputs:   Assembly for the sum, using recursive descent.
  string parseAdd(string lTerm, string rExp);

  // Function:  parseSub
  // Inputs:    lTerm - The term on the left-hand side of the '-' operator.
  //            rExp  - The expression on the right side of the '-' operator.
  // Outputs:   Assembly for the subtraction, using recursive descent.
  string parseSub(string lTerm, string rExp);

  // Function:  parseMul
  // Inputs:    lId   - The id on the left-hand side of the '*' operator.
  //            rTerm - The term on the right side of the '*' operator.
  // Outputs:   Assembly for the multiplication, using recursive descent.
  string parseMul(string lId, string rTerm);

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
};
