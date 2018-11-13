#pragma once
#include<fstream>
#include<iostream>
#include<string>

using namespace std;

template<class T>
struct FileStream {

  FileStream(string _filename); 
  T stream;
  string filename;
};

template<class T>
inline FileStream<T>::FileStream(string _filename) {
  filename = _filename;
  stream.open(filename);
  if ( stream.fail() ) {
    cout << endl << "Stream failed: " << filename << ", exiting..." << endl;
    exit(1);
  }
}
