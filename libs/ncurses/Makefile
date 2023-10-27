CC = g++
CFLAGS = -Wall -Werror -Wextra -fPIC -std=c++20
INCLUDES = -Iinc
SRCS = src/NcursesGraphicLib.cpp 
OBJS = $(SRCS:.c=.o)
TARGET = libncurses.so

.PHONY: clean fclean re
    
all:    $(TARGET)
	@echo  Successfully compiled a ncurse shared object.

$(TARGET): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -shared -o $(TARGET) $(OBJS) -lncurses

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -cpp $<  -o $@

clean:
	$(RM) *.o 

fclean: clean
	$(RM) $(TARGET)

re: fclean all