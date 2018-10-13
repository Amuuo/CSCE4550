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

#include"Assests.h"
#include"Instruction.h"
#include"ReferenceMonitor.h"


int main(int argc, char** argv)
{
  
  ReferenceMonitor  referenceMonitor{}; // reference monitor class  
  Assests           assests{};          // class holds all subjects and objects 
  string            inputLine;          // string holds each line of instruction file  
  

  
  // iterate through input file and handle instructions
  try {
    
    ifstream in{argv[1]}; // instruction file input stream
    
    while (!in.eof()) {

      getline(in,inputLine);

      transform(inputLine.begin(),inputLine.end(),
                inputLine.begin(),::tolower);

      referenceMonitor.scanInstruction({inputLine},assests);

      if (referenceMonitor.instructionHistory.size() % 10 == 0)
        referenceMonitor.printState(assests);      
    }
    referenceMonitor.printState(assests);
  }
  catch (exception& e) {
    cout << "Error: " << e.what();
    exit(1);
  }
}


