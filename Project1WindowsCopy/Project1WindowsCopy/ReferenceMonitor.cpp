#include "ReferenceMonitor.h"



ReferenceMonitor::ReferenceMonitor()
{
}








ReferenceMonitor::~ReferenceMonitor(){}





void ReferenceMonitor::printState(Assests& assests){
  cout << "\n\n ====== current state =====";
  cout << "\n|== subject ==|== value ===|";
  
  for (auto subject : assests.subjects) {
    cout << "\n" << "|   " << subject.first << "   |" 
         << right << setw(10) << subject.second.value << "  |";
  }
  cout << "\n|== object ===|== value ===|";
  for (auto object : assests.objects) {
    cout << "\n" << "|   " << object.first << "   |" 
         << right << setw(10) << object.second.value << "  |";
  }
  cout << "\n ==========================\n";
}





void ReferenceMonitor::scanInstruction(Instruction& instruction, Assests& assests) {

  try {
    if (methods.find(instruction.function) == methods.end()) {
      throw runtime_error("method doesn't exist: " + instruction.function);
    }
    else {
      methods[instruction.function](*this,this,instruction.instruction, assests);
    }
  }
  catch (exception& e) {
    logInstruction("Bad instruction",e.what());
  }
}





void ReferenceMonitor::logInstruction(string header,string instruction) {
  
  ostringstream out{};
  out << left << setw(16) << header << ": " << left << instruction;
  instructionHistory.push_back(out.str());
}





void ReferenceMonitor::addSubject(ReferenceMonitor* ref, string& instruction, Assests& assests) {

  string method,subject,security;


  istringstream iss{instruction};

  while (!iss.eof()) {

    iss >> method >> subject >> security;

    if (!iss.eof()) {
      throw runtime_error("Bad addSub instruction : " + instruction);
    }
  }

  if (ref->subjectSecurityLevel.find(subject) == ref->subjectSecurityLevel.end()) {
    ref->subjectSecurityLevel[subject] = ref->securityMap[security];
    assests.subjects[subject];
  }
  else {
    throw runtime_error(instruction);
  }
  ref->logInstruction("Subject added",instruction);
  
}





void ReferenceMonitor::addObject(ReferenceMonitor* ref,string& instruction, Assests& assests) {

  string method,object,security;

  istringstream iss{instruction};

  while (!iss.eof()) {
    iss >> method >> object >> security;

    if (!iss.eof()) {
      throw runtime_error(instruction);
    }
  }

  if (ref->objectSecurityLevel.find(object) == ref->objectSecurityLevel.end()) {
    ref->objectSecurityLevel[object] = ref->securityMap[security]; 
    assests.objects[object];
  }
  else {
    throw runtime_error(instruction);
  }  
  ref->logInstruction("Object added",instruction);
}





void ReferenceMonitor::executeRead(ReferenceMonitor* ref,string& instruction, Assests& assests) {

  istringstream iss{instruction};
  string method,subject,object;  


  while (!iss.eof()) {
    
    iss >> method >> subject >> object;
    
    if (ref->subjectSecurityLevel.find(subject) == ref->subjectSecurityLevel.end()) {
      throw runtime_error(instruction);
    }
    if (ref->objectSecurityLevel.find(object) == ref->objectSecurityLevel.end()) {
      throw runtime_error(instruction);
    }
    if (iss.get() == iss.eof()) {
      throw runtime_error(instruction);
    }
  }
  
  if (ref->subjectSecurityLevel[subject] >= ref->objectSecurityLevel[object]) {        
    assests.subjects[subject].value = assests.objects[object].value;
    ref->logInstruction("Access granted",instruction);
  }
  else {
    ref->logInstruction("Access denied",instruction);    
  }
}





void ReferenceMonitor::executeWrite (ReferenceMonitor* ref, string& instruction, Assests& assests) {
  
  istringstream iss{instruction};
  string method,subject,object, value;  


  while (!iss.eof()) {
    
    iss >> method >> subject >> object >> value;
    
    if (ref->subjectSecurityLevel.find(subject) == ref->subjectSecurityLevel.end()) {
      throw runtime_error(instruction);
    }
    if (ref->objectSecurityLevel.find(object) == ref->objectSecurityLevel.end()) {
      throw runtime_error(instruction);
    }
    if (!iss.eof()) {
      throw runtime_error(instruction);
    }
    if (value =="") {
      throw runtime_error(instruction);
    }
    for (auto c : value) {
      if (c < '0' || c > '9') {
        throw runtime_error(instruction);
      }
    }
  }

  ostringstream out{};
  
  if (ref->subjectSecurityLevel[subject] <= ref->objectSecurityLevel[object]) {
    assests.objects[object].value = stoi(value);
    ref->logInstruction("Access granted", subject+" writes value "+value+" to "+object);
  }
  else {
    ref->logInstruction("Access denied",instruction);    
  }
}
