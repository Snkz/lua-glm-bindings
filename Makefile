# Build tool
CC = g++

# Build flags
CPPFLAGS =  -std=c++11 -Wall

# Includes
CPPFLAGS += -Ilib/lua-5.3.1/install/include/ 

# LD Flags
LDFLAGS = -Llib/lua-5.3.1/install/lib/ 

# LD Names
LDLIBS = -ldl -llua


# Source
sample: sample.o

# Build Souurce
all: sample

clean:
	rm -f src/*.o

.PHONY: all clean
