CC      		   = g++
APP     		   = Onegin
LOG_FILES          = -D \$$LOG_FILE_NAME=\"Onegin.log\"  -D \$$TRACE_FILE_NAME=\"Onegin.trace\" 
OPTIONS            = -D\$$DO_LOG -D\$$DO_TRACE $(LOG_FILES)
WARNIGS_AND_ERRORS = -Wall -Wextra -Werror
FLAGS   		   = $(WARNIGS_AND_ERRORS) -g -O2 -rdynamic -ldl -finstrument-functions $(OPTIONS)

all: buffer.o onegin_text.o text.o log.o gcc_trace.o stdlib_addition.o
	$(CC) main.cpp buffer.o onegin_text.o text.o stdlib_addition.o log.o gcc_trace.o $(FLAGS) -o $(APP)

buffer.o: buffer.h buffer.cpp log.o gcc_trace.o 
	$(CC) -c buffer.cpp $(FLAGS)

onegin_text.o: onegin_text.h onegin_text.cpp log.o gcc_trace.o
	$(CC) -c onegin_text.cpp $(FLAGS)

text.o: text.h text.cpp log.o gcc_trace.o
	$(CC) -c text.cpp $(FLAGS)

stdlib_addition.o: ../Utilities/stdlib_addition.h ../Utilities/stdlib_addition.cpp log.o gcc_trace.o 
	$(CC) -c ../Utilities/stdlib_addition.cpp $(FLAGS)	

gcc_trace.o: ../Utilities/Log/gcc_trace.h ../Utilities/Log/gcc_trace.cpp
	$(CC) -c ../Utilities/Log/gcc_trace.cpp $(FLAGS)

log.o: ../Utilities/Log/log.h ../Utilities/Log/log.cpp
	$(CC) -c ../Utilities/Log/log.cpp $(FLAGS)

clean:
	rm buffer.o onegin_text.o text.o log.o gcc_trace.o  $(APP)