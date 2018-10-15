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
#include<iostream>
#include<string>
#include"Object.h"
using namespace std;


class Subject {

  public:
  Subject() {}
  Subject(const string name) : name{name} {}
    
  //SETTERS    
  void setName  (const string _name) {name=_name;}
  void setValue (const int _value)   {temp=_value;}
  //GETTERS
  const string getName() const {return name;}
  const int    getTemp() const {return temp;}    
    
  //FUNCTIONS
  void readObject(const Object& obj) {setValue(obj.getValue());}
  void writeObject(Object& obj, const int value) {obj.setValue(value);}
   
  private:
  string name{}; // subject name
  int    temp{}; // current subject value    
};

