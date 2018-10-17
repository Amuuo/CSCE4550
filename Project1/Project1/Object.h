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
#include<iostream>
using namespace std;


class Object {

  public:
  Object() {}
  Object(const string name) : name{name} {}
    
  //GETTERS
  const string getName()  const {return name;}
  const int    getValue() const {return value;}
  //SETTERS
  void setName  (const string _name) {name=_name;}
  void setValue (const int _value) {value=_value;}
    
  private:
  string name{};  // object name
  int    value{}; // current object value
};


