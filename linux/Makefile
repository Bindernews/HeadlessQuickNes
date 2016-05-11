
LIB_QUICKNES = ../quicknes/libquicknes.a
LINKFLAGS = -L ../quicknes/ -lSDL2_ttf -lquicknes -ldl `pkg-config --libs sdl2 luajit`
CXXFLAGS = -std=c++11 -I ../quicknes -O3 -Wno-multichar -fno-exceptions -fpermissive -fPIC \
		   `pkg-config --cflags sdl2 luajit`

SOURCES = \
	../src/hqn.cpp \
	../src/hqn_util.cpp \
	../src/hqn_lua.cpp \
	../src/hqn_lua_emu.cpp \
	../src/hqn_lua_gui.cpp \
	../src/hqn_lua_input.cpp \
	../src/hqn_lua_joypad.cpp \
	../src/hqn_lua_mainmemory.cpp \
	../src/hqn_lua_savestate.cpp \
	../src/hqn_gui_controller.cpp \
	../src/hqn_surface.cpp
#   ../src/hqn_main.cpp    <-- Don't include this because it's not necessary for shared build

OBJECTS = $(SOURCES:.cpp=.o) \

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

# Build as shared library
../libhqnes.so: $(OBJECTS) $(LIB_QUICKNES)
	$(CXX) -o $@ $(OBJECTS) $(LINKFLAGS) -fPIC -shared

$(LIB_QUICKNES):
	$(MAKE) -C ../quicknes/linux

clean:
	$(RM) $(OBJECTS)
	$(RM) $(LIB_QUICKNES)
	$(RM) ../hqnes.so ../hqnes

