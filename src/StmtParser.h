#ifndef STMT_PARSER_H
#define STMT_PARSER_H

#include "ExpParser.h"
#include "ParseUtil.h"

#include <sstream>
#include <iostream>
#include <string>

class StmtParser {
private:
  bool debug;
  ExpParser expParser;
  ParseUtil parseUtil;
  int loopCount;

public:
  // Constructor for StmtParser
  // Inputs: debugSwitch - True to enable debuging printouts, false otherwise.
  StmtParser(bool debugSwitch);


  // Function: compile
  // Inputs:   stmtList - The statement list (start symbol) to compile
  // Outputs:  The compiled assembly for the statements list.
  std::string compile(std::string stmtList);

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

  // Function: parseForLoop
  // Inputs:   text - The source code of the for loop.
  // Outputs:  true if the for statement is valid, false otherwise.
  //           out - contains the compiled assembly for the for loop.
  bool parseForLoop(std::string text, std::string &out);

  // Function: parseAssignment
  // Inputs:   text - The source code of the assignment statement.
  // Outputs:  true if the assignment statement is valid, false otherwise.
  //           out - contains the compiled asssembly for the assignment.
  bool parseAssignment(std::string text, std::string &out);

};

#endif
