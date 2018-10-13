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
  
  Instruction(string& instruction) : instruction{instruction} {
    istringstream iss{instruction};
    iss >> function;    
    // remove any leading or trailing spaces
    instruction = regex_replace(instruction,regex{"^ +| +$|( ) +"},"");
  }
  
  Instruction(string function,string instruction) : 
    function{function},
    instruction{instruction}{}  
  
  ~Instruction() {}

  string function{};
  string instruction{};
};



