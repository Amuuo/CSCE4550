/*
============================================================================
Name        : Project 2
Author      : Adam Williams
Version     : 1.0
Copyright   : 2018
Description : Program implements modified-AES style encryption
============================================================================
*/

#include "main.h"




int main() {  

  string plaintext = fileToString(userInput<string>("Enter plaintext filename: "));
  string cipher    = fileToString(userInput<string>("Enter ciphertext filename: "));    
  FileStream<ofstream> outputFile { userInput<string>("Enter output filename: ") };  

  sendToOutputs<string>("ciphertext: " + cipher    + '\n',   outputFile);
  sendToOutputs<string>("plaintext: "  + plaintext + '\n',   outputFile);
  
  // substitution and padding  
  matrixVector shiftedMatrix = 
    stringToMatrixVec(vigenereSubstitution(plaintext, cipher, outputFile));
  

  sendToOutputs<string>("\nSubstitution Blocks:\n\n" + printBlocks(shiftedMatrix), outputFile);  

  // shiftRows
  int shift=0;
  for ( auto& block : shiftedMatrix ) {        
    for ( auto& row : block ) 
      rotate(row.begin(), row.begin() + shift++, row.end());
    shift = 0;
  }  
  sendToOutputs<string>("\nShift Rows:\n\n" + printBlocks(shiftedMatrix), outputFile);
  

  matrixVector parityMatrix = shiftedMatrix;

  // parity bit
  for ( auto& block : parityMatrix ) 
    for ( auto& row : block ) 
      for ( auto& byte : row )         
        byte = binaryHasOddOnes(byte) ? byte ^= 0x80 : byte;        
       
  sendToOutputs<string>("\nParity Bit:\n\n" + printBlocks(parityMatrix, true), outputFile);

  
  matrixVector finalShiftedMatrix(parityMatrix.size());

  // mix columns
  for ( int i = 0; i < parityMatrix.size(); ++i ) {
    for ( int j = 0; j < parityMatrix[i].size(); j++ ) {      

      finalShiftedMatrix[i][0][j] = 
          mixRow(parityMatrix[i][0][j], 2) ^ 
          mixRow(parityMatrix[i][1][j], 3) ^ 
          parityMatrix[i][2][j] ^ 
          parityMatrix[i][3][j];

      finalShiftedMatrix[i][1][j] =
          parityMatrix[i][0][j] ^
          mixRow(parityMatrix[i][1][j], 2) ^
          mixRow(parityMatrix[i][2][j], 3) ^
          parityMatrix[i][3][j];
   
      finalShiftedMatrix[i][2][j] =
          parityMatrix[i][0][j] ^
          parityMatrix[i][1][j] ^
          mixRow(parityMatrix[i][2][j], 2) ^
          mixRow(parityMatrix[i][3][j], 3);

      finalShiftedMatrix[i][3][j] =
          mixRow(parityMatrix[i][0][j], 3) ^
          parityMatrix[i][1][j] ^
          parityMatrix[i][2][j] ^
          mixRow(parityMatrix[i][3][j], 2);
    }
  }

  sendToOutputs<string>("\nMixed Columns: \n\n" + printBlocks(finalShiftedMatrix, true), outputFile);

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
//         sendToOutputs
//---------------------------------
template<class T>
void sendToOutputs(T toPrint, FileStream<ofstream>& out) {

  out.stream << toPrint;
  cout << toPrint;
  
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
//        binaryHasEvenOnes
//---------------------------------
bool binaryHasOddOnes(uint8_t rowByte, int i) {
      
  for ( i = 0; rowByte != 0; ++i ) 
    rowByte &= ( rowByte - 1 );
  
  return i % 2 != 0;
}


//---------------------------------
//        stringToMatrixVec
//---------------------------------
matrixVector stringToMatrixVec(string text) {

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
string printBlocks(matrixVector & matrix, bool _hex) {
       
  ostringstream oss;

  // format elements of data block for output
  for ( auto& block : matrix ) {
    for ( auto& row : block ) {
      for ( auto& byte : row ) {
        if ( _hex ) oss << hex << static_cast<int>(byte) << dec << ' ';
        else oss << byte << ' ';
      }oss << endl;
    }oss << endl;
  }oss << endl;

  return oss.str();
}


//---------------------------------
//             mixRow
//---------------------------------
uint8_t mixRow(uint8_t source, int shift) {
  
  uint8_t newByte;
  newByte = shift == 2 ? source << 1 : (source << 1) ^ source;
  
  if ( source & 0x80 ) {
    newByte ^= 0x1b;
  }
  else if ( binaryHasOddOnes(newByte)) {
    newByte ^= 0x80;
  }
  return newByte;
}


//---------------------------------
//      vigenereSubstitution
//---------------------------------
string vigenereSubstitution(string& plaintext, string& cipher, FileStream<ofstream>& out) {
  
  string subString { plaintext };
  
  for ( int i = 0; i < plaintext.size(); ++i ) {
    int char1 = plaintext[i] - 'A';
    int char2 = cipher[i%cipher.size()] - 'A';
    subString[i] = ( ( char1 + char2 ) % 26 ) + 'A';
  }
  sendToOutputs<string>("subText: " + subString + "\n\n", out);

  // padding
  for ( auto i = 0; i < subString.size() % 16; ++i )
    subString.push_back('A');
  
  return subString;
}
