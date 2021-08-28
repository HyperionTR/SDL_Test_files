#Definimos la macro para los archivos a compilar
OBJ = SDL_Tut_03.cpp

#Definimos los comandos a emplear
CC = g++

#Definimos las opciones del compilador y linker
OPT = -w

#Definimos las opciones del linker
LNK = -lSDL2

#Definimos el nombre de archivo de salida
OUT = SDL_Tut_Program

#Por último, definimos el objetivo 'all' y los comandos a realiazar y sus dependencias
all: $(OBJ)
	$(CC) $(OBJ) -g $(OPT) $(LNK) -o $(OUT)