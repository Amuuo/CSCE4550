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
processRequest(Instruction& instruction) {

  methods[instruction.method](this,instruction);   
}




// ========================================================================
// function adds subject to the reference monitor subjectSecurityMap with 
// name and security level as well as the Assests class with just the name
// ========================================================================

void ReferenceMonitor::
addSubject(Instruction& instruction) {

  string subject {instruction.subject}, 
         security{instruction.security},    // string to hold instruction parameters
         line    {instruction.instruction};

    
  if (securityMap.find(security) == securityMap.end())
    throw runtime_error(
      Instruction::constructMsg(
        "!!BAD INSTRUCTION-->UNKNOWN SECURITY CLEARANCE",line, B_RED+WHITE));
  
  if (subjectSecurityLevel.find(subject) == subjectSecurityLevel.end()) {
    subjectSecurityLevel[subject] = securityMap[security];
    subjectMap[subject] = {subject};
  }
  else
    throw runtime_error(
      Instruction::constructMsg(
        "!!BAD INSTRUCTION-->SUBJECT ALREADY EXISTS",line, RED+BOLD));

  printInstructionResult(
    Instruction::constructMsg("SUBJECT ADDED", line, GREEN+BOLD));
}




// ========================================================================
// function adds object to the reference monitor objectSecurityMap with 
// name and security level as well as the Assests class with just the name
// ========================================================================

void ReferenceMonitor::
addObject(Instruction& instruction) {
 
  string object  {instruction.object}, 
         security{instruction.security},    // string to hold instruction parameters
         line    {instruction.instruction};


  if (securityMap.find(security) == securityMap.end())
    throw runtime_error(
      Instruction::constructMsg(
        "!!BAD INSTRUCTION-->UNKNOWN SECURITY CLEARANCE",line, B_RED+WHITE));

  if (objectSecurityLevel.find(object) == objectSecurityLevel.end()) {
    objectSecurityLevel[object] = securityMap[security]; 
    objectMap[object] = {object};
  }
  else
    throw runtime_error(
      Instruction::constructMsg(
        "!!BAD INSTRUCTION-->OBJECT ALREADY EXISTS",line, B_RED+WHITE));    

  printInstructionResult(
    Instruction::constructMsg("OBJECT ADDED", line, GREEN+BOLD));
}




// ========================================================================
// checks the security maps and decides whether the subject has 
// authorization to read from requested object, , updates assests if necessary
// ========================================================================

void ReferenceMonitor::
executeRead(Instruction& instruction) {

  string object  {instruction.object}, 
         subject {instruction.subject},    // string to hold instruction parameters
         line    {instruction.instruction};

  
  if (subjectSecurityLevel.find(subject) == subjectSecurityLevel.end() ||
        subjectMap.find(subject) == subjectMap.end())
    throw runtime_error(
      Instruction::constructMsg(
        "!!BAD INSTRUCTION-->UNKNOWN SUBJECT", line, B_RED+WHITE));

  if (objectSecurityLevel.find(object) == objectSecurityLevel.end()||
        objectMap.find(object) == objectMap.end())
    throw runtime_error(
      Instruction::constructMsg(
        "!!BAD INSTRUCTION-->UNKNOWN OBJECT", line, B_RED+WHITE));

  if (subjectSecurityLevel[subject] >= objectSecurityLevel[object]) {
    subjectMap[subject].readObject(objectMap[object]);
    
    printInstructionResult(
      Instruction::constructMsg("READ ACCESS GRANTED", subject+
                                  " reads "+object, GREEN+BOLD));
  }
  else
    printInstructionResult(
      Instruction::constructMsg("READ ACCESS DENIED", line, RED+BOLD));    
}




// ========================================================================
// checks the security maps and decides whether the subject has 
// authorization to write to requested object, updates assests if necessary
// ========================================================================

void ReferenceMonitor::
executeWrite(Instruction& instruction) {


  string object {instruction.object},
         subject{instruction.subject},    // string to hold instruction parameters
         line   {instruction.instruction};
  int    temp   {instruction.value};


  if (subjectSecurityLevel.find(subject) == subjectSecurityLevel.end() ||
        subjectMap.find(subject) == subjectMap.end())
    throw runtime_error(
      Instruction::constructMsg(
        "!!BAD INSTRUCTION-->UNKNOWN SUBJECT", line, B_RED+WHITE));


  if (objectSecurityLevel.find(object) == objectSecurityLevel.end()||
        objectMap.find(object) == objectMap.end())
    throw runtime_error(
      Instruction::constructMsg(
        "!!BAD INSTRUCTION-->UNKNOWN OBJECT", line, B_RED+WHITE));   
 
  
  if (subjectSecurityLevel[subject] <= objectSecurityLevel[object]) {
    subjectMap[subject].writeObject(objectMap[object],temp);

    printInstructionResult(Instruction::constructMsg(
      "WRITE ACCESS GRANTED", subject+" writes value "+ 
        to_string(temp)+" to "+object, GREEN+BOLD));
  }
  else
    printInstructionResult(
      Instruction::constructMsg("WRITE ACCESS DENIED", line, RED+BOLD));    
}





// ========================================================================
// prints the results of each instruction request
// ========================================================================

inline void ReferenceMonitor::
printInstructionResult(string message) {
  
  ostringstream out{};  // ostringstream to format instruction log
  
  out << "   " << setw(21) << setfill(' ') << left;
  out << Time{}.getTimeAndDate() << message;
    
  //print instruction to screen
  cout << endl << out.str();
  
  //print instruction to output log.txt
  ofstream{"log.txt",ios::app} << endl << out.str();
  
}




void ReferenceMonitor::formatAndOutputLogTitle(string inputFile) {

  ostringstream out{};

  int borderLength = inputFile.size()>=PAGE_WIDTH ? (inputFile.size()+2) : PAGE_WIDTH+2;  
  int timeLeadingSpace = (borderLength-24)/2;  
  int inputFileLeadingSpace = borderLength>=PAGE_WIDTH+2 ? 
      (borderLength-inputFile.size())/2 : (borderLength%(PAGE_WIDTH+2))/2;
  
  out << "\n\n\n\n\n";
  out << setw(borderLength) << setfill('=') << " " << "\n";
  out << string(timeLeadingSpace,' ') + Time{}.getFullTime();
  out << setw(borderLength) << setfill('=') << " " << "\n";
  out << setw(borderLength) << setfill(' ') << left; 
  out << string(inputFileLeadingSpace,' ') + inputFile << "\n";
  out << setw(borderLength) << setfill('-') << right << " ";
  out << "\n\n\n";

  cout << endl << out.str();
  ofstream{"log.txt",ios::app} << endl << out.str();  
}



// ========================================================================
// prints the current state of the objects and subjects in Assests struct
// ========================================================================

void ReferenceMonitor::
printState() {

  ostringstream out{};
  string lws((PAGE_WIDTH-24)/3,' ');

  out << "\n\n";
  out << "\n" << lws << "[44;37m +====== CURRENT STATE =====+ [0m";
  out << "\n" << lws << "[44;37m |                          | [0m";
  out << "\n" << lws << "[44;37m |-- subject --|--- temp ---| [0m";
  out << "\n" << lws << "[44;37m |             |            | [0m";
  
  for (auto subject : subjectMap) {
    out << "\n"  << lws << "[44;37m |   " << subject.second.getName() << "   |" 
        << right << setw(7) << subject.second.getTemp() << "     | [0m";
  }
  out << "\n" << lws <<"[44;37m |             |            | [0m";
  out << "\n" << lws <<"[44;37m |-- object ---|-- value ---| [0m";
  out << "\n" << lws <<"[44;37m |             |            | [0m";
  
  for (auto object : objectMap) {
    out << "\n"  << lws << "[44;37m |   " << object.second.getName() << "   |" 
        << right << setw(7) << object.second.getValue() << "     | [0m";
  }
  out << "\n" << lws << "[44;37m |             |            | [0m";
  out << "\n" << lws << "[44;37m +==========================+ [0m\n\n";

  cout << out.str();

  //ofstream{"log.txt", ios::app} << out.str();
}


