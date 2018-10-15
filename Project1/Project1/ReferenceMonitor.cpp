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
  
  //out << " " << setw(19) << setfill(' ') << left;
  out << "      "/*<< Time{}.getTimeAndDate() */<< message;
    
  //print instruction to screen
  cout << endl << out.str();  
}






// ========================================================================
// really excessive function that prints the current state dynamically 
// based on column width
// CODE IS SO UGLY
// ========================================================================

void ReferenceMonitor::
printState() {
  
  //string LWS((PAGE_WIDTH) / 5,' ');  
  //LWS += "  ";
  const string LWS = "          ";

  ostringstream out{ios::ate};  
  out << LWS << "[;1;46m" << ' ' << string(STATE_BOX_WIDTH,' ') << ' ' << "[0m" << endl;

  const float rowBytes      = out.str().size();
  const float leadingSize   = LWS.size()+9;
  const float columnWidth   = (((rowBytes-5)-(leadingSize)) / 4);
  const float fieldWidth    = columnWidth/2;  
  //const float titlePosition = (((rowBytes-LWS.size()) / 2) - (TITLE.size() / 2));
  
  auto headerPos = [&](int columnNum,string header, int rowNum=0) {
    return ((columnNum*(columnWidth)) + ((columnWidth) / 2) - 
              (header.size()/2)) + leadingSize + (rowNum*rowBytes);

  };
  auto dividerPos = [&](int columnNum, int rowNum=0) {
    return ((columnNum+1)*(columnWidth)) + (rowNum*rowBytes) + leadingSize;
  };
  

  out.seekp(0);
  cout << "\n\n\n";

  //out << LWS << "[47;30m" << '|' << string(STATE_BOX_WIDTH,' ') << '|' << "[0m" << endl;
  out << LWS << "[;1;46m" << ' ' << string(STATE_BOX_WIDTH+(4*16),' ') << ' ' << "[0m";
  //out << LWS << "[47;30m" << '|' << string(STATE_BOX_WIDTH,' ') << '|' << "[0m";

  //out.seekp(titlePosition+LWS.size()+2);
  //out << TITLE;
  
  for (int i = 0, row = 0; i < STATE_BOX_COLUMN_HEADERS.size(); ++i) {
    out.seekp((i*16)+headerPos(i,STATE_BOX_COLUMN_HEADERS[i],row));
    out << "[;1;46m" << STATE_BOX_COLUMN_HEADERS[i] << "[;1;46m";     
  }
    
  cout << "[0m";
  cout << out.str() << endl;
  out.seekp(0);

  for (auto k=(size_t)0,i = (subjectMap.size() >= objectMap.size() ?
                 subjectMap.size() : objectMap.size()); i > 0; --i, ++k) {    
    out << LWS << "[30;47m" << ' ' << string(STATE_BOX_WIDTH,' ') << ' ' << "[0m" << endl;
    int pos = out.tellp();
    /*for (int j = 0; j < 3; ++j) {
      out.seekp(dividerPos(j,k));
      out << '|';
    } */
    out.seekp(pos);
  } 
  //out << LWS << "[47;30m" << '|' << string(STATE_BOX_WIDTH,' ') << '|' << "[0m" << endl;  
  //out << LWS << "[47;30m" << ' ' << string(STATE_BOX_WIDTH,' ') << ' ' << "[0m" << endl;
  
  int i = 0,row = 0;
  for(auto sub = subjectMap.begin(); sub!=subjectMap.end(); ++sub,++i,++row){      
    
    out.seekp(headerPos(0,sub->second.getName(),row));
    out << sub->second.getName();
    out.seekp(headerPos(1,to_string(sub->second.getTemp()),row));
    out << sub->second.getTemp();        

  }
  i,row = 0;

  for(auto obj = objectMap.begin(); obj!=objectMap.end(); ++obj,++i,++row){      
    
    out.seekp(headerPos(2,obj->second.getName(),row));
    out << obj->second.getName();
    out.seekp(headerPos(3,to_string(obj->second.getValue()),row));
    out << obj->second.getValue();
  }        
  cout << out.str() << "\n";    
}


