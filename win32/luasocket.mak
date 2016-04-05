
LUAJIT_DIR = ..\LuaJIT\src
SOURCE_DIR = ..\luasocket\src
BUILD_DIR = ..\luasocket\bin
SOCKET_DLL = $(BUILD_DIR)\socket\core.dll
MIME_DLL = $(BUILD_DIR)\mime\core.dll

CCFLAGS = /O2 /Ot /I$(LUAJIT_DIR)

SOCKET_SRC = \
	$(SOURCE_DIR)\auxiliar.c \
	$(SOURCE_DIR)\buffer.c \
	$(SOURCE_DIR)\except.c \
	$(SOURCE_DIR)\inet.c \
	$(SOURCE_DIR)\io.c \
	$(SOURCE_DIR)\luasocket.c \
	$(SOURCE_DIR)\options.c \
	$(SOURCE_DIR)\select.c \
	$(SOURCE_DIR)\tcp.c \
	$(SOURCE_DIR)\timeout.c \
	$(SOURCE_DIR)\udp.c \
	$(SOURCE_DIR)\wsocket.c
	
MIME_SRC = \
	$(SOURCE_DIR)\mime.c
	
LUA_FILES = \
	$(SOURCE_DIR)\socket.lua

SOCKET_OBJ = $(SOCKET_SRC:.c=.obj)
MIME_OBJ = $(MIME_SRC:.c=.obj)

all: $(SOCKET_DLL) $(MIME_DLL)

%.obj: %.c
	$(CC) /c /Fo$(<) $(CCFLAGS) $(?)
	
$(SOCKET_DLL): $(SOCKET_OBJ)
	$(LINK) /DLL /out:$(<) $(?)
	# Now copy the lua files
	copy 

$(MIME_DLL): $(MIME_OBJ)
	$(LINK) /DLL /out:$(<) $(?)
	
