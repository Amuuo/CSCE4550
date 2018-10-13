
#include"BLPSecure.h"

int main(int argc, char** argv)
{
  
  ifstream in{argv[1]};
  string line, method;
  
  ReferenceMonitor referenceMonitor{};
  
  
  
  while (in) {
        
    getline(in,line);
    transform(line.begin(),line.end(),line.begin(),::tolower);
    
    istringstream instruction{line};    
    
    instruction >> method;
    Instruction instructionStruct(method,line);
    referenceMonitor.scanInstruction(instructionStruct, assests);

    cout << endl << referenceMonitor.instructionHistory.back();
    
    if (referenceMonitor.instructionHistory.size() % 10 == 0) {
      referenceMonitor.printState(assests);
    }
  }
  
  
  referenceMonitor.printState(assests);  



  string t;
  cin >> t;
}


