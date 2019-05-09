vpath %.h = inc/
vpath %.c = src/

SRC += $(wildcard *.c)
SRC += $(wildcard src/*.c)

CPPFLAGS += -I ./inc

LDFLAGS += -lpthread
LDFLAGS += -lssl
LDFLAGS += -lcrypto
LDFLAGS += -lcjson


main:$(SRC)
	$(CC) $^ -o $@ $(CPPFLAGS) $(LDFLAGS)

clean:
	$(RM) main
