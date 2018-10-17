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
#include<unistd.h>

void printHeader(string filename) {
  cout << "\n\n\n\n"
      "       [37;42m +------------------------------------------------+ [0m\n"
      "       [37;42m |        Computer Sceince and Engineering        | [0m\n"
      "       [37;42m |          CSCE 4550 - Computer Security         | [0m\n"
      "       [37;42m | Adam Williams arw0174 adamwilliams2@my.unt.edu | [0m\n"
      "       [37;42m +------------------------------------------------+ [0m\n\n";
  cout << "        [30;47m Input file : " << filename << " [0m\n\n\n\n";
}

int main(int argc, char** argv)
{  

  ReferenceMonitor  referenceMonitor{}; // reference monitor class
  string            inputLine;          // string holds each line of instruction file

  try 
  {    
    system("clear");

    // check for correct command line input
    if (argc > 2 || argc < 2) 
      throw runtime_error("Usage: ./BLPSecure <instruction file>");
    
    printHeader(argv[1]);
    
    // instruction file input stream
    ifstream in{argv[1]};
       
    // check input stream integrity
    if (in.fail())
      throw runtime_error("input file failed to open");

    // iterate through input file and handle instructions
    for(int numOfInstructions = 1; !in.eof(); ++numOfInstructions)
    {       
      try 
      {
        getline(in,inputLine);
        
        // constructor validates instruction
        Instruction instruction{inputLine};
        referenceMonitor.processRequest(instruction);        
      }
      catch (exception& e) 
      {
        referenceMonitor.printResult(e.what());        
      }      
      if (numOfInstructions % 10 == 0) 
        referenceMonitor.printState();      
    }
  }
  catch (exception& e) 
  {
    cout << "\nERROR: " << e.what() << "\n\n";
    exit(1);
  }
  referenceMonitor.printState();    
}


