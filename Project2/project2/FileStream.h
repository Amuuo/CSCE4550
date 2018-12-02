#pragma once
#include<fstream>
#include<iostream>
#include<string>

using namespace std;

template<class T> struct FileStream;

template<class T>
struct FileStream {

  FileStream(string _filename); 
  T stream;
  string filename;
  T& operator<< (const FileStream<T>&, uint8_t);
  T& operator<< (const FileStream<T>&, string );
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

template<class T>
inline T & FileStream<T>::operator<<(const FileStream<T>& out, uint8_t val) {
  cout << val;
  out << val;
  return out.stream;
}

template<class T>
inline T & FileStream<T>::operator<<(const FileStream<T>& out, string val) {
  
  cout << val;
  out << val;
  return out.stream;
}
