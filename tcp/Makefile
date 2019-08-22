vpath %.h = inc/
vpath %.c = src/

SRC += $(wildcard *.c)
SRC += $(wildcard src/*.c)

CPPFLAGS += -I ./inc
CPPFLAGS += -I ./inc

LDFLAGS += -lrt
LDFLAGS += -lpthread
LDFLAGS += -lmbedtls
LDFLAGS += -lmbedx509
LDFLAGS += -lmbedcrypto

main:$(SRC)
	$(CC) $^ -o $@ $(CPPFLAGS) $(LDFLAGS)

clean:
	$(RM) main
