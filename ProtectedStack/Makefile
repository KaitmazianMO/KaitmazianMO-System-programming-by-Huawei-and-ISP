CC      = g++
APP     = test
OPTIONS = -D$CANARIES_PROTECTION -D$MEMORY_CLEAN_UPS 
FLAGS   = -g -O2 -rdynamic -ldl -lbfd -finstrument-functions $(OPTIONS)

all: Stack.o ProtectedBuffer.o Memory.o hash.o Canary.o log.o gcc_trace.o
	$(CC) main.cpp Stack.o ProtectedBuffer.o Memory.o hash.o Canary.o log.o gcc_trace.o $(FLAGS)

Stack.o: Stack.h Stack.cpp
	$(CC) -c Stack.cpp $(FLAGS)

ProtectedBuffer.o: ProtectedBuffer.h ProtectedBuffer.cpp
	$(CC) -c ProtectedBuffer.cpp $(FLAGS)

Memory.o: Memory.h Memory.cpp
	$(CC) -c Memory.cpp $(FLAGS)

hash.o: hash.h hash.cpp
	$(CC) -c hash.cpp $(FLAGS)

Canary.o: Canary.h Canary.cpp
	$(CC) -c Canary.cpp $(FLAGS)

gcc_trace.o: ../Utilities/Log/gcc_trace.h ../Utilities/Log/gcc_trace.cpp
	$(CC) -c ../Utilities/Log/gcc_trace.cpp $(FLAGS)

log.o: ../Utilities/Log/log.h ../Utilities/Log/log.cpp
	$(CC) -c ../Utilities/Log/log.cpp $(FLAGS)
