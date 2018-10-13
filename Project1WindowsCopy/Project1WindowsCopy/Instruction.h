#pragma once
#include<string>
#include<iostream>
using namespace std;


class Instruction {
  public:
  Instruction() {}
  ~Instruction() {}

  Instruction(string function,string instruction) : 
    function{function},
    instruction{instruction}{}
  
  string function{};
  string instruction{};
};



