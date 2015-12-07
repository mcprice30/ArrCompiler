#ifndef PROTOTYPES_H
#define PROTOTYPES_H

#include <string>

std::string definitions_include = "\n"
"extern \"C\"\n"
"int32_t getValue(int32_t arrId, int32_t idx) {\n"
"  if (vals.find(arrId) == vals.end()) {\n"
"    return 0;\n"
"  } else if ( (vals[arrId]).find(idx) == (vals[arrId]).end()) {\n"
"    return 0;\n"
"  } else {\n"
"    return vals[arrId][idx];\n"
"  }\n"
"}\n"

"extern \"C\"\n"
"void setValue(int32_t val, int32_t idx, int32_t arrId) {\n"
"  vals[arrId][idx] = val;\n"
"}\n"

"extern \"C\"\n"
"void print(int32_t val) {\n"
"  std::cout << val;\n"
"}\n"

"extern \"C\"\n"
"void printChar(int32_t val) {\n"
"  std::cout << (char) val;\n"
"}\n";

std::string prototypes_include = "#include <map>\n"
"#include <iostream>\n"
"#include <stdint.h>\n"

"std::map<int32_t, std::map<int32_t, int32_t> > vals;\n"

"extern \"C\"\n"
"int32_t getValue(int32_t arrId, int32_t idx);\n"

"extern \"C\"\n"
"void setValue(int32_t val, int32_t idx, int32_t arrId);\n"

"extern \"C\"\n"
"void print(int32_t val);\n"

"extern \"C\"\n"
"void printChar(int32_t val);\n";


#endif
