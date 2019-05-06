vpath %.h = include/
vpath %.cc = src/
vpath %.cpp = src/

CPP = g++
SRC += $(wildcard *.cpp)
SRC += $(wildcard src/*.cc)
SRC += $(wildcard src/*.cpp)

CPPFLAGS += -I ./include
CPPFLAGS += -I	/usr/local/protobuf/include
CPPFLAGS += -I	/usr/local/libevent/include

LDFLAGS += -L  /usr/local/protobuf/lib
LDFLAGS += -L  /usr/local/libevent/lib
LDFLAGS += -lpthread
LDFLAGS += -lprotobuf
LDFLAGS += -lssl
LDFLAGS += -lcrypto
LDFLAGS += -levent
LDFLAGS += -levent_openssl

main:$(SRC)
	$(CPP) $^ -o $@ $(CPPFLAGS) $(LDFLAGS)

SRC_C+= $(wildcard src/*.cc)
SRC_C += $(wildcard src/*.cpp)

FLAGS += -shared
FLAGS += -fPIC
libgw.so:$(SRC_C)
	$(CPP) $(FLAGS) $^ -o $@ $(CPPFLAGS) $(LDFLAGS) 

clean:
	$(RM) main
