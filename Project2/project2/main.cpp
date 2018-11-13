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
  
  string plaintext;
  string ciphertext;    


  ifstream plaintextFile { userInput<string>("Enter plaintext filename:") };
  ifstream cipherKeyFile { userInput<string>("Enter cipherkey filename: ") };
        
  char character;
  string word;

  while ( !plaintextFile.eof() ) {
      
    plaintextFile.get(character);
      
    if ( isalpha(character) ) {
      plaintext += character;
    }
  }

  while ( !cipherKeyFile.eof() ) {
    
    cipherKeyFile >> word;
    ciphertext += word;
  }


  ofstream outputFile { userInput<string>("Enter output filename: ") };


  for ( auto i = 0; i<plaintext.size()%16; ++i ) {
    plaintext.push_back('A');
  }
  cout << "\nplaintextProcessed: " << plaintext << endl;
  string plaintextShifted { plaintext };
  cout << "plaintextShifted: " << plaintextShifted << endl;
  outputFile << "Input:\t\t" << "Output:" << endl;


  int counter = 0;
  for ( auto i = plaintextShifted.begin(); i < plaintextShifted.end(); i += 16 ) {            

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

