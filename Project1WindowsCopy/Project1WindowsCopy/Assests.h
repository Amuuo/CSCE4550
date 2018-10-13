#pragma once
#include<string>
#include<map>

using namespace std;


class Assests {

  public:
  Assests() {}
  ~Assests() {}

  struct Subject {
    Subject() {}
    Subject(string name,int temp) : name{name},temp{temp} {}
    string name{};
    int temp{};
  };

  struct Object {
    Object() {}
    Object(string name,int temp) : name{name},temp{temp} {}
    string name{};
    int temp{};
  };

  map<string,Subject> subjects{};
  map<string,Object> objects{};

};

