CC = g++
CFLAGS = -Wall      -Wextra -std=c++20 -fPIC
INCLUDES = -Iinc
SRCS = src/RaylibGraphicLib.cpp \
		src/RaylibTexture.cpp
OBJS = $(SRCS:.c=.o)
TARGET = libraylib.so

.PHONY: clean fclean re
    
all:    $(TARGET)
	@echo  Successfully compiled Raylib shared object.

$(TARGET): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -shared -o $(TARGET) $(OBJS) -Lraylib/src -lraylib

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -cpp $<  -o $@

clean:
	$(RM) *.o 

fclean: clean
	$(RM) $(TARGET)

re: fclean all

# g++ -c test.cpp -Iinclude
# g++ test.o -o sfml-app -Llib -lsfml-graphics -lsfml-window -lsfml-system