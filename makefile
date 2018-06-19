CC			= g++

#DEBUG		= -g -D DEBUG

LFLAGS		= -Wall $(DEBUG)
CFLAGS		= -Wall -MMD $(DEBUG)

TARGET		= game.exe

SRCS		= $(wildcard src/game/*.cpp src/game/*/*.cpp)

OBJS		= $(addprefix build/game/, $(notdir $(SRCS:.cpp=.o)))

DEFINES		= 

INCLUDES	= 

LIBS		= 

LINKS		= -lsfml-graphics -lsfml-window -lsfml-system



build: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(TARGET) $(LIBS) $(LINKS)

build/game/%.o: src/game/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES) $(DEFINES)
build/game/%.o: src/game/*/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES) $(DEFINES)


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