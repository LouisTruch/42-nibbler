CC = g++
CXXFLAGS = -Wall -Werror -Wextra -fPIC -std=c++20 -D_REENTRANT 

LDFLAGS := -LSDL/build/build/.libs -lSDL2 
INCLUDES = -ISDL/include

SRCS = $(wildcard srcs/*.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRCS))
DEPENDS := $(patsubst %.cpp, %.d, $(SRCS))
TARGET = libsdl.so

ifeq ($(DEBUG), 1)
	CXXFLAGS += -g
	CXXFLAGS += -DDEBUG
endif

ifeq ($(MANUAL_INSTALLSDL), 1)
	CXXFLAGS += -DMANUAL_INSTALLSDL
endif 

.PHONY: clean fclean re

yellow = /bin/echo -e "\x1B[33m$1\x1b[0m"
all:    $(TARGET)
	@$(call yellow,"Successfully compiled SDL shared object.")

-include $(DEPENDS)

$(TARGET): $(OBJS) 
	$(CXX) -shared -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o : %.cpp Makefile
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

clean:
	$(RM) $(OBJS) $(DEPENDS)

fclean: clean
	$(RM) $(TARGET)

re: fclean all