
TARGET = exmple
CC = cc
CUR_DIR = $(shell pwd)
INCLUDE = -I $(CUR_DIR)/inc
OBJS = main.o elog.o
override CFLAGS += -Wall -g -std=c99 -c

LDFLAGS += \
-Xlinker -Map="./$(TARGET).map" \


.PHONY:all clean cleanobjs cleantarget debug

all: $(TARGET)

exmple: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

main.o: main.c elog.h
	$(CC) $(INCLUDE) -c $< -o $@ 

elog.o: elog.c elog.h
	$(CC) $(INCLUDE) -c $< -o $@ 

clean: cleanobjs cleantarget

cleanobjs:
	rm -rf *.o
	
cleantarget:
	rm -rf $(TARGET)*

debug: 
	@echo "CFLAGS=$(CFLAGS)"
