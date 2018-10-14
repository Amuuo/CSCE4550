BLPSecure : BLPSecure.o ReferenceMonitor.o Instruction.o
	g++ -std=c++17 -Ofast -o BLPSecure BLPSecure.o ReferenceMonitor.o Instruction.o
	rm BLPSecure.o ReferenceMonitor.o Instruction.o

ReferenceMonitor.o : ReferenceMonitor.cpp ReferenceMonitor.h Instruction.h Assests.h
		g++ -std=c++17 -Ofast -c ReferenceMonitor.cpp

Instruction.o : Instruction.h
		g++ -std=c++17 -Ofast -c Instruction.cpp

BLPSecure.o : BLPSecure.cpp Assests.h Instruction.h
		g++ -std=c++17 -Ofast -c BLPSecure.cpp

clean : 
	rm BLPSecure BLPSecure.o ReferenceMonitor.o Instruction.o



