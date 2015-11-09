CPP      = g++
OBJ      = rubik.o
LIBS     = -lglut -lGL -lGLU
BIN      = rubik

.PHONY: all

all: $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(OBJ) -o $(BIN) $(LIBS)

rubik.o: rubik.cpp
	$(CPP) -c $< -o $@ $(INCS)
