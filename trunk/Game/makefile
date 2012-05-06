all : castle

castle: castle.c GL_utilities.c VectorUtils2.c loadobj.c LoadTGA2.c
	gcc -Wall -std=c99 -o Trollenstein castle.c GL_utilities.c VectorUtils2.c loadobj.c LoadTGA2.c -lGL -lglut -lm -DGL_GLEXT_PROTOTYPES 

clean :
	rm Trollenstein
