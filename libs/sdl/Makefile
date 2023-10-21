CC = g++
CFLAGS = -Wall -Wall -Wextra -fPIC 
INCLUDES = -Iinc
SRCS = src/GraphicHandler.cpp 
OBJS = $(SRCS:.c=.o)
TARGET = libsdl.so

.PHONY: clean fclean re
    
all:    $(TARGET)
	@echo  Successfully compiled a .so library.

$(TARGET): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -shared -o $(TARGET) $(OBJS) -lSDL2

.c.o:
	$(CC) $(CFLAGS) -I/usr/include/SDL2 $(INCLUDES) -cpp $<  -o $@

clean:
	$(RM) *.o 

fclean: clean
	$(RM) $(TARGET)

re: fclean all