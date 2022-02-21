# Usage:
# make        # compile all binary
# make clean  # remove all binaries and objects

#* Variables

# Name of the project
PROJ_NAME = main

# .cpp files
CPP_SOURCE = ${wildcard ./src/*.cpp}
C_SOURCE = ${wildcard ./src/*.c}

# .hpp files
HPP_SOURCE = ${wildcard ./include/*.hpp}
H_SOURCE = ${wildcard ./include/*.h}

# Object files
AUX_OBJ = ${subst .cpp,.o,${subst src,objects,${CPP_SOURCE}}}
AUX_OBJ += ${subst .c,.o,${subst src,objects,${C_SOURCE}}}

# Adds main separately
CPP_SOURCE += main.cpp
AUX_OBJ += ./objects/main.o
OBJ = ${filter-out main.o,${AUX_OBJ}}

# Compiler
CC = g++

# Compiler flags
CC_FLAGS = -Wall -g -c -O3 -Iinclude


#* Compilation and linking

all: objectsFolder ${PROJ_NAME}

${PROJ_NAME}: ${OBJ}
	${CC} $^ -o $@

./objects/%.o: ./src/%.cpp ./include/%.hpp
	${CC} $< ${CC_FLAGS} -o $@

./objects/%.o: ./src/%.c ./include/%.h
	${CC} $< ${CC_FLAGS} -o $@

./objects/main.o: main.cpp ${H_SOURCE}
	${CC} $< ${CC_FLAGS} -o $@

objectsFolder:
	@mkdir -p objects

clean:
	@rm -rf ./objects/*.o

print:
	@echo ${CPP_SOURCE}
	@echo ${HPP_SOURCE}
	@echo ${OBJ}

.PHONY: all clean
