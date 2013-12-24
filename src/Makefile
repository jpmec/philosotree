TARGET = philosotree
CC = g++
CC_FLAGS = -Wall


all: clear clean $(TARGET) test

%: %.cpp
	$(CC) $(CC_FLAGS) $*.cpp -o $*

clean:
	rm -f $(TARGET)

test:
	./$(TARGET)

clear:
	clear
