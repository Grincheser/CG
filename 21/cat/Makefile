CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
TARGET = s21_cat

all: $(TARGET)

$(TARGET): s21_cat.o
	$(CC) $(CFLAGS) s21_cat.o -o $(TARGET)

s21_cat.o: s21_cat.c s21_cat.h
	$(CC) $(CFLAGS) -c s21_cat.c

clean:
	rm -f *.o $(TARGET)

re: clean all