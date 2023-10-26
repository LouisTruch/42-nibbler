CC = g++
CFLAGS = -Wall -Wall -Wextra -fPIC -std=c++20
INCLUDES = -Iinc
SRCS = src/SfmlGraphicLib.cpp 
OBJS = $(SRCS:.c=.o)
TARGET = libsfml.so

.PHONY: clean fclean re
    
all:    $(TARGET)
	@echo  Successfully compiled SFML shared object.

$(TARGET): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -shared -o $(TARGET) $(OBJS) -Llib/lib -lsfml-graphics -lsfml-window -lsfml-system

.c.o:
	$(CC) $(CFLAGS) -Ilib/include $(INCLUDES) -cpp $<  -o $@

clean:
	$(RM) *.o 

fclean: clean
	$(RM) $(TARGET)

re: fclean all

# g++ -c test.cpp -Iinclude
# g++ test.o -o sfml-app -Llib -lsfml-graphics -lsfml-window -lsfml-system