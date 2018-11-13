#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<exception>
#include<algorithm>
#include<vector>

using namespace std;

template<class T> T userInput(const char*);
string fileContentsToString(ifstream&);


//=================================
//              main
//=================================
int main() {
  

  ifstream plaintextFile { "input1.txt" };  
  ifstream cipherKeyFile { "key1.txt" };
  //ifstream plaintextFile { userInput<string>("Enter plaintext filename:") };
  //ifstream cipherKeyFile { userInput<string>("Enter cipherkey filename: ") };
  
  string plaintext = fileContentsToString(plaintextFile);
  string ciphertext = fileContentsToString(cipherKeyFile);              

  ofstream outputFile { "output.txt" };
  //ofstream outputFile { userInput<string>("Enter output filename: ") };


  for ( auto i = 0; i<plaintext.size()%16; ++i ) {
    plaintext.push_back('A');
  }

  cout << "ciphertext: " << ciphertext << endl;
  cout << "plaintext before shift: " << plaintext << endl;
  outputFile << "Input:\t\t" << "Output:" << endl;


  int counter = 0;
  for ( auto i = plaintext.begin(); i < plaintext.end(); i += 16 ) {            

    rotate(i + 4, i + 5, i + 8);
    rotate(i + 8, i + 9, i + 12);
    rotate(i + 12, i + 13, i + 16);  
  }

  cout << "plaintextShifted: " << plaintext << endl;




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
string fileContentsToString(ifstream & stream) {
  
  char character;
  string outputString;
  
  while ( stream.get(character) ) {
    if ( isalpha(character) ) {
      outputString.push_back(character);
    }
  }
  return outputString;
}
