#include "StmtParser.h"
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {

  if (argc == 1) { // Print compiler info and end.
    cout << "Arr compiler - Beta" << endl;
    return 0;
  }

  string filename = string (argv[1]);

  int start = 2;

  if (filename.at(0) == '-') {
    start = 1;
  }

  bool verbose = false, keepAsm = false, noexe = false, eEgg = false, help = false,
  version = false;

  for (int i = start; i < argc; i++) {
    string flag = string(argv[i]);
    if (flag == "-verbose") {
      verbose = true;
    } else if (flag == "-asm") {
      keepAsm = true;
    } else if (flag == "-noexe") {
      noexe = true;
    } else if (flag == "-X") {
      eEgg = true;
    } else if (flag == "-help") {
      help = true;
    } else if (flag == "-version") {
      version = true;
    } else {
      cout << "Error - invalid flag: " << flag << endl;
    }
  }

  if (help) {
    cout << "Fill this in later." << endl;
  } else if (version) {
    cout << "Version something or another, i guess." << endl;
  } else  if (start > 1){
    StmtParser p = StmtParser(verbose);
    ifstream input;
    input.open(filename.c_str());
    string src = "", line = "";

    string outfile = filename.substr(0, filename.rfind(".")) + ".cpp";

    if (input.good()) {
      while (getline(input, line)) {
        src += line + " ";
      }
      input.close();

      ofstream output;
      output.open(outfile.c_str());
      output << p.compile(src);
      output.close();

      if (!noexe) {
        string asmCmd = "cl " + outfile ;
        system(asmCmd.c_str());
      }

    } else {
      cout << "Error: cannot open file: " << filename << endl;
    }

  } else {
    cout << "Error: no file name given." << endl;
  }

  return 0;
}
