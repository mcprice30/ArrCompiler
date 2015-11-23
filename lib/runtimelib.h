#ifndef RUNTIMELIB_H
#define RUNTIMELIB_H

#include <map>
#include <iostream>


std::map<int, std::map<int, int> > vals;

extern "C"
int getValue(int arrId, int idx);

extern "C"
void setValue(int val, int idx, int arrId);

extern "C"
void print(int val);

extern "C"
void printChar(int val);


#endif
