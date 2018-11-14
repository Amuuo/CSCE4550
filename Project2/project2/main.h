#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<exception>
#include<algorithm>
#include<vector>
#include<iomanip>
#include<array>
#include"FileStream.h"
using namespace std;
using matrixVector = vector<array<array<uint8_t, 4>, 4>>;

template<class T>
T userInput(const char*);

string fileToString(string);
string vigenereSubstitution(string&, string&);
void   printRow(string&, int, char, FileStream<ofstream>&);
bool   binaryHasOddOnes(uint8_t, int = 0);
matrixVector stringToMatrixVec(string&);
void   printBlocks(matrixVector&, FileStream<ofstream>&, char='\0');