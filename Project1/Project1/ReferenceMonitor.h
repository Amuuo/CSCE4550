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

#pragma once
#include"Subject.h"
#include"Object.h"
#include"Instruction.h"
#include<map>
#include<functional>
#include<algorithm>
#include<sstream>
#include<vector>
#include<stdexcept>
#include<fstream>
#include<string>
#include<iostream>
#include<iomanip>

using namespace std;
using namespace placeholders;

//const variables for print current state
const vector<string> STATE_BOX_COLUMN_HEADERS = {"subject","temp","object","value"};
const float          STATE_BOX_WIDTH{56};



class ReferenceMonitor {

  using functionMap = map<string,function<void(ReferenceMonitor*,Instruction&)>>;
  enum SecurityLevels {LOW,MEDIUM,HIGH};
  
  public:
  
  ReferenceMonitor(){}
  ~ReferenceMonitor(){}

  void printResult(string);
  void printState();  
  void processRequest    (Instruction&);
  void addSubject        (Instruction&);
  void addObject         (Instruction&);
  void verifyReadAccess  (Instruction&);
  void verifyWriteAccess (Instruction&);
  
  
  private:

  map<string,Subject>  subjectMap;             //map stores all objects
  map<string,Object>   objectMap;              //map stores all subjects
  map<string,int>      subjectSecurityLevel{}; //map stores subjects security clearance
  map<string,int>      objectSecurityLevel{};  //map stores objects security level
  
  //map stores the security level value with a string key
  map<string,int> securityMap{{"low",LOW},{"medium",MEDIUM},{"high",HIGH}};

  //map stores verification functions requested by the input instructions
  functionMap methods{{"addobj",bind(&ReferenceMonitor::addObject,this,_2)},
                      {"addsub",bind(&ReferenceMonitor::addSubject,this,_2)},
                      {"read"  ,bind(&ReferenceMonitor::verifyReadAccess,this,_2)},
                      {"write" ,bind(&ReferenceMonitor::verifyWriteAccess,this,_2)}};
};


