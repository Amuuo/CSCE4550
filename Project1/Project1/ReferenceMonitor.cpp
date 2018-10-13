#include "ReferenceMonitor.h"


// ========================================================================
// prints the current state of the objects and subjects in Assests struct
// ========================================================================

void ReferenceMonitor::
printState(Assests& assests) {

  cout << "\n\n ====== current state =====";
  cout << "\n|== subject ==|=== temp ===|";
  
  for (auto subject : assests.getSubjectMap()) {
    cout << "\n" << "|   " << subject.second.getName() << "   |" 
         << right << setw(10) << subject.second.getTemp() << "  |";
  }
  cout << "\n|== object ===|== value ===|";
  for (auto object : assests.getObjectMap()) {
    cout << "\n" << "|   " << object.second.getName() << "   |" 
         << right << setw(10) << object.second.getValue() << "  |";
  }
  cout << "\n ==========================\n";
}


// ========================================================================
// check if instruction method exists and invoke the necessary function
// ========================================================================

void ReferenceMonitor::
scanInstruction(Instruction& instruction, Assests& assests) {

  try {
    if (methods.find(instruction.function) == methods.end())
      throw runtime_error("method doesn't exist: " + instruction.instruction);
    
    else
      methods[instruction.function](*this,this,instruction.instruction, assests);   
  }
  catch (exception& e) {
    logInstruction("Bad instruction",e.what());
  }
}


// ========================================================================
// logs the results of each instruction request
// ========================================================================

void ReferenceMonitor::
logInstruction(string header,string instruction) {
  
  ostringstream out{};  // ostringstream to format instruction log

  out << left << setw(16) << header << ": " << left << instruction;
  instructionHistory.push_back(out.str());
}


// ========================================================================
// function adds subject to the reference monitor subjectSecurityMap with 
// name and security level as well as the Assests class with just the name
// ========================================================================

void ReferenceMonitor::addSubject(ReferenceMonitor* ref, string& instruction, Assests& assests) {

  string method,subject,security;  // strings to hold instruction parameters
  istringstream iss{instruction};  // stringstream to input instructions

  while (!iss.eof()) {

    iss >> method >> subject >> security;

    if (!iss.eof() || iss.get() == iss.eof())
      throw runtime_error("Bad addSub instruction : " + instruction);
  }

  if (ref->subjectSecurityLevel.find(subject) == ref->subjectSecurityLevel.end()) {
    ref->subjectSecurityLevel[subject] = ref->securityMap[security];
    assests.getSubjectMap()[subject] = {subject};
  }
  else
    throw runtime_error("(subject doesn't exist)" + instruction);

  ref->logInstruction("Subject added",instruction);
  
}


// ========================================================================
// function adds object to the reference monitor objectSecurityMap with 
// name and security level as well as the Assests class with just the name
// ========================================================================

void ReferenceMonitor::
addObject(ReferenceMonitor* ref,string& instruction, Assests& assests) {

  string method,object,security;   // string to hold instruction parameters
  istringstream iss{instruction};  // stringstream to input instructions


  while (!iss.eof()) {
    iss >> method >> object >> security;

    if (!iss.eof() || iss.get() == iss.eof())
      throw runtime_error(instruction);
  }

  if (ref->objectSecurityLevel.find(object) == ref->objectSecurityLevel.end()) {
    ref->objectSecurityLevel[object] = ref->securityMap[security]; 
    assests.getObjectMap()[object] = {object};
  }
  else
    throw runtime_error(instruction);

  ref->logInstruction("Object added",instruction);
}


// ========================================================================
// checks the security maps and decides whether the subject has 
// authorization to read from requested object, , updates assests if necessary
// ========================================================================

void ReferenceMonitor::
executeRead(ReferenceMonitor* ref,string& instruction, Assests& assests) {

  string method,subject,object;    // string to hold instruction parameters
  istringstream iss{instruction};  // stringstream to input instructions


  while (!iss.eof()) {
    
    iss >> method >> subject >> object;
    
    if (ref->subjectSecurityLevel.find(subject) == ref->subjectSecurityLevel.end())
      throw runtime_error("(Unknown subject)" + instruction);
    
    if (ref->objectSecurityLevel.find(object) == ref->objectSecurityLevel.end())
      throw runtime_error("(Unknown object)" + instruction);
   
    if (!iss.eof() || iss.get() != iss.eof())
      throw runtime_error("(Too many values)" + instruction);
   
  }
  if (ref->subjectSecurityLevel[subject] >= ref->objectSecurityLevel[object]) {        
    assests.getSubjectMap()[subject].readObject(assests.getObjectMap()[object]);
    ref->logInstruction("Access granted",instruction);
  }
  else 
    ref->logInstruction("Access denied",instruction);    
}


// ========================================================================
// checks the security maps and decides whether the subject has 
// authorization to write to requested object, updates assests if necessary
// ========================================================================

void ReferenceMonitor::
executeWrite (ReferenceMonitor* ref, string& instruction, Assests& assests) {
  
  string method,subject,object, temp;  // string to hold instruction parameters
  istringstream iss{instruction};      // stringstream to input instructions 


  while (!iss.eof()) {
    
    iss >> method >> subject >> object >> temp;
    
    if (ref->subjectSecurityLevel.find(subject) == ref->subjectSecurityLevel.end())
      throw runtime_error("(Unknown Subject)" + instruction);
    
    if (ref->objectSecurityLevel.find(object) == ref->objectSecurityLevel.end())
      throw runtime_error("(Unkown Object)" + instruction);
    
    if (temp =="")
      throw runtime_error("(No value to write)" + instruction);
    
    for (auto c : temp)
      if (c < '0' || c > '9')
        throw runtime_error("(Value is not alpha-numeric)" + instruction);
      
    
    if (!iss.eof() || iss.get() != iss.eof())
      throw runtime_error(instruction);
  }
  if (ref->subjectSecurityLevel[subject] <= ref->objectSecurityLevel[object]) {
    assests.getSubjectMap()[subject].writeObject(assests.getObjectMap()[object], stoi(temp));
    ref->logInstruction("Access granted", subject+" writes value "+temp+" to "+object);
  }
  else
    ref->logInstruction("Access denied",instruction);
}
