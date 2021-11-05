CC      		  = g++
APP     		  = ProtectedStack
IGNORING_WARNIGNS = -Wno-unused-value
LOG_FILES         = -D \$$LOG_FILE_NAME=\"ProtectedStack.log\"  -D \$$TRACE_FILE_NAME=\"ProtectedStack.trace\" 
OPTIONS 		  = -D\$$CANARIES_PROTECTION -D\$$MEMORY_CLEAN_UPS -D\$$STRUCT_VERIFICATION -D\$$HASH_PROTECTION -D\$$DO_LOG -D\$$DO_TRACE $(LOG_FILES)
FLAGS   		  = -Wall -g -O2 -rdynamic -ldl -finstrument-functions $(OPTIONS) $(IGNORING_WARNIGNS)

all: Stack.o ProtectedBuffer.o Memory.o hash.o Canary.o stdlib_addition.o log.o gcc_trace.o 
	$(CC) main.cpp Stack.o ProtectedBuffer.o Memory.o hash.o Canary.o stdlib_addition.o log.o gcc_trace.o $(FLAGS) -o $(APP)

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

stdlib_addition.o: ../Utilities/stdlib_addition.h ../Utilities/stdlib_addition.cpp log.o gcc_trace.o 
	$(CC) -c ../Utilities/stdlib_addition.cpp $(FLAGS)	

gcc_trace.o: ../Utilities/Log/gcc_trace.h ../Utilities/Log/gcc_trace.cpp
	$(CC) -c ../Utilities/Log/gcc_trace.cpp $(FLAGS)

log.o: ../Utilities/Log/log.h ../Utilities/Log/log.cpp
	$(CC) -c ../Utilities/Log/log.cpp $(FLAGS)

clean:
	rm log.o gcc_trace.o Canary.o hash.o Memory.o ProtectedBuffer.o Stack.o $(APP)