
# https://www.jianshu.com/p/ff0e0e26c47a
# makefile
# Author:	tkz
# Date:	2020/12/05

TARGET := a
CC := gcc

deps = $(shell find ./ -name "*.h")
SRC = $(shell find ./ -name "*.c")

PWD := $(shell pwd)

INCS := -I/home/pi/tkz 
LIBS := -L/home/pi/tkz

OBJS = $(SRC:%.c=%.o)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o  $@

%.o: %.c $(deps)
	$(CC) -c $< -o $@

clean:
	rm -rf $(OBJS) $(TARGET)





