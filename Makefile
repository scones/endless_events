CC             = g++
INCLUDES       = -Iinclude
ifeq ($(MSYSTEM), MINGW64)
	INCLUDES += -I/mingw64/include/freetype2 -IC:/msys64/mingw64/include/freetype2
endif
CFLAGS         = $(INCLUDES) -c -Wall -pedantic -std=c++11 -fexec-charset=UTF-8 -finput-charset=UTF-8 -DGLEW_STATIC -ggdb

LIB_PATHS      = -Llib
LIBS           = -lglew32 -static -lglfw3 -lopengl32 -lgdi32 -lfreetype64
LIBS_LIB       =
LDFLAGS        = ${LIB_PATHS} ${LIBS} -mwindows
LDFLAGS_LIB    = ${LIB_PATHS} ${LIBS_LIB}
MAKE           = mingw32-make


find = $(foreach dir,$(1),$(foreach d,$(wildcard $(dir)/*),$(call find,$(d),$(2))) $(wildcard $(dir)/$(strip $(2))))

SOURCES_LIB       = $(call find, src, *.cpp)
SOURCES           = $(SOURCES_LIB) main.cpp
OBJECTS           = $(SOURCES:.cpp=.o)
OBJECTS_LIB       = $(SOURCES_LIB:.cpp=.o)
STATIC_TARGET     = lib/libeye.a
EXECUTABLE_TARGET = endless.exe


all: $(SOURCES) $(STATIC_TARGET) check $(EXECUTABLE_TARGET)
#	./$(EXECUTABLE_TARGET)


$(EXECUTABLE_TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)


$(STATIC_TARGET): $(OBJECTS_LIB)
	ar rcs $@ $(OBJECTS_LIB)


check: $(STATIC_TARGET)
#	+$(MAKE) -C ./tests check


.cpp.o:
	$(CC) ${CFLAGS} $< -o $@


clean:
#	$(MAKE) -C ./tests clean
	rm -f $(OBJECTS_LIB)
	rm -f $(OBJECTS)
	rm -f $(STATIC_TARGET)
	rm -f $(EXECUTABLE_TARGET)
	rm -f gmon.out

