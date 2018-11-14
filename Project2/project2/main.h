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

template<class T>
void sendToOutputs(T, FileStream<ofstream>&);


matrixVector stringToMatrixVec(string);
string       fileToString(string);
string       vigenereSubstitution(string&, string&, FileStream<ofstream>&);
bool         binaryHasOddOnes(uint8_t, int = 0);
string       printBlocks(matrixVector&, bool=false);
uint8_t      mixRow(uint8_t, int);

