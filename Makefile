#Makefile

CFLAGS=-Wall

LIBS=-framework GLUT -framework OpenGL -framework Cocoa

OFILES=game.o graphics.o textures.o

all : D2R

D2R : $(OFILES)
	gcc $(CFLAGS) $(OFILES) -o $@ $(LIBS)
	
game.o : game.c game.h graphics.h
	gcc $(CFLAGS) -c $< -o $@
	
graphics.o : graphics.c graphics.h game.h
	gcc $(CFLAGS) -c $< -o $@
	
textures.o : textures.c textures.h
	gcc $(CFLAGS) -c $< -o $@

	
clean : 
	@rm -f *.x *.o
	@echo "CLEANED"
