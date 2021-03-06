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




int main(int argc, char** argv)
{
  
  ReferenceMonitor  referenceMonitor{}; // reference monitor class
  ifstream          in;                 // instruction file input stream
  Assests           assests{};          // class holds all subjects and objects 
  string            inputLine;          // string holds each line of instruction file  
  


  try {
    
    in.open(argv[1]);

    while (!in.eof()) {

      getline(in,inputLine);
      transform(inputLine.begin(),inputLine.end(),inputLine.begin(),::tolower);

      Instruction instructionStruct{inputLine};

      referenceMonitor.scanInstruction(instructionStruct,assests);

      cout << endl << referenceMonitor.instructionHistory.back();

      if (referenceMonitor.instructionHistory.size() % 10 == 0) {
        referenceMonitor.printState(assests);
      }
    }


    referenceMonitor.printState(assests);
  }
  
  catch (exception& e) {
    cout << "Error: " << e.what();
    exit(1);
  }
}


