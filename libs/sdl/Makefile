CC = g++
CFLAGS = -Wall -Werror -Wextra -fPIC -std=c++20
INCLUDES = -Iinc
SRCS = src/SDLGraphicLib.cpp 
OBJS = $(SRCS:.c=.o)
TARGET = libsdl.so

.PHONY: clean fclean re
    
all:    $(TARGET)
	@echo  Successfully compiled a SDL shared object.

$(TARGET): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -shared -o $(TARGET) $(OBJS) -lSDL2

.c.o:
	$(CC) $(CFLAGS) -I/usr/include/SDL2 $(INCLUDES) -cpp $<  -o $@

clean:
	$(RM) *.o 

fclean: clean
	$(RM) $(TARGET)

re: fclean all