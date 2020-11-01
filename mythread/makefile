###########################
#	Makefile
###########################

#source object target
SOURCE 	:= hook.cpp mycoroutine.cpp mycoroutine.h myscheduler.cpp myscheduler.h ctx.S
OBJS 	:= 
TARGET	:= libmycoroutine.so  

#compile and lib parameter
CXX		:= g++
LIBS	:= 
LDFLAGS	:= -ldl
DEFINES	:=
CFLAGS	:= -shared -fpic
CXXFLAGS:= -std=c++11
 
.PHONY: clean, install

#link parameter
LIB := 

#link
$(TARGET): $(OBJS) $(SOURCE)
	$(CXX) -o $@ $(CFLAGS) $(CXXFLAGS) $(LDFLAGS) $^

#clean
clean:
	rm -fr *.o
	rm -fr *.gch
	rm -rf libmycoroutine.so

install:
	cp libmycoroutine.so /usr/lib64/

