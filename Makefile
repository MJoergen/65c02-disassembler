sources  = cpu65c02.cpp
sources += decode.cpp
sources += main.cpp
objects = $(sources:.cpp=.o)
depends = $(sources:.cpp=.d)
CC = gcc
DEFINES  = -Wall -Wextra -O3

all: 65c02-disassembler
	echo

65c02-disassembler: $(objects) Makefile
	$(CC) -o $@ $(DEFINES) $(objects) -lstdc++
	mv $@ $(HOME)/bin

%.d: %.cpp Makefile
	set -e; $(CC) -M $(CPPFLAGS) $(DEFINES) $(INCLUDE_DIRS) $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
		[ -s $@ ] || rm -f $@

include $(depends)

%.o :
	$(CC) $(DEFINES) $(INCLUDE_DIRS) -c $< -o $@


clean: Makefile
	-rm $(objects)
	-rm $(depends)

