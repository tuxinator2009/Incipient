CXX=g++
CXX_FLAGS=-Wall -g
LIBS=-lm
SRC_FILES := $(wildcard *.cpp)
BIN_FILES := $(patsubst %.cpp,%,$(SRC_FILES))

all: $(BIN_FILES)

clean:
	rm $(BIN_FILES) $(PTAD_BIN_FILES)

%:%.cpp
	$(CXX) $(CXX_FLAGS) -o $@ $^ $(LIBS)
