#pragma once
#include<string>
#include<map>

using namespace std;


class Assests {

  public:    
  Assests() {}
  ~Assests() {}


  class Object {

    public:
    Object() {}
    Object(const string name) : name{name} {}
    
    //GETTERS    
    const string getName()  const {return name;}
    const int    getValue() const {return value;}
    //SETTERS
    void setName (const string _name) {name=_name;}
    void setValue(const int _value)   {value=_value;}
     
    private:
    string name{};  // object name
    int    value{}; // current object value
  };

  class Subject {

    public:
    Subject() {}
    Subject(const string name) : name{name} {}
    
    //SETTERS    
    void setName(const string _name) {name = _name;}
    void setValue(const int _value)  {temp = _value;}
    //GETTERS
    const string getName() const {return name;}
    const int    getTemp() const {return temp;}    
    //FUNCTIONS
    void readObject(const Object& obj)             {setValue(obj.getValue());}
    void writeObject(Object& obj, const int value) {obj.setValue(value);}
   
    private:
    string name{}; // subject name
    int    temp{}; // current subject value
    
  };


  //GETTERS
  map<string,Subject>& getSubjectMap() {return subjects;}
  map<string,Object>&  getObjectMap()  {return objects;}    
  private:
  map<string,Subject> subjects{}; // map holds subjects that can be called by name
  map<string,Object>  objects{};  // map holds object that can be called by name

};


