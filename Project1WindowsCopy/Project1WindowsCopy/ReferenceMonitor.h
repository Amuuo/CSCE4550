#pragma once
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
  ReferenceMonitor();
  ReferenceMonitor(string);
  ~ReferenceMonitor();


  struct Instruction
  {
    string function;
    string sub;
    string obj;
    string value;
  };
  
  struct Subject
  {
    Subject();
    Subject(string, int);
    string id;
    int _securityLevel;
    string value{"0"};
  };
  
  struct Object
  {
    Object();
    Object(string, int);
    string id;
    int _securityLevel;
    string value{"0"};
  };

  map<string,function<void(const ReferenceMonitor, ReferenceMonitor*, string&)>> methods {
    {"addobj", &ReferenceMonitor::addObject}, 
    {"addsub", &ReferenceMonitor::addSubject}, 
    {"read",   &ReferenceMonitor::executeRead}, 
    {"write",  &ReferenceMonitor::executeWrite}
  };  
  
  vector<string> instructionHistory{};
  
  map<string,int> securityLevel { {"low",0}, {"medium",1}, {"high",2} };
  map<string,Subject> subjects{};
  map<string,Object> objects{};

  void printState();

  void inputFile(string&);
  void logInstruction(string, string);
  void addSubject(ReferenceMonitor*,string&);
  void addObject(ReferenceMonitor*,string&);
  void executeRead(ReferenceMonitor*,string&);
  void executeWrite(ReferenceMonitor*,string&);
};

