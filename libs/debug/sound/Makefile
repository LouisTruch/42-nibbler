CC = g++
CXXFLAGS = -Wall -Werror -Wextra -fPIC -std=c++20
SRCS = $(wildcard srcs/*.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRCS))
DEPENDS := $(patsubst %.cpp, %.d, $(SRCS))
TARGET = libsounddebug.so

ifeq ($(DEBUG), 1)
	CXXFLAGS += -g
	CXXFLAGS += -DDEBUG
endif

.PHONY: clean fclean re

yellow = /bin/echo -e "\x1B[33m$1\x1b[0m"
all:    $(TARGET)
	@$(call yellow,"Successfully compiled SoundDebug shared object.")

-include $(DEPENDS)

$(TARGET): $(OBJS) 
	$(CXX) -shared -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o : %.cpp Makefile
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	$(RM) $(OBJS) $(DEPENDS)

fclean: clean
	$(RM) $(TARGET)

re: fclean all