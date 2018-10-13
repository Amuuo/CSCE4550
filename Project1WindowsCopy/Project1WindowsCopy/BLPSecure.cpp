#include"ReferenceMonitor.h"


int main()
{
  ReferenceMonitor referenceMonitor{"input.txt"};
  for (auto line : referenceMonitor.instructionHistory) {
    cout << "\n" << line;
  }

  string t;
  cin >> t;
}