
TARGET = ../hqnes
LIB_QUICKNES = ../quicknes/libquicknes.a
LIB_LUAJIT = ../LuaJIT/src/libluajit.so
LINKFLAGS = -L ../quicknes/ -L ../LuaJIT/src -lSDL2 -lquicknes -lluajit -ldl
CXXFLAGS = -std=c++11 -I ../quicknes -I ../LuaJIT/src -O3 -Wno-multichar -fno-exceptions -fpermissive

SOURCES = \
	../src/hqn.cpp \
	../src/hqn_main.cpp \
	../src/hqn_util.cpp \
	../src/hqn_lua.cpp \
	../src/hqn_lua_emu.cpp \
	../src/hqn_lua_joypad.cpp \
	../src/hqn_lua_mainmemory.cpp \
	../src/hqn_lua_savestate.cpp \
	../src/hqn_gui_controller.cpp \
	../src/options.cpp

OBJECTS = $(SOURCES:.cpp=.o)

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(TARGET): $(OBJECTS) $(LIB_QUICKNES) $(LIB_LUAJIT)
	$(CXX) -o $@ $(OBJECTS) $(LINKFLAGS)

$(LIB_QUICKNES):
	$(MAKE) -C ../quicknes/linux

$(LIB_LUAJIT):
	$(MAKE) -C ../LuaJIT/src

clean:
	$(RM) $(OBJECTS)
	$(RM) $(TARGET)

