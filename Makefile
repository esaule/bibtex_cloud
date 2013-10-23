LD=g++

BIBTEXSPIRIT_INC= -I $(HOME)/prgs/bibtex-spirit/include
CXXFLAGS= $(BIBTEXSPIRIT_INC) -std=c++11

main: main.o parse.o
	$(LD) -o main main.o parse.o