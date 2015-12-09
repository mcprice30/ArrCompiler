#include "StmtParser.h"
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {

  if (argc == 1) { // Print compiler info and end.
    cout << "Usage: arr <filename> [flags]" << endl;
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
      exit(1);
    }
  }

  if (help) {
    cout << "Usage: arr <filename> [flags]" << endl;
    cout << "    Flags:" << endl;
    cout << "-verbose - Enable verbose printout during compilation." << endl;
    cout << "-asm - Keep generated assembly (stored in a .cpp file)." << endl;
    cout << "-noexe - Only generate assembly. Do not compile to executable." << endl;
    cout << "-help - See usage and flags." << endl;
    cout << "-version - See current version." << endl;
  } else if (version) {
    cout << "Arr compiler - Version 1.0.0" << endl;
    cout << "(C) Mitchell C. Price and Jeffrey L. Overbey, December 2015" << endl;
  } else  if (start > 1){
    StmtParser p = StmtParser(verbose, eEgg);
    ifstream input;
    input.open(filename.c_str());
    string src = "", line = "";

    string fprefix = filename.substr(0, filename.rfind("."));

    string outfile = fprefix + ".cpp";
    string objfile = fprefix + ".obj";

    if (input.good()) {
      while (getline(input, line)) {
        src += line + " ";
      }
      input.close();

      string o;

      ofstream output;
      output.open(outfile.c_str());
      output << (o = p.compile(src));
      output.close();

      if (o == "Did not compile.") {
        string cleanCommand = "rm " + outfile;
        system(cleanCommand.c_str());
        exit(1);
      }

      if (!noexe) {
        string asmCmd = "cl " + outfile  + " > _ignore.txt 2>&1";
        system(asmCmd.c_str());
        string cleanCommand = "rm _ignore.txt " + objfile;
        system(cleanCommand.c_str());
      }

      if (!keepAsm && !noexe) {
        string cleanCommand = "rm " + outfile;
        system(cleanCommand.c_str());
      }

    } else {
      cout << "Error: cannot open file: " << filename << endl;
    }

  } else {
    cout << "Error: no file name given." << endl;
  }

  return 0;
}
