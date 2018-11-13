#include<cstdio>
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<exception>

using namespace std;



template<class T>
T userInput(const char* inputRequestPrompt) {  
  T userInput;
  cout << inputRequestPrompt;
  cin >> userInput;
  return userInput;
}



int main()
{
  
  ostringstream plaintextProcessed;
  ostringstream ciphertextProcessed;
  
  
  // try block will throw any issues with input/output streams
  try {    

    ifstream plaintextFile { userInput<string>("Enter input filename") };
    ifstream cipherKeyFile { userInput<string>("Enter cipherkey filename") };
    ofstream outputFile { userInput<string>("Enter output filename") };
    
    string word;

    while (plaintextFile) {
      plaintextFile >> word;
      plaintextProcessed << word;
    }
    while (cipherKeyFile) {
      cipherKeyFile >> word;
      ciphertextProcessed << word;
    }
  
  } catch (exception& e) { cout << e.what(); }



  
  





  return 0;
}