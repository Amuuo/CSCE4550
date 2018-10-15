blpecure : BLPSecure.o ReferenceMonitor.o Instruction.o
	g++ -std=c++17 -Ofast -o blpsecure BLPSecure.o ReferenceMonitor.o Instruction.o
	rm BLPSecure.o ReferenceMonitor.o Instruction.o

ReferenceMonitor.o : ReferenceMonitor.cpp ReferenceMonitor.h Instruction.h Subject.h Object.h Time.h
		g++ -std=c++17 -Ofast -c ReferenceMonitor.cpp

Instruction.o : Instruction.h
		g++ -std=c++17 -Ofast -c Instruction.cpp

BLPSecure.o : BLPSecure.cpp Instruction.h ReferenceMonitor.h
		g++ -std=c++17 -Ofast -c BLPSecure.cpp

clean : 
	rm blpsecure BLPSecure.o ReferenceMonitor.o Instruction.o
