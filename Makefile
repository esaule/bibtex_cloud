LD=g++

BIBTEXSPIRIT_INC = -I $(HOME)/prgs/bibtex-spirit/include
CXXFLAGS = $(BIBTEXSPIRIT_INC)

CXXFLAGS += -std=c++11


GTK_INCLUDE = `pkg-config --cflags gtk+-2.0` `pkg-config --cflags cairo`
GTK_LIB = `pkg-config --libs gtk+-2.0``pkg-config --libs cairo`

CXXFLAGS += $(GTK_INCLUDE)

LDFLAGS += GTK_LIB

OBJS = main.o parse.o log.o

main: $(OBJS)
	$(LD) $(GTK_LIB) -o main $(OBJS)

depend: 
	makedepend -f Makefile *.cpp *.hpp -Y util/*.cpp util/*.hpp
# DO NOT DELETE

log.o: log.hpp
main.o: parse.hpp util.hpp CairoTagCloud.hpp cairo_graphic_controller.hpp
main.o: serializable.hpp exception.hpp util/stacktrace.hpp log.hpp
parse.o: util.hpp
CairoTagCloud.o: cairo_graphic_controller.hpp serializable.hpp exception.hpp
CairoTagCloud.o: util/stacktrace.hpp log.hpp
cairo_graphic_controller.o: serializable.hpp exception.hpp
cairo_graphic_controller.o: util/stacktrace.hpp log.hpp
exception.o: util/stacktrace.hpp log.hpp
serializable.o: exception.hpp util/stacktrace.hpp log.hpp
