#pragma once
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
using namespace std;


class Instruction {
  
  public:
  Instruction() {}
  Instruction(string& instruction) : instruction{instruction} {
    istringstream iss{instruction};
    iss >> function;    
  }
  Instruction(string function,string instruction) : 
    function{function},
    instruction{instruction}{}  
  ~Instruction() {}

  string function{};
  string instruction{};
};



