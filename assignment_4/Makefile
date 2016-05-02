# Makefile
# 2006/02/26
# modified on 2016/5/2 for mingw64 environment
# author: Chen Wenli

# Micro Defination Block
PACKAGE	= raytracer
SRCS	= $(wildcard *.cpp)
HEADS	= $(wildcard *.h)
OBJS	= $(SRCS:.cpp=.o)

FILES	= Makefile $(HEADS) $(SRCS)
VER	= `date +%Y%m%d`

DEBUG	= -ggdb -march=native #-pg # -lefence

# common (*.o)
LD	= g++
LDFLAG	= $(DEBUG)
LDLIBS	= -lm -lopengl32 -lglu32 ../lib/freeglut.lib

# C (*.C)
CC	= gcc
CFLAGS	= -Wall $(DEBUG)
CPPFLAGS= -I/usr/include/g++-3 -I. -I../include

# C++ (*.cpp)
CXX = g++
CXXFLAGS = -Wall --std=c++11 $(DEBUG)

#etc
SHELL = /bin/sh
RM = rm -f
PROF = gprof

# Generate Rule Block
.SUFFIXS:
.SUFFIXS: .o .cpp

all: $(PACKAGE)

$(PACKAGE): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ $(LDLIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# Help Rules Block

clean:
	$(RM) $(PACKAGE) $(OBJS)
	$(RM) core gmon.out *~ #*#

tar:
	@echo $(PACKAGES)-$(VER) > .package
	@$(RM) -r `cat .package`
	@mkdir `cat .package`
	@ln $(FILES) `cat .package`
	tar cvf - `cat .package` | gzip -9 > `cat .package`.tar.gz
	@$(RM) -r `cat .package` .package

zip:
	zip -9 $(PACKAGE)-$(VER).zip $(FILES)

prof: run
	$(PROF) $(PACKAGE) | less

run: all



