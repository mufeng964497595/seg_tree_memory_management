CXX=g++
CXXFLAGS=-Wall -std=c++98
MY_INCS=-I./ -I../include -I../common \

VPATH=./ ../common ../include

OUTPUT=test_seg_tree_mem_mgr
SRC=$(wildcard ./*.cpp)
OBJ=$(notdir $(SRC:.cpp=.o))

.PHONY:all
all:${OUTPUT}

test_seg_tree_mem_mgr:test_seg_tree_mem_mgr.o

.SUFFIXES:
.SUFFIXES: .o .cpp
.o:
	${CXX} ${CXXFLAGS} -o $@ $^ ${MY_INCS} ${MY_LIBS}
.cpp.o:
	${CXX} ${CXXFLAGS} -c $< -o $@ ${MY_INCS} ${MY_LIBS} 

.PHONY:clean
clean:
	-rm -f ${OBJ} ${OUTPUT}

