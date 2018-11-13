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
void outputRow(string::iterator&, string::iterator&, ofstream&);
void outputBlock(string::iterator&, string::iterator&, ofstream&);



//=================================
//              main
//=================================
int main()
{
  
  string plaintextProcessed;
  string ciphertextProcessed;    


  ifstream plaintextFile { "input1.txt" };
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
  string plaintextShifted { plaintextProcessed };

  outputFile << "Input:\t\t" << "Output:" << endl;

  for ( auto i = plaintextShifted.begin(), j = plaintextProcessed.begin(); 
       i < plaintextShifted.end(); i += 16, j+=16 ) {            

    rotate(i + 4, i + 5, i + 8);
    rotate(i + 8, i + 9, i + 12);
    rotate(i + 12, i + 13, i + 16);

    outputBlock(i, j, outputFile);    
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
void outputRow(string::iterator& iter, string::iterator& iter2, ofstream& out) {

  int counter = 0;
  int counter2;

  for ( auto counter = 0; counter < 4; ++counter ) {
    
    counter2 = 0;
    
    for ( auto i = iter; counter2 < 4; ++i ) {
      out << *i;
    }
        
    counter2 = 0;
    
    for ( auto i = iter2; counter2 < 4; ++i ) {
      out << *i;
    }
    ++counter;
  }
  out << endl;
}

void outputBlock(string::iterator & processedIter, string::iterator& plainIter, ofstream& out) {
  
  int counter = 0;
  
  for ( auto i = processedIter, j = plainIter; counter < 4; processedIter+=4, plainIter+=4, ++counter ) {
    outputRow(i, j, out);
  }
  out << endl;
}
