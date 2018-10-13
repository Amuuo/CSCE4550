#include"ReferenceMonitor.h"


int main()
{
  
  ReferenceMonitor ref{};
  ReferenceMonitor referenceMonitor{"input.txt"};
  referenceMonitor.methods["addsub"](referenceMonitor, "hello");

  string t;
  cin >> t;
}