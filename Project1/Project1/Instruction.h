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
using namespace placeholders;
const string badLead = string(25,' ');

class Instruction {
  
  public:
  Instruction();
  Instruction(Instruction&&);
  Instruction(const Instruction&);
  Instruction(string&);
  ~Instruction();  


  map<string,function<void(Instruction*)>> functionMap{
    {"addsub",bind(&Instruction::verifty_addsub,this)},
    {"addobj",bind(&Instruction::verify_addobj ,this)},
    {"read"  ,bind(&Instruction::verify_read   ,this)},
    {"write" ,bind(&Instruction::verify_write  ,this)}};
  
  void verify_method ();
  void verifty_addsub();
  void verify_addobj ();
  void verify_read   ();
  void verify_write  ();
  string constructErrorMsg(string);
  static string constructErrorMsg(string, string);

  string method{};
  string subject{};
  string object{};
  string security{};
  int    value{};
  
  string instruction{};
};



