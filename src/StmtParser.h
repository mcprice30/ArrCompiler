#ifndef STMT_PARSER_H
#define STMT_PARSER_H

#include "ExpParser.h"
#include "ParseUtil.h"

#include <iostream>
#include <string>

class StmtParser {
private:
  bool debug;
  ExpParser expParser;
  ParseUtil parseUtil;

public:
  // Constructor for StmtParser
  // Inputs: debugSwitch - True to enable debuging printouts, false otherwise.
  StmtParser(bool debugSwitch);

  // Function: parseStmtList
  // Inputs:   text - The list of statements to parse.
  // Outputs:  true if compilation succeeds without error, false otherwise.
  //           out - contains the compiled assembly for the statements passed in.
  bool parseStmtList(std::string text, std::string &out);

  // Function: parseStmt
  // Inputs:   text - The statement to parse.
  // Outputs:  true if compilation succeeds without error, false otherwise.
  //           out - contains the compiled assembly for the statments passed in.
  bool parseStmt(std::string text, std::string &out);

  // Function: parsePrint
  // Inputs:   text - The expression to be printed.
  // Outputs:  true if the expression is valid, false otherwise.
  //           out - contains the compiled assembly for the print statement.
  bool parsePrint(std::string text, std::string &out);

  // Function: parsePrintChar
  // Inputs:   text - The expression to be printed as an ASCII char.
  // Outputs:  true if the expression is valid, false otherwise.
  //           out - contains the compiled assembly for the print statement.
  bool parsePrintChar(std::string text, std::string &out);

};

#endif
