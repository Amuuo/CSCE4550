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


class ReferenceMonitor
{
  public:

  ReferenceMonitor() {}
  ~ReferenceMonitor() {}

  map<string,function<void(const ReferenceMonitor, ReferenceMonitor*, string&, Assests&)>> methods {
    {"addobj", &ReferenceMonitor::addObject}, 
    {"addsub", &ReferenceMonitor::addSubject}, 
    {"read",   &ReferenceMonitor::executeRead}, 
    {"write",  &ReferenceMonitor::executeWrite}
  };  
  
  vector<string> instructionHistory{};
  
  map<string,int> securityMap { {"low",0}, {"medium",1}, {"high",2} };
  map<string,int> subjectSecurityLevel{};
  map<string,int> objectSecurityLevel{};

  void printState(Assests&);

  void scanInstruction(Instruction&, Assests&);
  void logInstruction(string, string);
  void addSubject(ReferenceMonitor*,string&, Assests&);
  void addObject(ReferenceMonitor*,string&, Assests&);
  void executeRead(ReferenceMonitor*,string&, Assests&);
  void executeWrite(ReferenceMonitor*,string&, Assests&);
};

