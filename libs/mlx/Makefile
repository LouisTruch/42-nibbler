CC = g++
CFLAGS = -Wall      -Wextra -fPIC -std=c++20 -g
INCLUDES = -Ilib/include/MLX42
SRCS = src/MlxGraphicLib.cpp 
OBJS = $(SRCS:.c=.o)
TARGET = libmlx.so

.PHONY: clean fclean re
    
all:    $(TARGET)
	@echo  Successfully compiled MLX shared object.

$(TARGET): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -shared -o $(TARGET) $(OBJS) ./lib/build/libmlx42.a -ldl -lglfw -pthread -lm
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -cpp $<  -o $@

clean:
	$(RM) *.o 

fclean: clean
	$(RM) $(TARGET)

re: fclean all