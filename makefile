CC			= g++

#DEBUG		= -g -D DEBUG

LFLAGS		= -Wall $(DEBUG) $(LIBS) $(LINKS)
CFLAGS		= -Wall -MMD $(DEBUG) $(INCLUDES) $(DEFINES)

TARGET		= slash

SRCS		= \
$(wildcard src/*.cpp src/*/*.cpp)

OBJS		= $(addprefix build/, $(notdir $(SRCS:.cpp=.o)))

DEFINES		= 

INCLUDES	= 

LIBS		= 

LINKS		= -lsfml-window -lsfml-audio -lsfml-graphics -lsfml-system



build: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LFLAGS) -o $(TARGET) $(OBJS)

build/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ 
build/%.o: src/*/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -fv $(OBJS) $(OBJS:.o=.d) $(TARGET)

rebuild: clean build

run:
	./$(TARGET)

cleandepend:
	rm -fv $(OBJS:.o=.d)

-include $(OBJS:.o=.d)
