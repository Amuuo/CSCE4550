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

#include"Instruction.h"
#include"ReferenceMonitor.h"




int main(int argc, char** argv)
{  
  ReferenceMonitor  referenceMonitor{}; // reference monitor class  
  string            inputLine;          // string holds each line of instruction file  
  

  try {            
    
    // check for correct command line input
    if (argc > 2 || argc < 2) 
      throw runtime_error("Usage: ./BLPSecure <instruction file>");    
        
    // log time and input file in log.txt     
    ReferenceMonitor::formatAndOutputLogTitle({"Input File: "+string{argv[1]}}); 
    
    // instruction file input stream
    ifstream in{argv[1]}; 
        
    if (in.fail())  // check input stream integrity
      throw runtime_error("input file failed to open");
    
    
    
    // iterate through input file and handle instructions
    for(int numOfInstructions = 1; !in.eof(); ++numOfInstructions){      
      
      try {        
        getline(in,inputLine);
        
        // constructor validates instruction
        Instruction instruction{inputLine}; 
        referenceMonitor.processRequest(instruction);

        if (numOfInstructions % 10 == 0)
          referenceMonitor.printState();                     
      }

      catch (exception& e) {
        referenceMonitor.printInstructionResult(e.what());
      }      
    } 
  }
  
  catch (exception& e) {
    cout << "\nERROR: " << e.what() << "\n\n";
    exit(1);
  } 
  referenceMonitor.printState();
}


