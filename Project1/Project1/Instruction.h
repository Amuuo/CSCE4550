#pragma once
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<regex>
using namespace std;



class Instruction {
  
  public:
  Instruction() {}
  Instruction(string& _instruction) {    
    
    instruction = _instruction;
    
    // convert instruction to all lowercase
    transform(instruction.begin(),instruction.end(),
              instruction.begin(),::tolower);
    
    // remove any leading or trailing spaces
    instruction = regex_replace(instruction,regex{"^ +| +$|( ) +"},"");
    
    istringstream iss{instruction};
    iss >> function;    
  }  
  ~Instruction() {}

  string function{};
  string instruction{};
};



