# Makefile
# 2006/02/26

# Micro Defination Block
PACKAGE	= raycast
SRCS	= perspective_camera.cpp plane.cpp triangle.cpp transform.cpp group.cpp image.cpp matrix.cpp orthographic_camera.cpp raycast.cpp scene_parser.cpp sphere.cpp
HEADS	= perspective_camera.h plane.h triangle.h transform.h light.h Ray.h camera.h group.h hit.h image.h matrix.h object3d.h orthographic_camera.h scene_parser.h sphere.h vectors.h
OBJS	= $(SRCS:.cpp=.o)

FILES	= Makefile $(HEADS) $(SRCS)
VER	= `date +%Y%m%d`

DEBUG	= -ggdb -pg # -lefence

# common (*.o)
LD	= g++
LDFLAG	= -g $(DEBUG)
LDLIBS	= -lm

# C (*.C)
CC	= gcc
CFLAGS	= -g -O3 -march=pentium4 -msse -msse2 -Wall $(DEBUG)
CPPFLAGS= -I/usr/include/g++-3 -I.

# C++ (*.cpp)
CXX = g++
CXXFLAGS = -g -O3 -march=pentium4 -msse -msse2 -Wall $(DEBUG)

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



