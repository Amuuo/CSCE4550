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
#include"Assests.h"
#include"Instruction.h"
#include"Time.h"
#include<map>
#include<unordered_map>
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
#include<ctime>
#define PAGE_WIDTH 90

using namespace std;
using namespace placeholders;

enum SecurityLevels {LOW,MEDIUM,HIGH};


class ReferenceMonitor {

  using functionMap = map<string,function<void(ReferenceMonitor*,Instruction&,Assests&)>>;      
  public:                                           
  ReferenceMonitor();
  ~ReferenceMonitor();

  static string printInstructionResult(string,string="");
  static void logTitle(string);
  
  void printState      (Assests&);
  void scanInstruction (Instruction&, Assests&);
  void addSubject      (Instruction&, Assests&);
  void addObject       (Instruction&, Assests&);
  void executeRead     (Instruction&, Assests&);
  void executeWrite    (Instruction&, Assests&);
  void logResult(string, string="");
  
 
  //map<string,int> instructionHistory;
  //vector<string>  instructionHistory{};   //vector logs all instruction results
  map<string,int> subjectSecurityLevel{}; //map stores subjects security clearance
  map<string,int> objectSecurityLevel{};  //map stores objects security level  
  
  //map stores the security level value with a string key
  map<string,int> securityMap{{"low",LOW},{"medium",MEDIUM},{"high",HIGH}};

  //map stores functions functions called for by the input
  functionMap methods{{"addobj",bind(&ReferenceMonitor::addObject,this,_2,_3)},
                      {"addsub",bind(&ReferenceMonitor::addSubject,this,_2,_3)},
                      {"read"  ,bind(&ReferenceMonitor::executeRead,this,_2,_3)},
                      {"write" ,bind(&ReferenceMonitor::executeWrite,this,_2,_3)}};
};


