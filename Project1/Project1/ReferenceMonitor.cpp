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




// ========================================================================
// check if instruction method exists and invoke the necessary function
// ========================================================================

void ReferenceMonitor::
processRequest(Instruction& ins) {

  if (ins.method == "addsub" || ins.method == "addobj") {
    
    if (securityMap.find(ins.security) == securityMap.end()) {
      
      throw runtime_error(ins.constructMsg(
          "!!BAD INSTRUCTION-->UNKNOWN SECURITY CLEARANCE", B_RED+WHITE));
    }
  }
  if (ins.method == "write" || ins.method == "read") {
    
    if (subjectSecurityLevel.find(ins.subject) == subjectSecurityLevel.end() ||
        subjectMap.find(ins.subject) == subjectMap.end()) {
      
      throw runtime_error(ins.constructMsg(
          "!!BAD INSTRUCTION-->UNKNOWN SUBJECT", B_RED+WHITE));
    }
    if (objectSecurityLevel.find(ins.object) == objectSecurityLevel.end() ||
        objectMap.find(ins.object) == objectMap.end()) {
      
      throw runtime_error(
        ins.constructMsg("!!BAD INSTRUCTION-->UNKNOWN OBJECT", B_RED+WHITE));
    }
  }
  methods[ins.method](this,ins);   
}




// ========================================================================
// function adds subject to the reference monitor subjectSecurityMap with 
// name and security level as well as the subject map with just the name
// ========================================================================

void ReferenceMonitor::
addSubject(Instruction& ins) {
 
  if (subjectSecurityLevel.find(ins.subject) == subjectSecurityLevel.end()) {
    
    subjectSecurityLevel[ins.subject] = securityMap[ins.security];
    subjectMap[ins.subject] = {ins.subject};
  }
  else {
    throw runtime_error(ins.constructMsg(
      "!!BAD INSTRUCTION-->SUBJECT ALREADY EXISTS", RED+BOLD));
  }

  printResult(
    ins.constructMsg("SUBJECT ADDED", GREEN+BOLD));
}




// ========================================================================
// function adds object to the reference monitor objectSecurityMap with 
// name and security level as well as the objects map with just the name
// ========================================================================

void ReferenceMonitor::
addObject(Instruction& ins) {

  if (objectSecurityLevel.find(ins.object) == objectSecurityLevel.end()) {
    
    objectSecurityLevel[ins.object] = securityMap[ins.security]; 
    objectMap[ins.object] = {ins.object};
  }
  else {
    throw runtime_error(ins.constructMsg(
      "!!BAD INSTRUCTION-->OBJECT ALREADY EXISTS", B_RED+WHITE));
  }

  printResult(ins.constructMsg("OBJECT ADDED", GREEN+BOLD));
}




// ========================================================================
// checks the security maps and decides whether the subject has 
// authorization to read from requested object. Allows subject access
// if authorized 
// ========================================================================

void ReferenceMonitor::
verifyReadAccess(Instruction& ins) {

  if (subjectSecurityLevel[ins.subject] >= objectSecurityLevel[ins.object]) {
    
    subjectMap[ins.subject].readObject(objectMap[ins.object]);
    
    printResult(ins.constructMsg("READ ACCESS GRANTED", GREEN+BOLD,
      ins.subject+" reads "+ins.object));
  }
  else {
    printResult(ins.constructMsg("READ ACCESS DENIED", RED+BOLD));
  }
}




// ========================================================================
// checks the security maps and decides whether the subject has 
// authorization to write to requested object. Allows subject access
// if authorized 
// ========================================================================

void ReferenceMonitor::
verifyWriteAccess(Instruction& ins) {
   
  if (subjectSecurityLevel[ins.subject] <= objectSecurityLevel[ins.object]) {
    
    subjectMap[ins.subject].writeObject(objectMap[ins.object],ins.value);

    printResult(ins.constructMsg("WRITE ACCESS GRANTED", GREEN+BOLD,
      ins.subject+" writes value "+ to_string(ins.value)+" to "+ins.object));
  }
  else {
    printResult(ins.constructMsg("WRITE ACCESS DENIED", RED+BOLD));
  }
}





// ========================================================================
// prints the results of each instruction request 
// ========================================================================

inline void ReferenceMonitor::
printResult(string message) {
        
  cout << string("",3) <<  message << endl;
}





// ========================================================================
// really excessive function that prints the current state dynamically 
// based on STATE_BOX_WIDTH variable on "ReferenceMonitor.h"
// ** CODE IS VERY UGLY AND BRITTLE BUT IT WORKS **
// ========================================================================

void ReferenceMonitor::
printState() {
    
  const string LWS = string("",5);

  ostringstream out{ios::ate};  
  out << LWS << "[;1;46m" << ' ' << string(STATE_BOX_WIDTH,' ') << ' ' << "[0m" << endl;

  const float rowBytes      = out.str().size();
  const float leadingSize   = LWS.size()+9;
  const float columnWidth   = (((rowBytes-5)-(leadingSize)) / 4);
  const float fieldWidth    = columnWidth/2;  
    
  auto headerPos = [&](int columnNum,string header, int rowNum=0) {
    return ((columnNum*(columnWidth)) + ((columnWidth) / 2) - 
              (header.size()/2)) + leadingSize + (rowNum*rowBytes);

  };
  auto dividerPos = [&](int columnNum, int rowNum=0) {
    return ((columnNum+1)*(columnWidth)) + (rowNum*rowBytes) + leadingSize;
  };
  
  out.seekp(0);
  cout << "\n";
  
  out << LWS << "[33;44m" << ' ' << string(STATE_BOX_WIDTH,'-') << ' ' << "[0m" << endl;
  out << LWS << "[33;44m" << ' ' << string(STATE_BOX_WIDTH+(4*16),' ') << ' ' << "[0m" << endl;
  out << LWS << "[33;44m" << ' ' << string(STATE_BOX_WIDTH,' ') << ' ' << "[0m";
  for (int i = 0, row = 1; i < STATE_BOX_COLUMN_HEADERS.size(); ++i) {
    out.seekp((i*16)+headerPos(i,STATE_BOX_COLUMN_HEADERS[i],row));
    out << "[37;44m" << STATE_BOX_COLUMN_HEADERS[i] << "[;1;44m";
  }
    
  cout << "[0m";
  cout << out.str() << endl;
  out.seekp(0);

  for (auto i = (subjectMap.size() >= objectMap.size() ?
                 subjectMap.size() : objectMap.size()); i > 0; --i) {
    out << LWS << "[33;44m" << ' ' << string(STATE_BOX_WIDTH,' ') << ' ' << "[0m" << endl;
    int pos = out.tellp();
    out.seekp(pos);
  }
  out << LWS << "[33;44m" << ' ' << string(STATE_BOX_WIDTH,'-') << ' ' << "[0m" << endl;
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


