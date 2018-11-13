#include<cstdio>
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<exception>
#include<algorithm>
#include<vector>

using namespace std;

template<class T> T userInput(const char*);
void outputRow(string::iterator&, ofstream&);




//=================================
//              main
//=================================
int main()
{
  
  string plaintextProcessed;
  string ciphertextProcessed;  
  

  // try block will throw any issues with input/output streams
  try {    

    ifstream plaintextFile { userInput<string>("Enter input filename: ") };
    ifstream cipherKeyFile { userInput<string>("Enter cipherkey filename: ") };
        
    
    char character;
    string word;

    while ( plaintextFile ) {
      
      plaintextFile.get(character);
      
      if ( isalpha(character) ) {
        plaintextProcessed += character;
      }
    }

    while ( cipherKeyFile ) {
    
      cipherKeyFile >> word;
      ciphertextProcessed += word;
    }
  
  } catch ( exception& e ) { cout << e.what(); }


  ofstream outputFile { userInput<string>("Enter output filename: ") };


  for ( auto i = 0; i<plaintextProcessed.size()%16; ++i ) {
    plaintextProcessed.push_back('A');
  }

  outputFile << "Input:\t\t" << "Output:" << endl;

  for ( auto i = plaintextProcessed.begin(); i < plaintextProcessed.end(); i += 16 ) {
    
    outputRow(i, outputFile);
    outputFile << "\t\t";

    rotate(i + 4, i + 5, i + 8);
    rotate(i + 8, i + 9, i + 12);
    rotate(i + 12, i + 13, i + 16);

    outputRow(i, outputFile);
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
//            ouputRow
//---------------------------------
void outputRow(string::iterator& iter, ofstream& out) {

  int counter = 0;
  for ( auto j = iter; counter < 4; ++j ) {
    out << *j;
    ++counter;
  }
  out << endl;
}