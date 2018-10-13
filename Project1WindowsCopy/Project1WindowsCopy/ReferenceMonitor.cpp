#include "ReferenceMonitor.h"



ReferenceMonitor::ReferenceMonitor()
{
}

ReferenceMonitor::ReferenceMonitor(string fileName)
{
  Instruction input;
  ifstream    inputStream{fileName};
  string      instruction;
  
  

  while(!inputStream.eof()) {
    
    try
    {
      getline(inputStream, instruction);
      instructionHistory.push_back(instruction);
      istringstream instructionStream{instruction};
  
      while (!instructionStream.eof())
      {
          instructionStream >> input.function;       

          if (methods.find(input.function)==methods.end()) 
          {
            throw runtime_error("method doesn't exist: " + input.function);
          }
     
          instructionStream >> input.sub >> input.obj;
       

          if (!instructionStream.eof()) 
          {
            instructionStream >> input.value;
            methods[input.function](*this, {input.sub, input.obj, input.value});
          }
          else
          {
            methods[input.function](*this, {input.sub, input.obj});
          }
     
          if (!instructionStream.eof()) 
          {
            throw runtime_error("instruction too long: " + instruction);
          }
        }
    }
    catch(exception& e)
    {
      cout << "\n\nERROR: " << e.what() << endl; 
    }
  }
}


ReferenceMonitor::~ReferenceMonitor(){}

void ReferenceMonitor::printState()
{
}

void ReferenceMonitor::addSubject(vector<string> strings)
{
  cout << "\naddSubjectTest ";
  
  for (auto s:strings)  
    cout << s << " ";
}

void ReferenceMonitor::addObject(vector<string> strings)
{
  cout << "\naddObjectTest ";
  
  for (auto s:strings)  
    cout << s << " ";
}

void ReferenceMonitor::executeRead(vector<string> strings)
{
  cout << "\nexecuteReadTest ";
  
  for (auto s:strings)  
    cout << s << " ";
}

void ReferenceMonitor::executeWrite(vector<string> strings)
{
  cout << "\nexecuteWriteTest ";
  
  for (auto s:strings)  
    cout << s << " ";
  
}
