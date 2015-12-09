# ArrCompiler
This is a basic compiler for the Arr Language, version 1.0.0.

## Install and Setup
To get this repository, simply call:
`git clone https://github.com/mcprice30/ArrCompiler.git`

You will also need the Microsoft C/C++ optimizing compiler installed and on your path.
Finally, you will need the following environment variables set:

* INCLUDE must contain a Visual C++ include directory, such as `C:/Program Files (x86)/Microsoft Visual Studio 12.0/VC/include`
* LIB must have a Visual C++ library directory, such as `C:/Program Files (x86)/Microsoft Visual Studio 12.0/VC/lib`
* LIB must also contain a Windows SDK library directory, such as `C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib`

## Build and Test
To build the Arr compiler, simply run `make`.
If you wish to build and run the Arr compiler's internal tests, you may do this
by running `make tests` to build the tests and `./tst` to run the test executable. Finally, you may clean the build using `make clean`.

## Using the Arr compiler
The Arr compiler is used by calling `arr <filename> [flags]`. It is recommended
that Arr source files be given the .arr extension. By default, an executable of
matching name will be generated. For example, foo.arr will compile to foo.exe.
The following are flags for the Arr compiler:

* -verbose - Enables verbose printout during compilation.
* -asm - Keep generated assembly (stored in a .cpp file).
* -noexe - Only generate assembly. Do not produce executable.
* -help - See usage and flags.
* -version - See current version.
Note that the filename must be provided before any flags.
