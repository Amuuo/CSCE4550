#pragma once
#include<string>
#include<map>


using namespace std;


class Assests {

  public:
  Assests();
  ~Assests();

  struct Subject {
    Subject();
    Subject(string,int);
    string name{};
    int value{};
  };

  struct Object {
    Object();
    Object(string,int);
    string name{};
    int value{};
  };

  map<string,Subject> subjects{};
  map<string,Object> objects{};

};

