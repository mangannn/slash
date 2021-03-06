CC			= g++

#DEBUG		= -g -D DEBUG
LFLAGS		= -Wall $(DEBUG)
CFLAGS		= -Wall -MMD $(DEBUG)

TARGET		= game.exe

SRCS		= $(wildcard src/game/*.cpp src/game/**/*.cpp)
OBJS		= $(patsubst src/%, build/%, $(SRCS:.cpp=.o))
PATH_FS		= $(addsuffix .f, $(dir $(OBJS)))
DEPS		= $(OBJS:.o=.d)

DEFINES		= 
INCLUDES	= 
LIBS		= 
LINKS		= -lsfml-graphics -lsfml-window -lsfml-system


build: $(TARGET)

$(TARGET): $(PATH_FS) $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(TARGET) $(LIBS) $(LINKS)

build/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES) $(DEFINES)

%/.f:
	mkdir -p $(dir $@) && touch $@

clean:
	rm -rv build/

rebuild: clean build

run:
	./$(TARGET)

cleandepend:
	rm -fv $(DEPS)

-include $(DEPS)

valgrind: $(TARGET)
	(valgrind --show-reachable=yes --leak-check=full -v ./$(TARGET))
