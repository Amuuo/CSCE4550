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
        
  cout << "   " <<  message << endl;
}





// ========================================================================
// really excessive function that prints the current state dynamically 
// based on STATE_BOX_WIDTH variable on "ReferenceMonitor.h"
// ** CODE IS VERY UGLY AND BRITTLE BUT IT WORKS **
// ========================================================================

void ReferenceMonitor::
printState() {
    
  const string   LWS = "       ";
  ostringstream  out{ios::ate};    
  vector<string> headerLines;
  vector<string> bodyLines;
  const float    columnWidth = STATE_BOX_WIDTH / 4;
        
  auto headerPos = [&](int columnNum,string header, int rowNum=0) 
  {
    return ((columnNum*(columnWidth)) + ((columnWidth) / 2) - 
              (header.size()/2)) + (rowNum*STATE_BOX_WIDTH);
  };
	
  out << string(STATE_BOX_WIDTH,' ');      
  for (int i = 0, row = 0; i < STATE_BOX_COLUMN_HEADERS.size(); ++i) 
  {
    out.seekp(headerPos(i,STATE_BOX_COLUMN_HEADERS[i],row));
    out << STATE_BOX_COLUMN_HEADERS[i];    
  }  
  headerLines.push_back(out.str());
  
  out.seekp(0);
  out << string(STATE_BOX_WIDTH,' ');              
  bodyLines.push_back(out.str());
  
  auto obj = objectMap.begin();
  auto sub = subjectMap.begin();  

  for (int i = objectMap.size()<subjectMap.size() ?
                subjectMap.size():objectMap.size(); i > 0;--i)
  { 
    out.seekp(0);
    out << string(STATE_BOX_WIDTH,' ');
    
    if (sub != subjectMap.end()) 
    {
      out.seekp(headerPos(0,sub->second.getName()));
      out << sub->second.getName();
      out.seekp(headerPos(1,to_string(sub->second.getTemp())));
      out << sub->second.getTemp();
      ++sub;
    }
    if (obj != objectMap.end()) 
    {
      out.seekp(headerPos(2,obj->second.getName()));
      out << obj->second.getName();
      out.seekp(headerPos(3,to_string(obj->second.getValue())));
      out << obj->second.getValue();
      ++obj;
    }
    bodyLines.push_back(out.str());            
  }  
  out.seekp(0);
  out << BOLD << B_BLACK << WHITE << string(STATE_BOX_WIDTH,' ') << RESET;
  bodyLines.push_back(out.str());


  cout << "\n\n";
  
  for (auto line : headerLines) {
    cout << LWS << B_WHITE << "  ";
    cout << BLACK << B_WHITE<< line;
    cout << B_WHITE << "  " << RESET << endl;
  }
  
  for (auto line : bodyLines) {
    cout << LWS << B_WHITE << "  ";
    cout << BOLD << WHITE << B_BLACK << line;
    cout << B_WHITE << "  " << RESET << endl;
  }
    
  
  out.seekp(0);
  out << LWS << BOLD << B_WHITE << string(STATE_BOX_WIDTH+4,' ') << RESET;
  bodyLines.push_back(out.str());
  cout << out.str();

  cout << "\n\n\n";
}


