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
main.o: parse.hpp util.hpp CairoBibtexCloud.hpp log.hpp
main.o: cairo_graphic_controller.hpp serializable.hpp exception.hpp
main.o: util/stacktrace.hpp gui_interface.hpp gui_option_chooser.hpp
main.o: CairoTagCloud.hpp GuiLayout.hpp util/smart_ptr.hpp
parse.o: util.hpp
CairoBibtexCloud.o: log.hpp cairo_graphic_controller.hpp serializable.hpp
CairoBibtexCloud.o: exception.hpp util/stacktrace.hpp gui_interface.hpp
CairoBibtexCloud.o: gui_option_chooser.hpp CairoTagCloud.hpp GuiLayout.hpp
CairoBibtexCloud.o: util/smart_ptr.hpp
CairoTagCloud.o: cairo_graphic_controller.hpp serializable.hpp exception.hpp
CairoTagCloud.o: util/stacktrace.hpp log.hpp
GuiLayout.o: cairo_graphic_controller.hpp serializable.hpp exception.hpp
GuiLayout.o: util/stacktrace.hpp log.hpp util/smart_ptr.hpp
cairo_graphic_controller.o: serializable.hpp exception.hpp
cairo_graphic_controller.o: util/stacktrace.hpp log.hpp
exception.o: util/stacktrace.hpp log.hpp
gui_option_chooser.o: cairo_graphic_controller.hpp serializable.hpp
gui_option_chooser.o: exception.hpp util/stacktrace.hpp log.hpp
gui_option_chooser.o: gui_interface.hpp
serializable.o: exception.hpp util/stacktrace.hpp log.hpp
