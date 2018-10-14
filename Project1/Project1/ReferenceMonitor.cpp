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

#include "ReferenceMonitor.h"

ReferenceMonitor::ReferenceMonitor() {
}

ReferenceMonitor::~ReferenceMonitor() {
}

// ========================================================================
// check if instruction method exists and invoke the necessary function
// ========================================================================

void ReferenceMonitor::
scanInstruction(Instruction& instruction, Assests& assests) {

  methods[instruction.method](this,instruction,assests);   
}




// ========================================================================
// function adds subject to the reference monitor subjectSecurityMap with 
// name and security level as well as the Assests class with just the name
// ========================================================================

void ReferenceMonitor::
addSubject(Instruction& instruction, Assests& assests) {

  string subject {move(instruction.subject)}, 
         security{move(instruction.security)},    // string to hold instruction parameters
         line    {move(instruction.instruction)};

    
  if (securityMap.find(security) == securityMap.end())
    throw runtime_error("(Unknown Security Clearance) "+line);
  
  if (subjectSecurityLevel.find(subject) == subjectSecurityLevel.end()) {
    subjectSecurityLevel[subject] = securityMap[security];
    assests.getSubjectMap()[subject] = {subject};
  }
  else
    throw runtime_error("(Subject Alreadys Exists) "+line);

  printInstructionResult("Subject Added",line); 
}




// ========================================================================
// function adds object to the reference monitor objectSecurityMap with 
// name and security level as well as the Assests class with just the name
// ========================================================================

void ReferenceMonitor::
addObject(Instruction& instruction, Assests& assests) {
 
  string object  {move(instruction.object)}, 
         security{move(instruction.security)},    // string to hold instruction parameters
         line    {move(instruction.instruction)};


  if (securityMap.find(security) == securityMap.end())
    throw runtime_error("(Unknown Security Clearance) "+line);

  if (objectSecurityLevel.find(object) == objectSecurityLevel.end()) {
    objectSecurityLevel[object] = securityMap[security]; 
    assests.getObjectMap()[object] = {object};
  }
  else
    throw runtime_error("(Object Alreadys Exists) "+line);

  printInstructionResult("Object Added",line);
}




// ========================================================================
// checks the security maps and decides whether the subject has 
// authorization to read from requested object, , updates assests if necessary
// ========================================================================

void ReferenceMonitor::
executeRead(Instruction& instruction, Assests& assests) {

  string object  {move(instruction.object)}, 
         subject {move(instruction.subject)},    // string to hold instruction parameters
         line    {move(instruction.instruction)};

  
  if (subjectSecurityLevel.find(subject) == subjectSecurityLevel.end())
    throw runtime_error("(Unknown Subject) "+line);

  if (objectSecurityLevel.find(object) == objectSecurityLevel.end())
    throw runtime_error("(Unknown Object) "+line);

  if (subjectSecurityLevel[subject] >= objectSecurityLevel[object]) {        
    assests.getSubjectMap()[subject].readObject(assests.getObjectMap()[object]);
    printInstructionResult("Access Granted", subject + " reads " + object);
  }
  else 
    printInstructionResult("Access Denied",line);    
}




// ========================================================================
// checks the security maps and decides whether the subject has 
// authorization to write to requested object, updates assests if necessary
// ========================================================================

void ReferenceMonitor::
executeWrite(Instruction& instruction,Assests& assests) {
  
  string object  {move(instruction.object)},
         subject {move(instruction.subject)},    // string to hold instruction parameters
         line    {move(instruction.instruction)};
  int    temp    {move(instruction.value)};

  
  if (subjectSecurityLevel.find(subject) == 
      subjectSecurityLevel.end())
    throw runtime_error("(Unknown Subject) "+line);

  
  if (objectSecurityLevel.find(object) == 
      objectSecurityLevel.end())
    throw runtime_error("(Unknown Object) "+line);   
 
  
  if (subjectSecurityLevel[subject] <= 
      objectSecurityLevel[object]) {
    
    assests.getSubjectMap()[subject].writeObject(
      assests.getObjectMap()[object], temp);
    
    printInstructionResult("Access Granted", subject+
                        " writes value "+to_string(temp)+
                        " to "+object);
  }
  else
    printInstructionResult("Access Denied",line);
}





// ========================================================================
// prints the results of each instruction request
// ========================================================================



void ReferenceMonitor::printInstructionResult(string header,string instruction) {

  ofstream log{"log.txt", ios::app};
  ostringstream out{};  // ostringstream to format instruction log
  out  << left << setw(16) << header << ": " << left << instruction;
  cout << endl << out.str();  // print result to screen
  log  << endl << out.str();  // print result to log
  log.close();
}




// ========================================================================
// prints the current state of the objects and subjects in Assests struct
// ========================================================================

void ReferenceMonitor::
printState(Assests& assests) {

  cout << "\n\n+====== current state =====+";
  cout << "\n|                          |";
  cout << "\n|== subject ==|=== temp ===|";
  cout << "\n|             |            |";
  
  for (auto subject : assests.getSubjectMap()) {
    cout << "\n" << "|   " << subject.second.getName() << "   |" 
         << right << setw(7) << subject.second.getTemp() << "     |";
  }
  cout << "\n|             |            |";
  cout << "\n|== object ===|== value ===|";
  cout << "\n|             |            |";
  
  
  for (auto object : assests.getObjectMap()) {
    cout << "\n" << "|   " << object.second.getName() << "   |" 
         << right << setw(7) << object.second.getValue() << "     |";
  }
  cout << "\n|             |            |";
  cout << "\n+==========================+\n";
}


