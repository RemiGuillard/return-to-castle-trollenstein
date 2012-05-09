NAME =	Trollenstein

FILE =	castle.c	\
	GL_utilities.c	\
	VectorUtils2.c	\
	loadobj.c	\
	LoadTGA2.c	\
	Trollenstein.c	\
	openfile.c	\
	Display.c	\
	MazeUtilities.c	\
	PlayerMovement.c

FLAGS=	-lGL -lglut -lm -DGL_GLEXT_PROTOTYPES

all : castle

castle: ${FILE}
	gcc -Wall -std=c99 -o ${NAME} ${FILE} ${FLAGS}

clean :
	rm ${NAME}
