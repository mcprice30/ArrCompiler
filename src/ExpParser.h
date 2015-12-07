#ifndef PARSER_H
#define PARSER_H

#include "ParseUtil.h"

#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <set>
#include <map>

class ExpParser {

private:
  std::map<std::string, int>* arrNames;
  bool debug;
  ParseUtil pUtil;

public:

  // Constructor for Parser.
  // Inputs: debugSwitch - true to enable debugging statements, false otherwise.
  ExpParser(bool debugSwitch, std::map<std::string, int>* arrNames_in);

  ExpParser();
  // ===========================
  // ==    Driver Function    ==
  // ===========================

  // Function: compileExp
  // Inputs:   expr - an expression to compile.
  // Outputs:  The compiled expression, or a statement saying that the expression
  //           cannot be compiled.
  std::string compileExp (std::string expr);

  // =======================================
  // ==     Primary Grammar Parsing       ==
  // =======================================

  // Function:  parseExp
  // Inputs:    expr - an expression to parse. Expressions are terms separated by
  //            + or - operators.
  // Outputs:   Assembly for the expression, using recursive descent.
  bool parseExp (std::string expr, std::string &out);

  // Function:  parseTerm
  // Inputs:    term - a term to parse. Terms are ids separated by * operators.
  // Outputs:   Assembly for the term, using recursive descent.
  bool parseTerm (std::string term, std::string &out);

  // Function:  parseId
  // Inputs:    id - an id to parse. Ids are numbers, array accesses, or
  //            expressions in parentheses.
  // Outputs:   Assembly for the id, using recursive descent.
  bool parseId(std::string id, std::string &out);

  //============================
  //==    Operation Parsing   ==
  //============================

  // Function:  parseAdd
  // Inputs:    lExp - The expression on the left side of the '+' operator.
  //            rExp  - The term on the right side of the '+' operator.
  // Outputs:   Assembly for the sum, using recursive descent.
  bool parseAdd(std::string lExp, std::string rTerm, std::string &out);

  // Function:  parseSub
  // Inputs:    lExp - The expression on the left side of the '-' operator.
  //            rTerm - The term on the right side of the '-' operator.
  // Outputs:   Assembly for the subtraction, using recursive descent.
  bool parseSub(std::string lExp, std::string rTerm, std::string &out);

  // Function:  parseMul
  // Inputs:    lTerm   - The term on the left side of the '*' operator.
  //            rId - The id on the right side of the '*' operator.
  // Outputs:   Assembly for the multiplication, using recursive descent.
  bool parseMul(std::string lTerm, std::string rId, std::string &out);

  // Function:  parseDeref
  // Inputs:    arrayName - The name of the array to access.
  //            indexExp  - Expression that evaluates to the array index.
  // Outputs:   Assembly for the deference.
  // TODO: Finish this function. parseDeref currently evaluates this as
  //       (indexExp) rather than deferencing the array.
  bool parseDeref(std::string arrName, std::string indexExp, std::string &out);

  // Function: parseNum
  // Inputs:   number - A string to parse as a number.
  // Outputs:  Assembly for a number literal.
  bool parseNum(std::string number, std::string &out);

};

#endif
