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



//=================================
//              main
//=================================
int main()
{
  
  string plaintextProcessed;
  string ciphertextProcessed;    


  ifstream plaintextFile { userInput<string>("Enter plaintext filename:").c_str() };
  ifstream cipherKeyFile { userInput<string>("Enter cipherkey filename: ").c_str() };
        
  char character;
  string word;

  while ( !plaintextFile.eof() ) {
      
    plaintextFile.get(character);
      
    if ( isalpha(character) ) {
      plaintextProcessed += character;
    }
  }

  while ( !cipherKeyFile.eof() ) {
    
    cipherKeyFile >> word;
    ciphertextProcessed += word;
  }


  ofstream outputFile { userInput<string>("Enter output filename: ") };


  for ( auto i = 0; i<plaintextProcessed.size()%16; ++i ) {
    plaintextProcessed.push_back('A');
  }
  cout << "\nplaintextProcessed: " << plaintextProcessed << endl;
  string plaintextShifted { plaintextProcessed };
  cout << "plaintextShifted: " << plaintextShifted << endl;
  outputFile << "Input:\t\t" << "Output:" << endl;


  int counter = 0;
  for ( auto i = plaintextShifted.begin(), j = plaintextProcessed.begin(); 
       i < plaintextShifted.end(); i += 16, j+=16, counter+=16 ) {            

    rotate(i + 4, i + 5, i + 8);
    rotate(i + 8, i + 9, i + 12);
    rotate(i + 12, i + 13, i + 16);  
  }

  cout << "plaintextShifted: " << plaintextShifted << endl;




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

