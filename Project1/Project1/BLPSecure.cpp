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
  

  try {            
    
    // check for correct input
    if (argc > 2 || argc < 2)
      throw runtime_error("\nUsage: ./BLPSecure <instruction file>");    
    
    // instruction file input stream
    ifstream in{argv[1]};
    
    // check input stream integrity
    if (in.fail()) 
      throw runtime_error("input file failed to open");
    
    // iterate through input file and handle instructions
    while (!in.eof()) {

      getline(in,inputLine);
      referenceMonitor.scanInstruction({inputLine},assests);

      if (referenceMonitor.instructionHistory.size() % 10 == 0)
        referenceMonitor.printState(assests);      
    }    
  }
  catch (exception& e) {
    cout << "\nError: " << e.what() << endl;
    exit(1);
  }
  referenceMonitor.printState(assests);
}


