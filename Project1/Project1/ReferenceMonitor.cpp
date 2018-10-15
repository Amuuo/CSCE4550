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
    throw runtime_error(Instruction::constructErrorMsg("UNKNOWN SECURITY CLEARANCE",line));
  
  if (subjectSecurityLevel.find(subject) == subjectSecurityLevel.end()) {
    subjectSecurityLevel[subject] = securityMap[security];
    assests.getSubjectMap()[subject] = {subject};
  }
  else
    throw runtime_error(Instruction::constructErrorMsg("[31;1mSUBJECT ALREADY EXISTS",line));

  logResult("[32;1mSUBJECT ADDED", line);
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
    throw runtime_error(Instruction::constructErrorMsg("UNKNOWN SECURITY CLEARANCE",line));

  if (objectSecurityLevel.find(object) == objectSecurityLevel.end()) {
    objectSecurityLevel[object] = securityMap[security]; 
    assests.getObjectMap()[object] = {object};
  }
  else
    throw runtime_error(Instruction::constructErrorMsg("[31;1mOBJECT ALREADY EXISTS",line));

  logResult("[32;1mOBJECT ADDED", line);
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
    throw runtime_error(Instruction::constructErrorMsg("UNKNOWN SUBJECT",line));

  if (objectSecurityLevel.find(object) == objectSecurityLevel.end())
    throw runtime_error(Instruction::constructErrorMsg("UNKNOWN OBJECT",line));

  if (subjectSecurityLevel[subject] >= objectSecurityLevel[object]) {
    assests.getSubjectMap()[subject].readObject(assests.getObjectMap()[object]);
    
    logResult("[32;1mREAD ACCESS GRANTED", subject+" reads "+object);
  }
  else
    logResult("[31;1mREAD ACCESS DENIED", line);    
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
    throw runtime_error(Instruction::constructErrorMsg("UNKNOWN SUBJECT",line));

  
  if (objectSecurityLevel.find(object) == 
      objectSecurityLevel.end())
    throw runtime_error(Instruction::constructErrorMsg("UNKNOWN OBJECT", line));   
 
  
  if (subjectSecurityLevel[subject] <=
      objectSecurityLevel[object]) {

    assests.getSubjectMap()[subject].writeObject(
      assests.getObjectMap()[object],temp);

    logResult("[32;1mWRITE ACCESS GRANTED", subject+" writes value "+ 
                                  to_string(temp)+" to "+object);

  }
  else
    logResult("[31;1mWRITE ACCESS DENIED", line);    
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
  
  bool lineIsAlreadyConstructed{line == " "};
  
  ostringstream out{};  // ostringstream to format instruction log
  
  out << "   ";
  out << "[37m" << setw(21) << setfill(' ') << left << (line != "" ? Time{}.getTimeAndDate() : "");
  out << "[0m" << (lineIsAlreadyConstructed ? setw(0) : setw(47)) << setfill('-') << left << header;
  
  if(!lineIsAlreadyConstructed)
    out << "> " << setw(35) << setfill(' ') << left << string{line+"[0m"};

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
  string lws((PAGE_WIDTH-24)/2,' ');

  out << "\n\n" << lws << "[47;30m +====== CURRENT STATE =====+ [0m";
  out << "\n" << lws << "[47;30m |                          | [0m";
  out << "\n" << lws << "[47;30m |-- subject --|--- temp ---| [0m";
  out << "\n" << lws << "[47;30m |             |            | [0m";
  
  for (auto subject : assests.getSubjectMap()) {
    out << "\n"  << lws << "[47;30m |   " << subject.second.getName() << "   |" 
        << right << setw(7) << subject.second.getTemp() << "     | [0m";
  }
  out << "\n" << lws <<"[47;30m |             |            | [0m";
  out << "\n" << lws <<"[47;30m |-- object ---|-- value ---| [0m";
  out << "\n" << lws <<"[47;30m |             |            | [0m";
  
  for (auto object : assests.getObjectMap()) {
    out << "\n"  << lws << "[47;30m |   " << object.second.getName() << "   |" 
        << right << setw(7) << object.second.getValue() << "     | [0m";
  }
  out << "\n" << lws << "[47;30m |             |            | [0m";
  out << "\n" << lws << "[47;30m +==========================+ [0m\n";

  cout << out.str();

  //ofstream{"log.txt", ios::app} << out.str();
}


