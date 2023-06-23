all:
	@gcc source/main.c -I include -lX11 -o RVM
	./RVM test.rasm