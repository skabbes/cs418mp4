TARGET = mp4

CC = g++
CCOPTS=  -g

OBJ = main.o shaderhelp.o model.o
LIBS = -lglut -lGL -lGLU -lGLEW

LINK = g++
LINKOPTS = 

all: $(TARGET)

$(TARGET): $(OBJ)
	$(LINK) $(CCOPTS) $(LINKOPTS) -o $@ $(OBJ) $(LIBS)

%.o : %.cpp
	$(CC) -c $(CCOPTS) $< -o $@

clean:
	rm -rf *.o $(TARGET)
