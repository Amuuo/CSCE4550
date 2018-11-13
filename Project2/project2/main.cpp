/*
============================================================================
----------------------------------------------------------------------------
Name        : Project 2
Author      : Adam Williams
Version     : 1.0
Copyright   : 2018
Description : Program implements modified-AES style encryption
----------------------------------------------------------------------------
============================================================================
*/


#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<exception>
#include<algorithm>
#include<vector>
#include<iomanip>
#include"FileStream.h"
using namespace std;


template<class T> 
T userInput(const char*);

string fileToString(string);
string vigenereSubstitution(string&, string&);
void   printRow(string&, int, char, FileStream<ofstream>&);
bool   binaryHasOddOnes(int8_t);



//=================================
//              main
//=================================
int main() {

  string plaintextString = fileToString(userInput<string>("Enter plaintext filename: "));
  string cipherString = fileToString(userInput<string>("Enter ciphertext filename: "));  
  string substitutedPlaintext = vigenereSubstitution(plaintextString, cipherString);  
  FileStream<ofstream> outputFile { userInput<string>("Enter output filename: ") };  

  
  cout << "ciphertext: " << cipherString << endl;
  cout << "plaintext before vignere: " << plaintextString << endl;  
  cout << "plaintext after vignere: " << substitutedPlaintext << endl << endl;
  
  // add buffer to plaintext string
  for ( auto i = 0; i<substitutedPlaintext.size()%16; ++i ) {
    substitutedPlaintext.push_back('A');
  }  
  
  for ( auto i = substitutedPlaintext.begin(); i != substitutedPlaintext.end(); i += 16 ) {            
    rotate(i + 4, i + 5, i + 8);
    rotate(i + 8, i + 10, i + 12);
    rotate(i + 12, i + 15, i + 16);  
  }  
  
  string substituedPlainTextUnshifted { substitutedPlaintext };

  for ( int i = 0; i < substitutedPlaintext.size(); i+=4) {    
    if ( i % 16 == 0 ) {
      cout << endl;
      outputFile.stream << endl;
    }    
    printRow(substituedPlainTextUnshifted, i, '\t', outputFile);    
    printRow(substitutedPlaintext, i, '\n', outputFile);    
  }

  vector<uint8_t> parityBytes;

  for ( int i = 0; i < substitutedPlaintext.size(); ++i ) {
    parityBytes.push_back(substitutedPlaintext[i]);
    if ( binaryHasOddOnes(parityBytes[i])) {
      parityBytes[i] ^= 0x80;
    }
  }

  int counter = 0;
  for ( auto& i : parityBytes ) {    
    if ( counter % 4 == 0 ) cout << endl;
    //printf(" %2x", i);
    cout << " " << hex << static_cast<int>(i);
    ++counter;
  }


  return 0;
}




//---------------------------------
//           userInput
//---------------------------------
template<class T>
T userInput(const char* inputRequestPrompt) {
  
  T userInput;
  cout << inputRequestPrompt;
  cin >> userInput;
  return userInput;
}


//---------------------------------
//      fileContentsToString
//---------------------------------
string fileToString(string streamFilename) {
  
  FileStream<ifstream> in_stream { streamFilename };
  char character;
  string outputString;
  
  while ( in_stream.stream.get(character) ) {
    if ( isalpha(character) ) {
      outputString.push_back(character);
    }
  }
  return outputString;
}


//---------------------------------
//            printRow
//---------------------------------
void printRow(string & text, int position, char endChar, FileStream<ofstream>& outStream) {
  
  for ( int j = position; j < ( position + 4 ); ++j ) {
    cout << text[j];
    outStream.stream << text[j];
  }
  cout << endChar;
  outStream.stream << endChar;
}


//---------------------------------
//        binaryHasEvenOnes
//---------------------------------
bool binaryHasOddOnes(int8_t shiftedTextChar) {
  
  int i;
  
  for ( i = 0; shiftedTextChar != 0; ++i ) {
    shiftedTextChar &= ( shiftedTextChar - 1 );
  }
  return i % 2 != 0;
}


//---------------------------------
//      vigenereSubstitution
//---------------------------------
string vigenereSubstitution(string& plaintext, string& cipher) {
  
  string subString { plaintext };
  
  for ( int i = 0; i < plaintext.size(); ++i ) {
    int char1 = plaintext[i] - 'A';
    int char2 = cipher[i%cipher.size()] - 'A';
    subString[i] = ( ( char1 + char2 ) % 26 ) + 'A';
  }
  return subString;
}
