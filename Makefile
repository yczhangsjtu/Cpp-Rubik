CPP      = g++
OBJ      = rubik.o
LIBS     = -lglut -lGL -lGLU
BIN      = rubik

ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
  LDFLAGS=-L"/System/Library/Frameworks/OpenGL.framework/Libraries" -framework GLUT -framework OpenGL $(LIBS)
else
  LDFLAGS=$(LIBS)
endif

.PHONY: all

all: $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(OBJ) -o $(BIN) $(LDFLAGS)

rubik.o: rubik.cpp
	$(CPP) -c $< -o $@ $(INCS)
