#pragma once
#include<map>
#include<functional>
#include<utility>
#include<sstream>
#include<vector>
#include<stdexcept>
#include<fstream>
#include<string>
#include<iostream>

using namespace std;



class ReferenceMonitor
{
  public:
  ReferenceMonitor();
  ReferenceMonitor(string);
  ~ReferenceMonitor();

  enum SecurityLevel { LOW, MEDIUM, HIGH };

  struct Instruction
  {
    string function;
    string sub;
    string obj;
    string value;
  };

  map<string,function<void(const ReferenceMonitor,string)>> methods {
    {"addobj", &ReferenceMonitor::addObject}, 
    {"addsub", &ReferenceMonitor::addSubject}, 
    {"read",   &ReferenceMonitor::executeRead}, 
    {"write",  &ReferenceMonitor::executeWrite}
  };  
  
  vector<string> instructionHistory{};
  
  map<string,SecurityLevel> subject{};
  map<string,SecurityLevel> objects{};

  void printState();

  void addSubject(string);
  void addObject(string);
  void executeRead(string);
  void executeWrite(string);
};

