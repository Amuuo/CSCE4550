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

#pragma once

#include"Assests.h"
#include"Instruction.h"
#include"ReferenceMonitor.h"

Assests assests{};


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
}


