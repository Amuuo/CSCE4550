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



Instruction::Instruction(string& _instruction) {
    
  instruction = _instruction;
    
  // convert instruction to all lowercase
  transform(instruction.begin(),instruction.end(),
            instruction.begin(),::tolower);
    
  // remove any leading or trailing spaces
  instruction = regex_replace(instruction,regex{"^ +| +$|( ) +"},"");
    
  istringstream iss{instruction};
  iss >> method;

  verifyMethod();
} 



// ========================================================================
// function adds subject to the reference monitor subjectSecurityMap with 
// name and security level as well as the subject map with just the name
// ========================================================================
void Instruction::verifyMethod() {

  if (functionMap.find(method) == functionMap.end())
    throw runtime_error(constructMsg("UNKNOWN METHOD"));
  else {
    functionMap[method](this);
  }
}



// ========================================================================
// function adds subject to the reference monitor subjectSecurityMap with 
// name and security level as well as the subject map with just the name
// ========================================================================
void Instruction::veriftyAddsub() {

  istringstream iss{instruction};  // stringstream to input instructions

  while (iss) { 
    iss >> method >> subject  >> security;

    if (!iss.eof())
      throw runtime_error(constructMsg("TOO MANY PARAMETERS"));
  }
  if (subject.empty())
    throw runtime_error(constructMsg("NO SUBJECT"));
  
  if (security.empty())
    throw runtime_error(constructMsg("NO SECURITY"));
}



// ========================================================================
// function adds subject to the reference monitor subjectSecurityMap with 
// name and security level as well as the subject map with just the name
// ========================================================================
void Instruction::verifyAddobj() {

  istringstream iss{instruction};  // stringstream to input instructions
  

  while (iss) { 
    iss >> method >> object >> security;

    if (!iss.eof())
      throw runtime_error(constructMsg("TOO MANY PARAMETERS"));
  }

  if (object.empty())
    throw runtime_error(constructMsg("NO OBJECT"));
  
  if (security.empty())
    throw runtime_error(constructMsg("NO SECURITY"));

  
}



// ========================================================================
// function adds subject to the reference monitor subjectSecurityMap with 
// name and security level as well as the subject map with just the name
// ========================================================================
void Instruction::verifyRead() {
  
  istringstream iss{instruction};  // stringstream to input instructions

  while (iss) { 
    iss >> method >> subject >> object;

    if (!iss.eof())
      throw runtime_error(constructMsg("TOO MANY PARAMETERS"));
  } 
  if (subject.empty())
      throw runtime_error(constructMsg("NO SUBJECT"));
  if (object.empty())
    throw runtime_error(constructMsg("NO OBJECT"));
}



// ========================================================================
// function adds subject to the reference monitor subjectSecurityMap with 
// name and security level as well as the subject map with just the name
// ========================================================================
void Instruction::verifyWrite() {
  
  istringstream iss{instruction};  // stringstream to input instructions
  string valueStr;

  while (iss) { 
    iss >> method >> subject >> object >> valueStr;
    
    if (!iss.eof())
      throw runtime_error(constructMsg("TOO MANY PARAMETERS"));
  }
  
  if (subject.empty())
    throw runtime_error(constructMsg("NO SUBJECT"));
  if (object.empty())
    throw runtime_error(constructMsg("NO OBJECT"));
  if (valueStr.empty())
    throw runtime_error(constructMsg("NO VALUE"));
  for (auto c : valueStr)
    if (c < '0' || c > '9')
      throw runtime_error(constructMsg("NON-NUMERIC VALUE"));

  value = stoi(valueStr);
}



// ========================================================================
// function adds subject to the reference monitor subjectSecurityMap with 
// name and security level as well as the subject map with just the name
// ========================================================================
string Instruction::constructMsg(string result, string color, string line) {
  
  color = "[" + color + "m";
  //line = line == "" ? instruction : line;
    
  ostringstream out; // osstream to format string

  out << color << setw(40) << setfill('-') << left;
  out << string{result};
  out << "> " << setw(35) << setfill(' ') << left;
  out << (line == "" ? string{instruction+"[0m"} : string{line + "[0m"});
  return out.str();
}



// ========================================================================
// function adds subject to the reference monitor subjectSecurityMap with 
// name and security level as well as the subject map with just the name
// ========================================================================
string Instruction::constructMsg(string error) {
  
  return constructMsg("!!BAD INSTRUCTION-->"+error, B_RED+WHITE, instruction);
}
