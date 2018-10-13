BLPSecure : BLPSecure.o ReferenceMonitor.o
	g++ -std=c++14 -o BLPSecure BLPSecure.o ReferenceMonitor.o

ReferenceMonitor.o : ReferenceMonitor.cpp ReferenceMonitor.h Instruction.h Assests.h
		g++ -std=c++14 -c ReferenceMonitor.cpp

BLPSecure.o : BLPSecure.cpp Assests.h Instruction.h
		g++ -std=c++14 -c BLPSecure.cpp

clean : rm BLPSecure BLPSecure.o ReferenceMonitor.o



