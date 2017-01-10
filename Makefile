LINK.o=$(LINK.cc)
CXXFLAGS=-Wall -Wextra -g --std=c++11 -MMD
LDLIBS=
EXE=example
OBJS= \
    example.o

all: $(EXE)

$(EXE): $(OBJS)

clean:
	-rm -f $(EXE) $(OBJS) *.d

rebuild: clean all

.PHONY: all clean rebuild

-include *.d
