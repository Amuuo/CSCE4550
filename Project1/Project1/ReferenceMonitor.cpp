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

#include "ReferenceMonitor.h"

ReferenceMonitor::ReferenceMonitor() {
}

ReferenceMonitor::~ReferenceMonitor() {
}

// ========================================================================
// check if instruction method exists and invoke the necessary function
// ========================================================================

void ReferenceMonitor::
processRequest(Instruction& ins) {

  if (ins.method == "addsub" || ins.method == "addobj") {
    
    if (securityMap.find(ins.security) == securityMap.end()) {
      
      throw runtime_error(Instruction::constructMsg(
          "!!BAD INSTRUCTION-->UNKNOWN SECURITY CLEARANCE",
          ins.instruction,B_RED + WHITE));
    }
  }
  if (ins.method == "write" || ins.method == "read") {
    
    if (subjectSecurityLevel.find(ins.subject) == subjectSecurityLevel.end() ||
        subjectMap.find(ins.subject) == subjectMap.end()) {
      
      throw runtime_error(Instruction::constructMsg(
          "!!BAD INSTRUCTION-->UNKNOWN SUBJECT",ins.instruction,B_RED + WHITE));
    }
    if (objectSecurityLevel.find(ins.object) == objectSecurityLevel.end() ||
        objectMap.find(ins.object) == objectMap.end()) {
      
      throw runtime_error(Instruction::constructMsg(
          "!!BAD INSTRUCTION-->UNKNOWN OBJECT",ins.instruction,B_RED + WHITE));
    }
  }
  methods[ins.method](this,ins);   
}




// ========================================================================
// function adds subject to the reference monitor subjectSecurityMap with 
// name and security level as well as the Assests class with just the name
// ========================================================================

void ReferenceMonitor::
addSubject(Instruction& ins) {
 
  if (subjectSecurityLevel.find(ins.subject) == subjectSecurityLevel.end()) {
    
    subjectSecurityLevel[ins.subject] = securityMap[ins.security];
    subjectMap[ins.subject] = {ins.subject};
  }
  else {
    throw runtime_error(Instruction::constructMsg(
      "!!BAD INSTRUCTION-->SUBJECT ALREADY EXISTS",ins.instruction,RED + BOLD));
  }

  printInstructionResult(
    Instruction::constructMsg("SUBJECT ADDED", ins.instruction, GREEN+BOLD));
}




// ========================================================================
// function adds object to the reference monitor objectSecurityMap with 
// name and security level as well as the Assests class with just the name
// ========================================================================

void ReferenceMonitor::
addObject(Instruction& ins) {

  if (objectSecurityLevel.find(ins.object) == objectSecurityLevel.end()) {
    
    objectSecurityLevel[ins.object] = securityMap[ins.security]; 
    objectMap[ins.object] = {ins.object};
  }
  else {
    throw runtime_error(Instruction::constructMsg(
      "!!BAD INSTRUCTION-->OBJECT ALREADY EXISTS",ins.instruction,B_RED + WHITE));
  }

  printInstructionResult(Instruction::constructMsg(
    "OBJECT ADDED", ins.instruction, GREEN+BOLD));
}




// ========================================================================
// checks the security maps and decides whether the subject has 
// authorization to read from requested object, , updates assests if necessary
// ========================================================================

void ReferenceMonitor::
executeRead(Instruction& ins) {

  if (subjectSecurityLevel[ins.subject] >= objectSecurityLevel[ins.object]) {
    subjectMap[ins.subject].readObject(objectMap[ins.object]);
    
    printInstructionResult(Instruction::constructMsg(
      "READ ACCESS GRANTED", ins.subject+" reads "+ins.object, GREEN+BOLD));
  }
  else {
    printInstructionResult(Instruction::constructMsg(
      "READ ACCESS DENIED",ins.instruction,RED + BOLD));
  }
}




// ========================================================================
// checks the security maps and decides whether the subject has 
// authorization to write to requested object, updates assests if necessary
// ========================================================================

void ReferenceMonitor::
executeWrite(Instruction& ins) {
   
  if (subjectSecurityLevel[ins.subject] <= objectSecurityLevel[ins.object]) {
    subjectMap[ins.subject].writeObject(objectMap[ins.object],ins.value);

    printInstructionResult(Instruction::constructMsg(
      "WRITE ACCESS GRANTED", ins.subject+" writes value "+ 
        to_string(ins.value)+" to "+ins.object, GREEN+BOLD));
  }
  else {
    printInstructionResult(Instruction::constructMsg(
      "WRITE ACCESS DENIED",ins.instruction,RED + BOLD));
  }
}





// ========================================================================
// prints the results of each instruction request
// ========================================================================

inline void ReferenceMonitor::
printInstructionResult(string message) {
  
  ostringstream out{};  // ostringstream to format instruction log
  
  out << "   " << setw(21) << setfill(' ') << left;
  out << Time{}.getTimeAndDate() << message;
    
  //print instruction to screen
  cout << endl << out.str();
  
  //print instruction to output log.txt
  ofstream{"log.txt",ios::app} << endl << out.str();
  
}




void ReferenceMonitor::formatAndOutputLogTitle(string inputFile) {

  ostringstream out{};

  int borderLength = inputFile.size()>=PAGE_WIDTH ? (inputFile.size()+2) : PAGE_WIDTH+2;  
  int timeLeadingSpace = (borderLength-24)/2;  
  int inputFileLeadingSpace = borderLength>=PAGE_WIDTH+2 ? 
      (borderLength-inputFile.size())/2 : (borderLength%(PAGE_WIDTH+2))/2;
  
  out << "\n\n\n\n\n";
  out << setw(borderLength) << setfill('=') << " " << "\n";
  out << string(timeLeadingSpace,' ') + Time{}.getFullTime();
  out << setw(borderLength) << setfill('=') << " " << "\n";
  out << setw(borderLength) << setfill(' ') << left; 
  out << string(inputFileLeadingSpace,' ') + inputFile << "\n";
  out << setw(borderLength) << setfill('-') << right << " ";
  out << "\n\n\n";

  cout << endl << out.str();
  ofstream{"log.txt",ios::app} << endl << out.str();  
}



// ========================================================================
// prints the current state of the objects and subjects in Assests struct
// ========================================================================

void ReferenceMonitor::
printState() {

  string lws((PAGE_WIDTH-24)/3,' ');
  vector<string> columns = {"subject","temp","object","value"};

  const string title = "CURRENT STATE";
  int pageWidth   = 65;
    
  cout << endl << endl << endl;
  ostringstream out{ios::ate};
  out << lws << '+' << string(pageWidth,'=') << '+' << endl; int pos = out.tellp();

  int rowBytes = out.str().size();
  int columnWidth = (rowBytes-lws.size()) / 4;
  int fieldWidth  = columnWidth/2;  
  int titlePosition = (((rowBytes-lws.size()) / 2) - (title.size() / 2));

  auto headerPos = [&](int columnNum,string header) {
    return ((columnNum*columnWidth) + (columnWidth / 2) - 
              (header.size() / 2)) + lws.size();
  };
  
  out << lws << '|' << string(pageWidth,' ') << '|' << endl; pos = out.tellp();
  out << lws << '|' << string(pageWidth,' ') << '|' << endl; pos = out.tellp();
  out << lws << '|' << string(pageWidth,' ') << '|'; pos = out.tellp();

  
  out.seekp(titlePosition+lws.size());
  out << title;
  
  int row = 2;
  for (int i = 0; i < columns.size(); ++i) {
    out.seekp((row*rowBytes)+headerPos(i,columns[i]));
    out << columns[i]; 
  }
 
  cout << out.str();
  out.seekp(0);
  

  out << endl;

  for (auto i = (subjectMap.size() >= objectMap.size() ?
                 subjectMap.size() : objectMap.size()); i > 0; --i) {
    
    out << lws << '|' << string(pageWidth,' ') << '|' << endl;    
  }
  out << lws << '|' << string(pageWidth,' ') << '|' << endl;  
  out << lws << '+' << string(pageWidth,'=') << '+' << endl;
  out << endl;
  
  int i = 0;
  row = 0;
  for(auto sub : subjectMap){    
    
    out.seekp(1+(row*rowBytes)+headerPos(0, sub.second.getName()));
    out << sub.second.getName();
    out.seekp(1+(row*rowBytes)+headerPos(1, to_string(sub.second.getTemp())));
    out << sub.second.getTemp();
    ++i;
    ++row;
  }
  i = 0;
  row = 0;
  for (auto obj : objectMap) {
    
    out.seekp(1+(row*rowBytes)+headerPos(2, obj.second.getName()));
    out << obj.second.getName();
    out.seekp(1+(row*rowBytes)+headerPos(3, to_string(obj.second.getValue())));
    out << obj.second.getValue();
    ++i;
    ++row;
  }  
      
  cout << out.str();  
  
}


