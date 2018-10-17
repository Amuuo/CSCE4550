blpsecure : BLPSecure.o ReferenceMonitor.o Instruction.o
	g++ -std=c++0x -o blpsecure BLPSecure.o ReferenceMonitor.o Instruction.o
	rm BLPSecure.o ReferenceMonitor.o Instruction.o

ReferenceMonitor.o : ReferenceMonitor.cpp ReferenceMonitor.h Instruction.h Subject.h Object.h
		g++ -std=c++0x -c ReferenceMonitor.cpp

Instruction.o : Instruction.h
		g++ -std=c++0x -c Instruction.cpp

BLPSecure.o : BLPSecure.cpp Instruction.h ReferenceMonitor.h
		g++ -std=c++0x -c BLPSecure.cpp

clean : 
	rm blpsecure BLPSecure.o ReferenceMonitor.o Instruction.o
