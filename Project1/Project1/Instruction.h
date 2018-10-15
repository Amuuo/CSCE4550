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

const string BLUE    = ";34";
const string GREEN   = ";32";
const string RED     = ";31";
const string YELLOW  = ";33";
const string WHITE   = ";37";
const string B_BLUE  = ";44";
const string B_RED   = ";41";
const string B_GREEN = ";42";
const string B_WHITE = ";47";
const string BOLD    = ";1";


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
  string constructMsg(string);
  static string constructMsg(string, string, string="0");

  string method{};
  string subject{};
  string object{};
  string security{};
  int    value{};
  
  string instruction{};
};



