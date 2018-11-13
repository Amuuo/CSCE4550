#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<exception>
#include<algorithm>
#include<vector>
#include"FileStream.h"
using namespace std;


template<class T> 
T userInput(const char*);

string fileToString(string);
string vigenereSubstitution(string&, string&);
void   printRow(string&, int, char, FileStream<ofstream>&);



//=================================
//              main
//=================================
int main() {

  string plaintextString = fileToString(userInput<string>("Enter plaintext filename: "));
  string cipherString = fileToString(userInput<string>("Enter ciphertext filename: "));  
  string substitutedPlaintext = vigenereSubstitution(plaintextString, cipherString);
  string substituedPlainTextUnshifted { substitutedPlaintext };
  FileStream<ofstream> outputFile { userInput<string>("Enter output filename: ") };  

  
  cout << "ciphertext: " << cipherString << endl;
  cout << "plaintext before vignere: " << plaintextString << endl;  
  cout << "plaintext after vignere: " << substitutedPlaintext << endl << endl;
  
  // add buffer to plaintext string
  for ( auto i = 0; i<substitutedPlaintext.size()%16; ++i ) {
    substitutedPlaintext.push_back('A');
  }  
  
  for ( auto i = substitutedPlaintext.begin(); i < substitutedPlaintext.end(); i += 16 ) {            
    rotate(i + 4, i + 5, i + 8);
    rotate(i + 8, i + 10, i + 12);
    rotate(i + 12, i + 15, i + 16);  
  }  
  
  for ( int i = 0; i < substitutedPlaintext.size(); i+=4) {    
    if ( i % 16 == 0 ) {
      cout << endl;
      outputFile.stream << endl;
    }    
    printRow(substituedPlainTextUnshifted, i, '\t', outputFile);    
    printRow(substitutedPlaintext, i, '\n', outputFile);    
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
    cout << text[ j ];
    outStream.stream << text[ j ];
  }
  cout << endChar;
  outStream.stream << endChar;
}


//---------------------------------
//      vigenereSubstitution
//---------------------------------
string vigenereSubstitution(string& plaintext, string& cipher) {
  string subString { plaintext };
  
  for ( int i = 0; i < plaintext.size(); ++i ) {
    int char1 = plaintext[ i ] - 'A';
    int char2 = cipher[ i%cipher.size() ] - 'A';
    subString[ i ] = ((char1+char2) % 26 ) + 'A';
  }
  return subString;
}
