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
    Object(string name,int value) : name{name}, value{value} {}
    string name{};
    int value{};
  };

  class Subject {

    public:
    Subject() {}
    Subject(string name,int temp) : name{name},temp{temp} {}
    string name{};
    int temp{};
    
    void readObject(const Object& obj) {
      temp = obj.value;
    }
    void writeObject(Object& obj, int value) {
      obj.value = value;
    }
  };


  map<string,Subject> subjects{};
  map<string,Object> objects{};

};

