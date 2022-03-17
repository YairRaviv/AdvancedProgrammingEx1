all: slast

slast:
	gcc -o slast slast.c
clean:
	rm slast
