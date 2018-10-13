#include "ReferenceMonitor.h"



ReferenceMonitor::ReferenceMonitor()
{
}

ReferenceMonitor::ReferenceMonitor(string fileName)
{
  Instruction input;
  ifstream    inputStream{fileName};
  string      instruction;
  
  

  while(inputStream) {
    
    getline(inputStream, instruction);
    instructionHistory.push_back(instruction);
    istringstream instructionStream{instruction};
  
  
    while (instructionStream)
    {
       instructionStream >> input.function;       

       if (methods.find(input.function)==methods.end()) {
         throw runtime_error("method doesn't exist");
       }
     
       instructionStream >> input.sub >> input.obj;
       
       methods[input.function](*this, input.sub);

       if (instructionStream) {
         instructionStream >> input.value;
       }
     
       if (instructionStream) {
         throw runtime_error("bad instruction");
       }
    }
  }
}


ReferenceMonitor::~ReferenceMonitor()
{
}

void ReferenceMonitor::printState()
{
}

void ReferenceMonitor::addSubject(string s)
{
  cout << "addSubjectTest" << s;
}

void ReferenceMonitor::addObject(string s)
{
  cout << "addObjectTest" << s;
}

void ReferenceMonitor::executeRead(string s)
{
  cout << "executeReadTest" << s;
}

void ReferenceMonitor::executeWrite(string s)
{
  cout << "executeWriteTest" << s;
}
