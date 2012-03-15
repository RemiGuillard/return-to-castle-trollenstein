all : lab4-1 lab4-2 lab4-3

lab4-1: lab4-1.c GL_utilities.c VectorUtils2.c loadobj.c LoadTGA2.c
	gcc -Wall -std=c99 -o lab4-1 lab4-1.c GL_utilities.c VectorUtils2.c loadobj.c LoadTGA2.c -lGL -lglut -lm -DGL_GLEXT_PROTOTYPES 

lab4-2: lab4-2.c GL_utilities.c VectorUtils2.c loadobj.c LoadTGA2.c
	gcc -Wall -std=c99 -o lab4-2 lab4-2.c GL_utilities.c VectorUtils2.c loadobj.c LoadTGA2.c -lGL -lglut -lm -DGL_GLEXT_PROTOTYPES 

lab4-3: lab4-3.c GL_utilities.c VectorUtils2.c loadobj.c LoadTGA2.c
	gcc -Wall -std=c99 -o lab4-3 lab4-3.c GL_utilities.c VectorUtils2.c loadobj.c LoadTGA2.c -lGL -lglut -lm -DGL_GLEXT_PROTOTYPES 

clean :
	rm lab4-1 lab4-2 lab4-3
