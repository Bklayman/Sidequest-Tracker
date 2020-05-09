CXX = g++
CXXFLAGS = -g -Wall -std=c++17

#produce a list of all cc files
CXX_FILES = $(wildcard *.cc)

#produce a list of all *.o files by substituting cc in CXX_FILES with o.
OFILES = $(CXX_FILES:cc=o)

#dir in which to generate dependences
DEPDIR = .deps

TARGETS = main

all:		clean $(TARGETS)

main:		$(OFILES)
		$(CXX) $(OFILES) -o $@

.phony:		clean
clean:
		rm -rf *.o *~ $(TARGETS) $(DEPDIR) .gitignore
