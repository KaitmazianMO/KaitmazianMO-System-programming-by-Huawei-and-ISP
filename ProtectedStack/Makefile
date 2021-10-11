CC      		  = g++
APP     		  = test
IGNORING_WARNIGNS = -Wno-unused-value
OPTIONS 		  = -D$CANARIES_PROTECTION -D$MEMORY_CLEAN_UPS -D$STRUCT_VERIFICATION -D$HASH_PROTECTION 
FLAGS   		  = -Wall -g -O2 -rdynamic -ldl -lbfd -finstrument-functions $(OPTIONS) $(IGNORING_WARNIGNS)

all: Stack.o ProtectedBuffer.o Memory.o hash.o Canary.o log.o gcc_trace.o
	$(CC) main.cpp Stack.o ProtectedBuffer.o Memory.o hash.o Canary.o log.o gcc_trace.o $(FLAGS)

Stack.o: Stack.h Stack.cpp log.o gcc_trace.o
	$(CC) -c Stack.cpp $(FLAGS)

ProtectedBuffer.o: ProtectedBuffer.h ProtectedBuffer.cpp log.o gcc_trace.o
	$(CC) -c ProtectedBuffer.cpp $(FLAGS)

Memory.o: Memory.h Memory.cpp log.o gcc_trace.o
	$(CC) -c Memory.cpp $(FLAGS)

hash.o: hash.h hash.cpp log.o gcc_trace.o
	$(CC) -c hash.cpp $(FLAGS)

Canary.o: Canary.h Canary.cpp log.o gcc_trace.o
	$(CC) -c Canary.cpp $(FLAGS)

gcc_trace.o: ../Utilities/Log/gcc_trace.h ../Utilities/Log/gcc_trace.cpp
	$(CC) -c ../Utilities/Log/gcc_trace.cpp $(FLAGS)

log.o: ../Utilities/Log/log.h ../Utilities/Log/log.cpp
	$(CC) -c ../Utilities/Log/log.cpp $(FLAGS)
