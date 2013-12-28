TARGET = philosotree

UNAME=$(shell uname)

DARWIN_BOOST_LIB_PATH = /opt/local/lib
LINUX_BOOST_LIB_PATH = /usr/lib/
DEFAULT_LIB_PATH = /usr/local/lib
BOOST_LIB_PATH = $(DEFAULT_LIB_PATH)

DARWIN_BOOST_INCLUDE_PATH = /opt/local/include/
LINUX_BOOST_INCLUDE_PATH = /usr/include/
DEFAULT_INCLUDE_PATH = /usr/local/include/
BOOST_INCLUDE_PATH = $(DEFAULT_INCLUDE_PATH)

DARWIN_BOOST_LIB = boost_system-mt
LINUX_BOOST_LIB = boost_system


DARWIN_CXX = clang++
LINUX_CXX = g++
DEFAULT_CXX = g++

CXX = $(DEFAULT_CXX)

INCLUDE_FLAGS = -I$(BOOST_INCLUDE_PATH) -I./src/

DARWIN_CXXFLAGS = -Wall -Wextra -O3 $(INCLUDE_FLAGS) -Wno-unused-variable -Wno-unused-parameter -Wno-sign-compare
LINUX_CXXFLAGS = -Wall -Wextra -O3 $(INCLUDE_FLAGS)

DARWIN_LDFLAGS = -L$(BOOST_LIB_PATH) -l$(BOOST_LIB)
LINUX_LDFLAGS = -L$(BOOST_LIB_PATH) -l$(BOOST_LIB)

ifeq ($(UNAME),Darwin)
BOOST_LIB_PATH=$(DARWIN_BOOST_LIB_PATH)
BOOST_INCLUDE_PATH=$(DARWIN_BOOST_INCLUDE_PATH)
BOOST_LIB=$(DARWIN_BOOST_LIB)

CXX = $(DARWIN_CXX)
CXXFLAGS = $(DARWIN_CXXFLAGS)
LDFLAGS = $(DARWIN_LDFLAGS)
endif

ifeq ($(UNAME),Linux)
BOOST_LIB_PATH=$(LINUX_BOOST_LIB_PATH)
BOOST_INCLUDE_PATH=$(LINUX_BOOST_INCLUDE_PATH)
BOOST_LIB=$(LINUX_BOOST_LIB)

CXX = $(LINUX_CXX)
CXXFLAGS = $(LINUX_CXXFLAGS)
LDFLAGS = $(LINUX_LDFLAGS)
endif




all: clear clean $(TARGET) test

%: %.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $*.cpp -o ./bin/$*

clean:
	rm -f $(TARGET)

test:
	./$(TARGET) Kevin_Bacon Philosophy > test_output.txt


clear:
	clear

showos:
	@echo compiling for $(UNAME)
	@echo BOOST_LIB_PATH=$(BOOST_LIB_PATH)
	@echo BOOST_INCLUDE_PATH=$(BOOST_INCLUDE_PATH)
