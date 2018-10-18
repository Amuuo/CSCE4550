blpsecure : BLPSecure.o ReferenceMonitor.o Instruction.o
	g++ -std=c++14 -o blpsecure BLPSecure.o ReferenceMonitor.o Instruction.o -pthread
	rm BLPSecure.o ReferenceMonitor.o Instruction.o

ReferenceMonitor.o : ReferenceMonitor.cpp ReferenceMonitor.h Instruction.h Subject.h Object.h
		g++ -std=c++14 -c ReferenceMonitor.cpp

Instruction.o : Instruction.h
		g++ -std=c++14 -c Instruction.cpp

BLPSecure.o : BLPSecure.cpp Instruction.h ReferenceMonitor.h
		g++ -std=c++14 -c BLPSecure.cpp -pthread

clean : 
	rm blpsecure BLPSecure.o ReferenceMonitor.o Instruction.o
