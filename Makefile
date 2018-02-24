#Makefile

CFLAGS=-Wall

LIBS=-framework GLUT -framework OpenGL -framework Cocoa

OFILES=game.o calculations.o textures.o

all : D2R

D2R : $(OFILES)
	gcc $(CFLAGS) $(OFILES) -o $@ $(LIBS)
	
game.o : game.c game.h calculations.h textures.h constants.h
	gcc $(CFLAGS) -c $< -o $@
	
calculations.o : calculations.c calculations.h constants.h
	gcc $(CFLAGS) -c $< -o $@
	
textures.o : textures.c textures.h
	gcc $(CFLAGS) -c $< -o $@

	
clean : 
	@rm -f *.x *.o
	@echo "CLEANED"
