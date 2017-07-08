host = pc
debug = y

ifeq ($(host), arm)
	CC = arm-linux-gnueabi-gcc
else
	CC = gcc
endif

SRC := #
SRC += $(wildcard *.c)

#OBJ := $(subst .c,.o,$(SRC))
OBJ = $(SRC:%.c=%.o)

CFLAGS := #
ifeq ($(debug), y)
	CFLAGS += -g
else
	CFLAGS += -O2
endif
CFLAGS += -Wall
CFLAGS += -std=gnu99

LDFLAGS := #
LDFLAGS += -lpthread
LDFLAGS += -lm

EXEC_NAME = demo
EXEC_PATHNAME = ./$(EXEC_NAME)

$(EXEC_PATHNAME):$(OBJ)
	@echo make ...
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS) 
	@echo make over
	@echo Execute target is $(EXEC_PATHNAME)
$(OBJ):%.o:%.c
	$(CC) $^ -c -o $@ $(CFLAGS)

.PHONY:clean $(EXEC_PATHNAME)
clean:
	@echo "clean ..."
	rm $(EXEC_PATHNAME) $(OBJ) *.o -rf
	@echo "clean over\n"
