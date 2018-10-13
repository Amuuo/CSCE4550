#include "ReferenceMonitor.h"



ReferenceMonitor::ReferenceMonitor()
{
}





ReferenceMonitor::ReferenceMonitor(string filename) {
  
  inputFile(filename);
}





ReferenceMonitor::~ReferenceMonitor(){}





void ReferenceMonitor::printState(){
  cout << "\n\n ====== current state =====";
  cout << "\n|== subject ==|== value ===|";
  
  for (auto subject : subjectMap) {
    cout << "\n" << "|   " << subject.first << "   |" 
         << right << setw(10) << subject.second.value << "  |";
  }
  cout << "\n|== object ===|== value ===|";
  for (auto object : objectMap) {
    cout << "\n" << "|   " << object.first << "   |" 
         << right << setw(10) << object.second.value << "  |";
  }
  cout << "\n ==========================\n";
}





void ReferenceMonitor::inputFile(string & filename) {
  Instruction instructionStruct;
  ifstream    inputStream{filename};
  string      inputLine;
  string      function;


  while (!inputStream.eof()) {



    try {
      getline(inputStream,inputLine);

      transform(inputLine.begin(),inputLine.end(),inputLine.begin(),::tolower);
      istringstream instructionStream{inputLine};

      instructionStream >> function;

      if (methods.find(function) == methods.end()) {
        throw runtime_error("method doesn't exist: " + function);
      }
      else {
        methods[function](*this,this,inputLine);
      }
    }
    catch (exception& e) {
      logInstruction("Bad instruction",e.what());
    }
    if (instructionHistory.size() % 10 == 0) {
      printState();
    }
    cout << endl << instructionHistory.back();
  }
  printState();
}





void ReferenceMonitor::logInstruction(string header,string instruction) {
  
  ostringstream out{};
  out << left << setw(16) << header << ": " << left << instruction;
  instructionHistory.push_back(out.str());
}





void ReferenceMonitor::addSubject(ReferenceMonitor* ref, string& instruction) {

  string method,subject,security;


  istringstream iss{instruction};

  while (!iss.eof()) {

    iss >> method >> subject >> security;

    if (!iss.eof()) {
      throw runtime_error("Bad addSub instruction : " + instruction);
    }
  }

  if (ref->subjectMap.find(subject) == ref->subjectMap.end()) {
    ref->subjectMap[subject] = {subject,securityMap[security]};
  }
  else {
    throw runtime_error(instruction);
  }
  ref->logInstruction("Subject added",instruction);
  
}





void ReferenceMonitor::addObject(ReferenceMonitor* ref,string& instruction) {

  string method,object,security;

  istringstream iss{instruction};

  while (!iss.eof()) {
    iss >> method >> object >> security;

    if (!iss.eof()) {
      throw runtime_error(instruction);
    }
  }

  if (ref->objectMap.find(object) == ref->objectMap.end()) {
    ref->objectMap[object] = {object, securityMap[security]};    
  }
  else {
    throw runtime_error(instruction);
  }  
  ref->logInstruction("Object added",instruction);
}





void ReferenceMonitor::executeRead(ReferenceMonitor* ref,string& instruction) {

  istringstream iss{instruction};
  string method,subject,object;  


  while (!iss.eof()) {
    
    iss >> method >> subject >> object;
    
    if (ref->subjectMap.find(subject) == ref->subjectMap.end()) {
      throw runtime_error(instruction);
    }
    if (ref->objectMap.find(object) == ref->objectMap.end()) {
      throw runtime_error(instruction);
    }
    if (iss.get() == iss.eof()) {
      throw runtime_error(instruction);
    }
  }
  
  if (ref->subjectMap[subject].securityLevel >= ref->objectMap[object].securityLevel) {        
    ref->subjectMap[subject].value = ref->objectMap[object].value;    
    
    ref->logInstruction("Access granted",instruction);

  }
  else {
    ref->logInstruction("Access denied",instruction);    
  }

  

}





void ReferenceMonitor::executeWrite (ReferenceMonitor* ref, string& instruction) {
  
  istringstream iss{instruction};
  string method,subject,object, value;  


  while (!iss.eof()) {
    
    iss >> method >> subject >> object >> value;
    
    if (ref->subjectMap.find(subject) == ref->subjectMap.end()) {
      throw runtime_error(instruction);
    }
    if (ref->objectMap.find(object) == ref->objectMap.end()) {
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
  
  if (ref->subjectMap[subject].securityLevel <= ref->objectMap[object].securityLevel) {
    ref->objectMap[object].value = value;    
    ref->logInstruction("Access granted", subject+" writes value "+value+" to "+object);
  }
  else {
    ref->logInstruction("Access denied",instruction);    
  }
}



ReferenceMonitor::Object::Object() {
}

ReferenceMonitor::Object::Object(string id, int securityLevel) :
  id{id}, securityLevel{securityLevel} {}

ReferenceMonitor::Subject::Subject() {
}

ReferenceMonitor::Subject::Subject(string id, int securityLevel) :
  id{id}, securityLevel{securityLevel} {}
