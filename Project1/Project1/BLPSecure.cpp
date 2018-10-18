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
#include<thread>
#include<mutex>
#include<array>
#include<condition_variable>
#include<deque>


string title = "\n\n\n\n\n\n"
"                [37;42m +------------------------------------------------+ [0m\n"
"                [37;42m |        Computer Sceince and Engineering        | [0m\n"
"                [37;42m |          CSCE 4550 - Computer Security         | [0m\n"
"                [37;42m | Adam Williams arw0174 adamwilliams2@my.unt.edu | [0m\n"
"                [37;42m +------------------------------------------------+ [0m\n";
  



int main(int argc, char** argv)
{  
  ReferenceMonitor  referenceMonitor{}; // reference monitor class
  string            inputLine;          // string holds each line of instruction file
  thread            headerThread[5];
  vector<string>    instructionHistory{};


  try 
  {    
    system("clear");

    // check for correct command line input
    if (argc > 2 || argc < 2) 
      throw runtime_error("Usage: ./BLPSecure <instruction file>");

    // instruction file input stream
    ifstream in{argv[1]};
    title += string("             [30;47m Input file : " + string(argv[1]) + " [0m");
    
    
    // check input stream integrity
    if (in.fail())
      throw runtime_error("input file failed to open");

    // iterate through input file and handle instructions

    for (int numOfInstructions = 1; !in.eof(); ++numOfInstructions)
    {           
      try 
      {
        getline(in,inputLine);
        instructionHistory.push_back(inputLine);
        // constructor validates instruction
        Instruction instruction{inputLine};
        referenceMonitor.processRequest(instruction);        
      }
      catch (exception& e) 
      {
        referenceMonitor.printResult(e.what());        
      }      
      if (numOfInstructions % 10 == 0) 
      {
        //referenceMonitor.printState();
      }
    }
  }
  catch (exception& e) 
  {
    cout << "\nERROR: " << e.what() << "\n\n";
    exit(1);
  }
  cout << "[18h";
  system("clear");
  char c = '1';
  deque<string> q;


  for (int i = 0; i < referenceMonitor.instructionResultHistory.size(); ++i) {
    cout << "7";
    cout << title;
    cout<<"\n\n\n";
    cout<<"                   "<<"[2K"<<"[5m"<<B_WHITE << BLACK
        << "Instruction:"<<RESET<<" "<< B_MAGENTA << YELLOW << BOLD
        << " " << instructionHistory[i] << " " << RESET;
    
    usleep(50000);
    
    cout << "\n\n\n";
    
    if (i>=15)
    {
      q.push_front(referenceMonitor.instructionResultHistory[i]);
      for (auto line:q)
      {
        cout<<"[0K"<<line;
      }
      q.pop_back();
    }
    else
    {
      q.push_front(referenceMonitor.instructionResultHistory[i]);
      for (auto line:q)
      {
        cout<<"[0K"<<line;
      }
    }
    cout << "[34;3H";            
    cout << referenceMonitor.stateHistory[i];
    cout << "8";
  }
}


