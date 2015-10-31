CC                 = g++

INCLUDES           = -I../include -Iinclude

CFLAGS             = $(INCLUDES) -c -Wall -pedantic -std=c++11 -finput-charset=UTF-8 -ggdb

LIB_PATHS          = -Llib
LIBS_STATIC_BOOST  = -lboost_system-mt
LIBS_SHARED_BOOST  = -lboost_unit_test_framework-mt
LIBS_SYSTEM        =

LIBS_STATIC        = -Wl,-Bstatic $(LIBS_SYSTEM) $(LIBS_BOOST)
LIBS_SHARED        = -Wl,-Bdynamic $(LIBS_SHARED_BOOST)
LIBS               = $(LIBS_SHARED) $(LIBS_STATIC)

LIBS_LIB           =
LDFLAGS            = ${LIB_PATHS} ${LIBS}
LDFLAGS_LIB        = ${LIB_PATHS} ${LIBS_LIB}
MAKE               = make


find = $(foreach dir,$(1),$(foreach d,$(wildcard $(dir)/*),$(call find,$(d),$(2))) $(wildcard $(dir)/$(strip $(2))))

SOURCES_LIB        = $(call find, src, *.cpp)
SOURCES            = main.cpp $(SOURCES_LIB)
OBJECTS            = $(SOURCES:.cpp=.o)
OBJECTS_LIB        = $(SOURCES_LIB:.cpp=.o)

EXECUTABLE_TARGET = 
ifeq ($(MSYSTEM), MINGW64)
	EXECUTABLE_TARGET = rim_test64.exe
else
	EXECUTABLE_TARGET = rim_test64
endif


check: $(SOURCES) check $(EXECUTABLE_TARGET)
	./$(EXECUTABLE_TARGET) --log_level=test_suite


$(EXECUTABLE_TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)


.cpp.o:
	$(CC) ${CFLAGS} $< -o $@


clean:
	rm -f $(OBJECTS_LIB)
	rm -f $(OBJECTS)
	rm -f $(STATIC_TARGET)
	rm -f $(EXECUTABLE_TARGET)
	rm -f gmon.out

