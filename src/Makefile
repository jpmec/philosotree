TARGET = philosotree

UNAME=$(shell uname)

MAC_BOOST_LIB_PATH = /opt/local/lib
LINUX_BOOST_LIB_PATH = /usr/lib/
DEFAULT_LIB_PATH = /usr/local/lib
BOOST_LIB_PATH = $(DEFAULT_LIB_PATH)

MAC_BOOST_INCLUDE_PATH = /opt/local/include/
LINUX_BOOST_INCLUDE_PATH = /usr/include/
DEFAULT_INCLUDE_PATH = /usr/local/include/
BOOST_INCLUDE_PATH = $(DEFAULT_INCLUDE_PATH)

MAC_BOOST_LIB = boost_system-mt
LINUX_BOOST_LIB = boost_system


ifeq ($(UNAME),Darwin)
BOOST_LIB_PATH=$(MAC_BOOST_LIB_PATH)
BOOST_INCLUDE_PATH=$(MAC_BOOST_INCLUDE_PATH)
BOOST_LIB=$(MAC_BOOST_LIB)
endif

ifeq ($(UNAME),Linux)
BOOST_LIB_PATH=$(LINUX_BOOST_LIB_PATH)
BOOST_INCLUDE_PATH=$(LINUX_BOOST_INCLUDE_PATH)
BOOST_LIB=$(LINUX_BOOST_LIB)
endif


CC = g++
CC_FLAGS = -Wall
INCLUDE_FLAGS = -I$(BOOST_INCLUDE_PATH)
LIB_FLAGS = -L$(BOOST_LIB_PATH) -l$(BOOST_LIB)


all: clear clean $(TARGET) test

%: %.cpp
	$(CC) $(CC_FLAGS) $(INCLUDE_FLAGS) $(LIB_FLAGS) $*.cpp -o $*

clean:
	rm -f $(TARGET)

test:
	./$(TARGET) en.wikipedia.org "/w/api.php?action=query&format=json&prop=links&pllimit=max&titles=google" > test_output.txt


clear:
	clear

showos:
	@echo compiling for $(UNAME)
	@echo BOOST_LIB_PATH=$(BOOST_LIB_PATH)
	@echo BOOST_INCLUDE_PATH=$(BOOST_INCLUDE_PATH)
