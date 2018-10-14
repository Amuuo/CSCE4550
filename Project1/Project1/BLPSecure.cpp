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
    
    if (argc > 2 || argc < 2) // check for correct input
      throw runtime_error("Usage: ./BLPSecure <instruction file>");    
    
    ReferenceMonitor::logTitle({"Input File: "+string{argv[1]}}); // log time and input file in log.txt     
    ifstream in{argv[1]}; // instruction file input stream
        
    if (in.fail())  // check input stream integrity
      throw runtime_error("input file failed to open");
    
    
    while (!in.eof()) { // iterate through input file and handle instructions
      
      try {

        static int numOfInstructions = 1;
        getline(in,inputLine);
        Instruction instruction{inputLine}; // constructor validates instruction
        referenceMonitor.scanInstruction(instruction,assests);

        if (numOfInstructions % 10 == 0)
          referenceMonitor.printState(assests);     
        
        ++numOfInstructions;
      }
      catch (exception& e) {
        referenceMonitor.logResult(e.what(), " ");
      }      
    } 
  }
  catch (exception& e) {
    cout << "\nERROR: " << e.what() << "\n\n";
    exit(1);
  } 
  referenceMonitor.printState(assests);
}


