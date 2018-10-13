#include "ReferenceMonitor.h"



ReferenceMonitor::ReferenceMonitor()
{
}





ReferenceMonitor::ReferenceMonitor(string filename) {
  
  inputFile(filename);
}





ReferenceMonitor::~ReferenceMonitor(){}





void ReferenceMonitor::printState(){}





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
        throw runtime_error("method doesn't exist: " + instructionStruct.function);
      }
      else {
        methods[function](*this,this,inputLine);
      }
    }
    catch (exception& e) {
      cout << "\n\nERROR: " << e.what() << endl;
      instructionHistory.push_back(e.what());
    }
  }
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

  if (ref->subjects.find(subject) == ref->subjects.end()) {
    ref->subjects[subject] = {subject,securityLevel[security]};
  }
  else {
    throw runtime_error("Bad instruction : " + instruction);
  }

  ref->instructionHistory.push_back("Subject Added: " + instruction);
}





void ReferenceMonitor::addObject(ReferenceMonitor* ref,string& instruction) {

  string method,object,security;

  istringstream iss{instruction};

  while (!iss.eof()) {
    iss >> method >> object >> security;

    if (!iss.eof()) {
      throw runtime_error("Bad instruction : " + instruction);
    }
  }

  if (ref->objects.find(object) == ref->objects.end()) {
    ref->objects[object] = {object, securityLevel[security]};    
  }
  else {
    throw runtime_error("Bad instruction: " + instruction);
  }

  ref->instructionHistory.push_back("Object Added: " + instruction);
}





void ReferenceMonitor::executeRead (ReferenceMonitor* ref, string& instruction) {
  
  ref->instructionHistory.push_back("executeReadTest: " + instruction);
}





void ReferenceMonitor::executeWrite (ReferenceMonitor* ref, string& instruction) {
  
  ref->instructionHistory.push_back("executeWriteTest: " + instruction);
}



ReferenceMonitor::Object::Object() {
}

ReferenceMonitor::Object::Object(string id, int _securityLevel) :
  id{id}, _securityLevel{_securityLevel} {}

ReferenceMonitor::Subject::Subject() {
}

ReferenceMonitor::Subject::Subject(string id, int _securityLevel) :
  id{id}, _securityLevel{_securityLevel} {}
