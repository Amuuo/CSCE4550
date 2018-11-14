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

#include "main.h"




int main() {  

  string plaintext = fileToString(userInput<string>("Enter plaintext filename: "));
  string cipher    = fileToString(userInput<string>("Enter ciphertext filename: "));  
  string subText   = vigenereSubstitution(plaintext, cipher);  
  
  FileStream<ofstream> outputFile { userInput<string>("Enter output filename: ") };  

  
  cout << "ciphertext: " << cipher    << endl;
  cout << "plaintext: "  << plaintext << endl;  
  cout << "subText: "    << subText   << endl << endl;
  

  for ( auto i = 0; i<subText.size()%16; ++i )
    subText.push_back('A');      
    
  matrixVector shiftedMatrix = stringToMatrixVec(subText);
  
  printBlocks(shiftedMatrix, outputFile);

  int shift=0;
  for ( auto& block : shiftedMatrix ) {        
    for ( auto& row : block ) 
      rotate(row.begin(), row.begin() + shift++, row.end());
    shift = 0;
  }
  
  printBlocks(shiftedMatrix, outputFile);

  matrixVector parityMatrix = shiftedMatrix;

  for ( auto& block : parityMatrix ) 
    for ( auto& row : block ) 
      for ( auto& byte : row )         
        byte = binaryHasOddOnes(byte) ? byte ^= 0x80 : byte;        

     
  printBlocks(parityMatrix, outputFile, ' ');



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
  
  while ( in_stream.stream.get(character) ) 
    if ( isalpha(character) ) 
      outputString.push_back(character);    
  
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
bool binaryHasOddOnes(uint8_t shiftedTextChar, int i) {
      
  for ( i = 0; shiftedTextChar != 0; ++i ) 
    shiftedTextChar &= ( shiftedTextChar - 1 );
  
  return i % 2 != 0;
}


//---------------------------------
//        stringToMatrixVec
//---------------------------------
matrixVector stringToMatrixVec(string & text) {

  matrixVector tempMatrixVec;
  
  for ( auto i = 0, blockNum=0; i < text.size(); ++blockNum) {
    array<array<uint8_t, 4>, 4> tempArray;
    tempMatrixVec.push_back(tempArray);    
    for ( auto j = 0; j < 4; ++j)       
      for ( auto k = 0; k < 4; ++k, ++i ) 
        tempMatrixVec[blockNum][j][k] = static_cast<uint8_t>(text[i]);
  }
  return tempMatrixVec;
}


//---------------------------------
//           printBlocks
//---------------------------------
void printBlocks(matrixVector & matrix, FileStream<ofstream>& outStream, char separator) {
  
  if ( separator == ' ' ) outStream.stream << hex;

  for ( auto& block : matrix ) {
    for ( auto& row : block ) {
      for ( auto& byte : row ) {
        outStream.stream << byte;
      }outStream.stream << endl;
    }outStream.stream << endl;
  }

  if ( separator == ' ' ) outStream.stream << dec;
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
