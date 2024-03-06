build:
	gcc -o tema3 -std=c11 -g main.c functii.c
run:
	./tema3
mem-check:
	valgrind --leak-check=full --track-origins=yes ./tema3 2
clean:
	rm tema3