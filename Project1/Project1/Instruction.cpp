/*
============================================================================
----------------------------------------------------------------------------
Name        : Project 1
Author      : Adam Williams
Version     : 1.0
Copyright   : 2018
Description : Program implements Bell-LaPadula security rules using a 
              reference monitor to grant access to interactions between
              various subjects and objects              
----------------------------------------------------------------------------
============================================================================
*/

#include "Instruction.h"



Instruction::Instruction() {}

Instruction::Instruction(string& _instruction) {    
    
    instruction = _instruction;
    
    // convert instruction to all lowercase
    transform(instruction.begin(),instruction.end(),
              instruction.begin(),::tolower);
    
    // remove any leading or trailing spaces
    instruction = regex_replace(instruction,regex{"^ +| +$|( ) +"},"");
    
    istringstream iss{instruction};
    iss >> method;

    verify_method();
  } 


Instruction::Instruction(Instruction&& instruction) :
  method      {move(instruction.method)},
  subject     {move(instruction.subject)},
  object      {move(instruction.object)},
  security    {move(instruction.security)},
  value       {move(instruction.value)},
  instruction {move(instruction.instruction)}{}

Instruction::Instruction(const Instruction& instruction) : 
  method      {instruction.method},
  subject     {instruction.subject},
  object      {instruction.object},
  security    {instruction.security},
  value       {instruction.value},
  instruction {instruction.instruction}{}

Instruction::~Instruction() {}







void Instruction::verify_method() {

  if (functionMap.find(method) == functionMap.end())
    throw runtime_error(constructErrorMsg("Unknown Method"));
  else {
    functionMap[method](this);
  }
}

void Instruction::verifty_addsub() {

  istringstream iss{instruction};  // stringstream to input instructions

  while (iss) { 
    iss >> method >> subject  >> security;

    if (!iss.eof())
      throw runtime_error(constructErrorMsg("Too Many Parameters"));
  }
  if (subject.empty())
    throw runtime_error(constructErrorMsg("No Subject"));
  
  if (security.empty())
    throw runtime_error(constructErrorMsg("No Security"));
}



void Instruction::verify_addobj() {

  istringstream iss{instruction};  // stringstream to input instructions
  

  while (iss) { 
    iss >> method >> object >> security;

    if (!iss.eof())
      throw runtime_error(constructErrorMsg("Too Many Parameters"));
  }

  if (object.empty())
    throw runtime_error(constructErrorMsg("No Object"));
  
  if (security.empty())
    throw runtime_error(constructErrorMsg("No Security"));

  
}



void Instruction::verify_read() {
  
  istringstream iss{instruction};  // stringstream to input instructions

  while (iss) { 
    iss >> method >> subject >> object;

    if (!iss.eof())
      throw runtime_error(constructErrorMsg("Too Many Parameters"));          
  } 
  if (subject.empty())
      throw runtime_error(constructErrorMsg("No Subject"));
  if (object.empty())
    throw runtime_error(constructErrorMsg("No Object"));
}



void Instruction::verify_write() {
  
  istringstream iss{instruction};  // stringstream to input instructions 
  string valueStr;

  while (iss) { 
    iss >> method >> subject >> object >> valueStr;
    
    if (!iss.eof())
      throw runtime_error(constructErrorMsg("Too Many Parameters"));  
  }
  
  if (subject.empty())
    throw runtime_error(constructErrorMsg("No Subject"));
  if (object.empty())
    throw runtime_error(constructErrorMsg("No Object"));
  if (valueStr.empty())
    throw runtime_error(constructErrorMsg("No Value"));
  for (auto c : valueStr)
    if (c < '0' || c > '9')
      throw runtime_error(constructErrorMsg("Value is not Alpha-numeric"));

  value = stoi(valueStr);
}

string Instruction::constructErrorMsg(string error, string line) {
  ostringstream out;
  out << setw(18) << right << "!BAD INSTRUCTION!" <<" >> " 
      << "(" <<error<<") " << "\""<< line << "\"";
  return out.str();
}

string Instruction::constructErrorMsg(string error) {
  return constructErrorMsg(error,instruction);
}
