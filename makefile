CC			= g++

#DEBUG		= -g -D DEBUG

LFLAGS		= -Wall $(DEBUG) $(LIBS) $(LINKS)
CFLAGS		= -Wall -MMD $(DEBUG) $(INCLUDES) $(DEFINES)

TARGET		= game.exe

SRCS		= $(wildcard src/game/*.cpp src/game/*/*.cpp)

OBJS		= $(addprefix build/game/, $(notdir $(SRCS:.cpp=.o)))

DEFINES		= 

INCLUDES	= 

LIBS		= 

LINKS		= -lsfml-window -lsfml-audio -lsfml-graphics -lsfml-system



build: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LFLAGS) -o $(TARGET) $(OBJS)

build/game/%.o: src/game/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ 
build/game/%.o: src/game/*/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -fv $(OBJS) $(OBJS:.o=.d) $(TARGET)

rebuild: clean build

run:
	./$(TARGET)

cleandepend:
	rm -fv $(OBJS:.o=.d)

-include $(OBJS:.o=.d)

valgrind: $(TARGET)
	(valgrind --show-reachable=yes --leak-check=full -v ./$(TARGET))