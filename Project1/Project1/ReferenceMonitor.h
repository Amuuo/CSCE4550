#pragma once
#include"Assests.h"
#include"Instruction.h"
#include<map>
#include<functional>
#include<utility>
#include<algorithm>
#include<sstream>
#include<vector>
#include<stdexcept>
#include<fstream>
#include<string>
#include<iostream>
#include<iomanip>

using namespace std;

enum SecurityLevels {LOW,MEDIUM,HIGH};


class ReferenceMonitor {

  using functionMap = map<string,function<void(const ReferenceMonitor,
                             ReferenceMonitor*,string&,Assests&)>>;      
  public:
  ReferenceMonitor() {}
  ~ReferenceMonitor() {}

  void printState      (Assests&);
  void scanInstruction (Instruction, Assests&);
  void logInstruction  (string, string);
  void addSubject      (ReferenceMonitor*,string&, Assests&);
  void addObject       (ReferenceMonitor*,string&, Assests&);
  void executeRead     (ReferenceMonitor*,string&, Assests&);
  void executeWrite    (ReferenceMonitor*,string&, Assests&);
  
  
  vector<string>  instructionHistory{};   //vector logs all instruction results
  map<string,int> subjectSecurityLevel{}; //map stores subjects security clearance
  map<string,int> objectSecurityLevel{};  //map stores objects security level  
  
  //map stores the security level value with a string key
  map<string,int> securityMap{{"low",LOW},{"medium",MEDIUM},{"high",HIGH}};

  //map stores functions functions called for by the input
  functionMap methods {{"addobj", &ReferenceMonitor::addObject}, 
                       {"addsub", &ReferenceMonitor::addSubject}, 
                       {"read"  , &ReferenceMonitor::executeRead}, 
                       {"write" , &ReferenceMonitor::executeWrite}};  
};

