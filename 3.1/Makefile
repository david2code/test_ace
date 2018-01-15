TARGETS = ace_tst

OBJECTS:=$(patsubst %.cpp,%.o,$(wildcard *.cpp))

COPTIONS = -Wall -g -o2
CINCDIRS = -I$(ACE_ROOT)
LINKLIBS = -L$(ACE_ROOT)/lib -lACE

CFLAGS = $(COPTIONS) $(CINCDIRS)
CC = g++

$(TARGETS): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGETS) $(OBJECTS) $(LINKLIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	$(RM) $(TARGETS) $(OBJECTS)
