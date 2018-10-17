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
#include<string>
#include<sstream>
#include<iostream>
#include<regex>
#include<algorithm>
#include<map>
#include<iomanip>
#include<functional>

using namespace std;


//strings hold color codes
const string BLUE     = "[34m";
const string GREEN    = "[32m";
const string RED      = "[31m";
const string YELLOW   = "[33m";
const string WHITE    = "[37m";
const string CYAN     = "[36m";
const string MAGENTA  = "[35m";
const string BLACK    = "[30m"; 
const string B_CYAN   = "[46m";
const string B_BLACK  = "[40m";
const string B_BLUE   = "[44m";
const string B_RED    = "[41m";
const string B_GREEN  = "[42m";
const string B_WHITE  = "[47m";
const string B_YELLOW = "[43m";
const string BOLD     = "[1m";
const string RESET    = "[0m";


class Instruction {
  
  public:

  Instruction(){}
  Instruction(string&);
  ~Instruction(){}


  map<string,function<void(Instruction*)>> functionMap{
    {"addsub",bind(&Instruction::veriftyAddsub,this)},
    {"addobj",bind(&Instruction::verifyAddobj ,this)},
    {"read"  ,bind(&Instruction::verifyRead   ,this)},
    {"write" ,bind(&Instruction::verifyWrite  ,this)}};
  
  void verifyMethod ();
  void veriftyAddsub();
  void verifyAddobj ();
  void verifyRead   ();
  void verifyWrite  ();
  string constructMsg(string);
  string constructMsg(string, string, string="");

  string method{};    //stores instruction function
  string subject{};   //stores instruction subject
  string object{};    //stores instruction object
  string security{};  //stores instruction security
  int    value{};     //stores instruction value
  
  string instruction{};
};



