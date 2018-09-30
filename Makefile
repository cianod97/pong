CXX:=clang++
CXXFLAGS:=-Wall
LDFLAGS:=-lSDL2
SOURCES:=$(wildcard *.cpp)
OBJS:=$(SOURCES:.cpp=.o)
EXECUTABLE:=pong

$(EXECUTABLE): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJS) $(EXECUTABLE)