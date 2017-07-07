CC = gcc
CFLAGS = -g -Wall -O0
LFLAGS = -lseccomp
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

TARGET = ish

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	$(RM) $(TARGET) $(OBJS) *~
