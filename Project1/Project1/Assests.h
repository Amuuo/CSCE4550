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
    Object(string name) : name{name} {}
    string name{};
    int value{};
  };

  class Subject {

    public:
    Subject() {}
    Subject(string name) : name{name} {}
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

