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
    throw runtime_error(Instruction::constructErrorMsg("Unknown Security Clearance",line));
  
  if (subjectSecurityLevel.find(subject) == subjectSecurityLevel.end()) {
    subjectSecurityLevel[subject] = securityMap[security];
    assests.getSubjectMap()[subject] = {subject};
  }
  else
    throw runtime_error(Instruction::constructErrorMsg("Subject Alreadys Exists",line));

  logResult("SUBJECT ADDED", line);
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
    throw runtime_error(Instruction::constructErrorMsg("Unknown Security Clearance",line));

  if (objectSecurityLevel.find(object) == objectSecurityLevel.end()) {
    objectSecurityLevel[object] = securityMap[security]; 
    assests.getObjectMap()[object] = {object};
  }
  else
    throw runtime_error(Instruction::constructErrorMsg("Object Alreadys Exists",line));

  logResult("OBJECT ADDED", line);
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
    throw runtime_error(Instruction::constructErrorMsg("Unknown Subject",line));

  if (objectSecurityLevel.find(object) == objectSecurityLevel.end())
    throw runtime_error(Instruction::constructErrorMsg("Unknown Object",line));

  if (subjectSecurityLevel[subject] >= objectSecurityLevel[object]) {
    assests.getSubjectMap()[subject].readObject(assests.getObjectMap()[object]);
    
    logResult("ACCESS GRANTED", subject+" reads "+object);
  }
  else
    logResult("ACCESS DENIED", line);    
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
    throw runtime_error(Instruction::constructErrorMsg("Unknown Subject",line));

  
  if (objectSecurityLevel.find(object) == 
      objectSecurityLevel.end())
    throw runtime_error(Instruction::constructErrorMsg("Unknown Object", line));   
 
  
  if (subjectSecurityLevel[subject] <=
      objectSecurityLevel[object]) {

    assests.getSubjectMap()[subject].writeObject(
      assests.getObjectMap()[object],temp);

    logResult("ACCESS GRANTED", subject+" writes value "+ 
                                  to_string(temp)+" to "+object);

  }
  else
    logResult("ACCESS DENIED", line);    
}





// ========================================================================
// logs results in a multimap
// ========================================================================
void ReferenceMonitor::
logResult(string header, string instruction) {
  
  //instructionHistory.insert(header,instruction);
  printInstructionResult(header,instruction);

}





// ========================================================================
// prints the results of each instruction request
// ========================================================================

inline string ReferenceMonitor::
printInstructionResult(string header,string line) {
  
  
  
  ostringstream out{};  // ostringstream to format instruction log
  
                                                  
  out << (line != "" ? Time{}.getTime() : "");
  out << right << setw(18) << header;
  out << (line != "" && line != " " ? " >> " : "");
  out << left << "\""<< line <<"\"";
  cout << endl << out.str();  // print result to screen
  
  ofstream{"log.txt", ios::app} << endl << out.str();  // print result to log
  return out.str();
}





void ReferenceMonitor::logTitle(string inputFile) {

  ostringstream out{};

  int borderLength = inputFile.size()>=PAGE_WIDTH ? (inputFile.size()+2) : PAGE_WIDTH+2;  
  int timeLeadingSpace = (borderLength-24)/2;  
  int inputFileLeadingSpace = borderLength>=PAGE_WIDTH+2 ? 
      (borderLength-inputFile.size())/2 : (borderLength%(PAGE_WIDTH+2))/2;
  
  out << string(5,'\n');
  out << string(borderLength,'=') << endl;
  out << string(timeLeadingSpace,' ') << Time{}.getFullTime();
  out << string(borderLength,'=') << endl;
  out << string(inputFileLeadingSpace,' ') << inputFile << "\n";  
  out << string(borderLength,'-') << "\n\n\n";

  printInstructionResult(out.str());
}



// ========================================================================
// prints the current state of the objects and subjects in Assests struct
// ========================================================================

void ReferenceMonitor::
printState(Assests& assests) {

  ostringstream out{};
  string lws((PAGE_WIDTH-24) / 2,' ');

  out << "\n\n" << lws << "+====== CURRENT STATE =====+";
  out << "\n" << lws << "|                          |";
  out << "\n" << lws << "|-- subject --|--- temp ---|";
  out << "\n" << lws << "|             |            |";
  
  for (auto subject : assests.getSubjectMap()) {
    out << "\n"  << lws << "|   " << subject.second.getName() << "   |" 
        << right << setw(7) << subject.second.getTemp() << "     |";
  }
  out << "\n" << lws <<"|             |            |";
  out << "\n" << lws <<"|-- object ---|-- value ---|";
  out << "\n" << lws <<"|             |            |";
  
  for (auto object : assests.getObjectMap()) {
    out << "\n"  << lws << "|   " << object.second.getName() << "   |" 
        << right << setw(7) << object.second.getValue() << "     |";
  }
  out << "\n" << lws << "|             |            |";
  out << "\n" << lws << "+==========================+\n";

  cout << out.str();

  ofstream{"log.txt", ios::app} << out.str();
}


